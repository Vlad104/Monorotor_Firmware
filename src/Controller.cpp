#include "Controller.h"

Controller::Controller() {
    usb_ = new Serial(USB_TX, USB_RX);
    rs485_ = new Serial(UART_TX, UART_RX);
    last_port_ = usb_;

    usb_->baud(UART_BAUDRATE);
    rs485_->baud(UART_BAUDRATE);

    usb_->attach(this, &Controller::usb_event, Serial::RxIrq);
    rs485_->attach(this, &Controller::rs485_event, Serial::RxIrq);

    transmit_enable_ = new DigitalOut(UART_EN);
    transmit_enable_->write(0);

    #ifdef TEST
        usb_->printf("Monorotor Firmware 3.1\r\n");
    #endif
}

Controller::~Controller() {
    delete rs485_;
    delete usb_;
    delete transmit_enable_;
}

void Controller::rs485_event() {
    serial_event(rs485_);
}

void Controller::usb_event() {
    serial_event(usb_);
}

void Controller::serial_event(Serial* port) {
    last_port_ = port;
    char message_type = port->getc();

    if (message_type == '=') {
        read_command(port);
    } else if (message_type == '@') {
        read_params(port);
    } else {
        send_answer(port, '?');
    }
}

void Controller::read_command(Serial* port) {
    char temp = port->getc();
    make_command(temp);
}

void Controller::read_params(Serial* port) {
    char params_type = port->getc();
    char temp = port->getc();

    char buffer_1[STR_SIZE+1];
    char buffer_2[STR_SIZE+1];

    int size = 0;
    while (temp != '*' && size < STR_SIZE) {
        buffer_1[size] = temp;
        temp = port->getc();
        size++;
    }

    for (int i = size; i < STR_SIZE; i++) {
        buffer_1[i] = '0';        
    }

    int start = 0;
    if (buffer_1[0] == '-') {
        buffer_2[0] = '-';
        start = 1;
    }

    int ofset = STR_SIZE - size;
    for (int i = 0; i < ofset; i++) {
        buffer_2[i + start] = '0';
    }

    for (int i = start; i < size; i++) {
        buffer_2[ofset + i] = buffer_1[i];
    }

    buffer_1[STR_SIZE] = '\0';
    buffer_2[STR_SIZE] = '\0';

    #ifdef TEST
        usb_->printf("command: %c: %s\r\n", params_type, buffer_1);
        usb_->printf("command: %c: %s\r\n", params_type, buffer_2);
    #endif
    make_params(params_type, buffer_2);
    send_answer(port, '!');
}

void Controller::make_command(char command) {
    #ifdef TEST
        usb_->printf("command: %c\r\n", command);
        dozators_.print(usb_);
    #endif
    switch (command) {
        case '0':
            #ifdef TEST
                usb_->printf("stop\r\n");
            #endif
            dozators_.stop();
            break;
        case '1':
            #ifdef TEST
                usb_->printf("start\r\n");
            #endif
            dozators_.start();
            break;
        case '2':
            #ifdef TEST
                usb_->printf("continues\r\n");
            #endif
            dozators_.continues_start();            
            break;
        default:
            break;
    }
}


void Controller::make_params(char params_type, const char str_buffer[STR_SIZE]) {
    float value = str_to_float(str_buffer);
    #ifdef TEST
        usb_->printf("params value: %c: %f\r\n", params_type, value);
    #endif
    switch (params_type) {
        case 'D':     
            dozators_.set_dozator(str_buffer[0]);
            break;
        case 'V':     
            dozators_.calculate_volume(value);
            break;
        case 'F':
            dozators_.calculate_feedrate(value);
            break;
        case 'A':
            dozators_.calculate_accel(value);
            break;
        case 'a':
            dozators_.calculate_gear(value, -1);
            break;
        case 'b':
            dozators_.calculate_gear(-1, value);
            break;
        case 'r':
            dozators_.calculate_ratio(value);
            break;
        default:
            break;
    }
}

float Controller::str_to_float(const char str_buffer[STR_SIZE]) {
    float value = atof(str_buffer);
    return value;
}

void Controller::loop() {
    bool was_stopped = false;
    while (1) {
        dozators_.run(was_stopped);

        if (was_stopped) {
            was_stopped = false;
            send_answer(last_port_, '!');
        }
    }
}

void Controller::send_answer(Serial* port, char answer) {
    transmit_enable_->write(1);
    port->putc(answer);
    transmit_enable_->write(0);
}