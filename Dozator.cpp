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

void Dozator::volume(uint32_t volume) {
	volume_ = volume;
}

void Dozator::set_feedrate(uint32_t feedrate) {
	feedrate_ = feedrate;
}

void Dozator::set_accel(uint32_t accel) {
	accel_ = accel;
}

void Dozator::set_reverse(uint32_t reverse) {
	reverse_ = reverse;
}

void Dozator::set_gear(uint32_t gear) {
	gear_ = gear;
}

void Dozator::set_ratio(uint32_t ratio) {
	ratio_ = ratio;
}

void Dozator::set_dir(bool dir) {
	dir_ = dir;
}

void Dozator::start_movement() {
	int32_t volume = (dir_) ? volume_ : -1*volume_;
	stepper_->setCurrentPosition(0);
    stepper_->setAcceleration(/*uint32_t*/);  
    stepper_->move(/*int32_t*/);
    stepper_->setMaxSpeed(/*uint32_t*/);
}

void Dozator::stop_movement() {
	stepper_->setCurrentPosition(0); 
    stepper_->move(0);
}