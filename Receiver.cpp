#include "Receiver.h"

Receiver::Receiver() {
	rs485_ = new Serial(UART_TX, UART_RX);
	usb_ = new Serial(USB_TX, USB_RX);

	rs485_->baud(UART_BAUDRATE);
	usb_->baud(UART_BAUDRATE);

	rs485_->attach(&rs485_event, Serial::RxIrq);
	usb_->attach(&usb_event,     Serial::RxIrq);

	transmit_enable_ = new(UART_EN);
	transmit_enable_->write(0);
}

Receiver::~Receiver() {
	delete rs485_;
	delete usb_;
	delete transmit_enable_;
}

void Receiver::rs485_event() {
	serial_event(rs485_);
}

void Receiver::usb_event() {
	serial_event(usb_);
}

void Receiver::serial_event(Serial* port) {
	char message_type = port->getc();

	if (message_type >= '0' && message_type <= '3') {
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

void Receiver::make_command(char command) {
	switch (command) {
		case '0':
			//stop;
			break;
		case '1':
			//start;
			break;
		case '2':
			//inf start;
			break;
		case '3':
			//stop;
			break;
		default:
			//
			break;
	}
}

void Receiver::make_params(char message_type, const std::string& str_buffer) {
	switch (message_type) {
		case 'V':
			//stop;
			break;
		case 'F':
			//start;
			break;
		case 'R':
			//inf start;
			break;
		case 'a':
			//stop;
			break;
		case 'b':
			//stop;
			break;
		case 'r':
			//stop;
			break;
		default:
			//
			break;
	}
}