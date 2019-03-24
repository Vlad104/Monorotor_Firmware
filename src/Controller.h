#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "mbed.h"
#include <cstdlib>

#include "params.h"
#include "Dozator.h"
#include "MultiDozator.h"

class Controller {
public:
    Controller();
    ~Controller();

	void start_control(Dozator* A, Dozator* B);
    void loop();

private:
    Serial*         usb_; 
    Serial*         rs485_;
    Serial*         last_port_;
    DigitalOut*     transmit_enable_;

    MultiDozator    dozators_;

    void usb_event();
    void rs485_event();
    void serial_event(Serial* port);
    void read_command(Serial* port);
    void read_params(Serial* port);
    void make_command(char command);
    void make_params(char message_type, const char str_buffer[STR_SIZE]);
    float str_to_float(const char str_buffer[STR_SIZE]);
    void send_answer(Serial* port, char answer);

};

#endif // CONTROLLER_H