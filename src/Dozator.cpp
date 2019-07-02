#include "Dozator.h"

Dozator::Dozator(PinName step_pin, PinName dir_pin) :
    // AccelStepper(1, step_pin, dir_pin),
    AccelStepper(step_pin, dir_pin),
    volume_(0), feedrate_(0.0), accel_(0.0)
{
    setMinPulseWidth(PULSE_WIDTH);
    setMaxSpeed(MOTOR_MAX_SPEED); 
    setCurrentPosition(0);
    setPinsInverted(false, false, false);
}

void Dozator::set_volume(float volume) {
    if (volume > 100000000.0 || volume < -100000000.0) {
        volume_ = (long) volume;
        return;
    }
    volume_ = (volume > 0.0) ? (long) (volume + 0.5) : (long) (volume - 0.5);
}

void Dozator::set_reverse(float reverse) {
    if (reverse > 100000000.0 || reverse < -100000000.0) {
        reverse_ = (long) reverse;
        return;
    }
    reverse_ = (reverse > 0.0) ? (long) (reverse + 0.5) : (long) (reverse - 0.5);
}

void Dozator::set_feedrate(float feedrate) {
    feedrate_ = feedrate;
}

void Dozator::set_accel(float accel) {
    accel_ = accel;
}

void Dozator::start_movement() {
    setCurrentPosition(0);
    setAcceleration(accel_);  
    move(volume_);
    setMaxSpeed(feedrate_);
}

void Dozator::start_reverse() {
    setCurrentPosition(0);
    setAcceleration(accel_);
    move(-reverse_);
    setMaxSpeed(feedrate_);
}

void Dozator::stop_movement() {
    setCurrentPosition(0); 
    move(0);
}

void Dozator::continues_movement() {
    setCurrentPosition(0);
    setAcceleration(accel_);
    move(1000000000);
    setMaxSpeed(feedrate_);
}

#ifdef DEBUG   
    void Dozator::print(Serial* port) {            
        port->printf("\r\nDozator:\r\n");
        port->printf("volume_: %ld\r\n", volume_);
        port->printf("reverse_: %ld\r\n", reverse_);
        port->printf("feedrate_: %f\r\n", feedrate_);
        port->printf("accel_: %f\r\n", accel_);
    }
#endif