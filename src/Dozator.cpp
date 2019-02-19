#include "Dozator.h"

Dozator::Dozator(PinName step_pin, PinName dir_pin) :
    step_pin_(step_pin), dir_pin_(dir_pin),
    gear_(0), volume_(0), feedrate_(0), accel_(0)
{
    stepper_ = new AccelStepper(1, step_pin, dir_pin);    // PinName
    //stepper_ = new AccelStepper(1, STEP_A, DIR_A);
    stepper_->setMinPulseWidth(PULSE_WIDTH);
    stepper_->setMaxSpeed(MOTOR_MAX_SPEED); 
    stepper_->setCurrentPosition(0);
    stepper_->setPinsInverted(true, true, false);
}

Dozator::~Dozator() {
    delete stepper_;
}

void Dozator::set_gear(float gear) {
    gear_ = gear;
    
    set_volume(volume_);
    set_feedrate(feedrate_);
    set_accel(accel_);
}

void Dozator::set_volume(float volume) {
    float temp = volume * gear_;
    volume_ = ((int32_t) temp) + 1;     // + 1 is rounds
}

void Dozator::set_feedrate(float feedrate) {
    float temp = feedrate * gear_;
    feedrate_ = ((int32_t) temp) + 1;   // + 1 is rounds
}

void Dozator::set_accel(float accel) {
    float temp = accel * gear_;
    accel_ = ((int32_t) temp) + 1;      // + 1 is rounds
}

void Dozator::start_movement() {
    stepper_->setCurrentPosition(0);
    stepper_->setAcceleration(accel_);  
    stepper_->move(volume_);
    stepper_->setMaxSpeed(feedrate_);
}

void Dozator::stop_movement() {
    stepper_->setCurrentPosition(0); 
    stepper_->move(0);
}

void Dozator::continues_movement() {
    stepper_->setCurrentPosition(0);
    stepper_->setAcceleration(accel_);  
    stepper_->move(1000000000);
    stepper_->setMaxSpeed(feedrate_);   
}

void Dozator::run() {
    stepper_->run();
}

bool Dozator::stopped() {
	return stepper_->distanceToGo() == 0;
}