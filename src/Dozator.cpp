#include "Dozator.h"

Dozator::Dozator(int step_pin, int dir_pin) :
	step_pin_(step_pin), dir_pin_(dir_pin),
	volume_(0), feedrate_(0), accel_(0), 
	reverse_(0), gear_(0), ratio_(0), dir_(true)
{
	stepper_ = new AccelStepper(1, step_pin, dir_pin);
	stepper_->setMinPulseWidth(PULSE_WIDTH);
	stepper_->setMaxSpeed(MOTOR_MAX_SPEED);	
    stepper_->setCurrentPosition(0);
    stepper_->setPinsInverted(true, true, false);
}

Dozator::~Dozator() {
	delete stepper_;
}

void Dozator::set_gear(uint32_t gear) {
	gear_ = gear;
	
	set_volume(volume_);
	set_feedrate(feedrate_);
	set_accel(accel_);
}

void Dozator::set_volume(float volume) {
	float temp = volume * gear_;
	volume_ = ((int32_t) temp) + 1;		// + 1 is rounds
}

void Dozator::set_feedrate(uint32_t feedrate) {
	float temp = feedrate * gear_;
	feedrate_ = ((int32_t) temp) + 1;	// + 1 is rounds
}

void Dozator::set_accel(uint32_t accel) {
	float temp = accel * gear_;
	accel_ = ((int32_t) temp) + 1;		// + 1 is rounds
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

void Dozator::continues_movable() {
	stepper_->setCurrentPosition(0);
    stepper_->setAcceleration(accel_);  
    stepper_->move(1000000000);
    stepper_->setMaxSpeed(feedrate_);	
}