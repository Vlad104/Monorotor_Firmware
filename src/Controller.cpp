#include "Controller.h"

Controller::Controller() {
	rs485_ = new Serial(UART_TX, UART_RX);
	usb_ = new Serial(USB_TX, USB_RX);

	rs485_->baud(UART_BAUDRATE);
	usb_->baud(UART_BAUDRATE);

	rs485_->attach(&Controller::rs485_event, Serial::RxIrq);
	usb_->attach(&Controller::usb_event, Serial::RxIrq);

	transmit_enable_ = new DigitalOut(UART_EN);
	transmit_enable_->write(0);
}

Controller::~Controller() {
	delete rs485_;
	delete usb_;
	delete transmit_enable_;
}

void Controller::rs485_event() {
	Controller.serial_event(rs485_);
}

void Controller::usb_event() {
	Controller.serial_event(usb_);
}

void Controller::serial_event(Serial* port) {
	char message_type = port->getc();

	if (message_type >= '0' && message_type <= '2') {
		char temp = port->getc();
		make_command(temp);
		return;
	} 

	std::string buffer = "";
	char temp = port->getc();

	while(temp != '|') {
		buffer += temp;
		temp = port->getc();
	}
	make_params(message_type, buffer);
}

void Controller::make_command(char command) {
	switch (command) {
		case '0':
			dozators.stop();
			break;
		case '1':
			dozators.start();
			break;
		case '2':
			dozators.continues_start();
			break;
		default:
			break;
	}
}

void Controller::make_params(char message_type, const std::string& str_buffer) {
	switch (message_type) {
		case 'V':
			float volume = std::stof(str_buffer);
			dozators_.calculate_volume(volume);
			break;
		case 'F':
			float feedrate = std::stof(str_buffer);
			dozators_.calculate_feedrate(feedrate);
			break;
		case 'A':
			float accel = std::stof(str_buffer);
			dozators_.calculate_accel(accel);
			break;
		case 'a':
			float gear_A = std::stof(str_buffer);
			dozators_->set_dozator_gear(0, gear_A);
			break;
		case 'b':
			float gear_B = std::stof(str_buffer);
			dozators_->set_dozator_gear(1, gear_B);
			break;
		case 'r':
			float ratio_A = std::stof(str_buffer);
			dozators_.calculate_all(ratio_A);
			break;
		default:
			break;
	}
}

void Controller::loop() {
	dozators.run();
}