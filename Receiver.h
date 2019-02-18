#ifndef RECEIVER_H
#define RECEIVER_H

#include "mbed.h"
#include "params.h"
#include <string>

class Receiver : public Serial {
public:
	Receiver();
	~Receiver();

	static void read_message(Serial& port);

private:
	Serial* 	rs485_;
	Serial* 	usb_; 
	DigitalOut*	transmit_enable_;

	void rs485_event();
	void usb_event();
	void serial_event(Serial* port);
	void make_command(char command);
	void make_params(char message_type, const std::string& str_buffer);

};

#endif // RECEIVER_H