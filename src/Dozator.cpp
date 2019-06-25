#include "Dozator.h"

Dozator::Dozator(PinName step_pin, PinName dir_pin) :
    // AccelStepper(1, step_pin, dir_pin),
    AccelStepper(step_pin, dir_pin),
    volume_(0), feedrate_(0), accel_(0)
{
    setMinPulseWidth(PULSE_WIDTH);
    setMaxSpeed(MOTOR_MAX_SPEED); 
    setCurrentPosition(0);
    setPinsInverted(true, true, false);
}

void Dozator::set_volume(float volume) {
    volume_ = ((int32_t) volume) + 1;
}

void Dozator::set_feedrate(float feedrate) {
    feedrate_ = ((uint32_t) feedrate) + 1;
}

void Dozator::set_accel(float accel) {
    accel_ = ((uint32_t) accel) + 1;
}

void Dozator::start_movement() {
    setCurrentPosition(0);
    setAcceleration(accel_);  
    move(volume_);
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

// bool Dozator::stopped() { // inline
//     return distanceToGo() == 0;
// }

#ifdef TEST   
    void Dozator::print(Serial* port) {            
        port->printf("\r\nDozator:\r\n");
        port->printf("volume_: %d\r\n", volume_);
        port->printf("feedrate_: %d\r\n", feedrate_);
        port->printf("accel_: %d\r\n", accel_);
    }
#endif