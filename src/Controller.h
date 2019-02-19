#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../mbed.h"
#include <string>

#include "params.h"
#include "DataModel.h"
#include "MultiDozator.h"

class Controller {
public:
	Controller();
	~Controller();

	void loop();

private:
	Serial* 		rs485_;
	Serial* 		usb_; 
	DigitalOut*		transmit_enable_;

	DataModel		data_;
	MultiDozator	dozators_;

	static void rs485_event();
	static void usb_event();
	void serial_event(Serial* port);
	void make_command(char command);
	void make_params(char message_type, const std::string& str_buffer);

};

#endif // CONTROLLER_H