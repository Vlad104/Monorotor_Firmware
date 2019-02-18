#include "MultiDozator.h"

MultiDozator::MultiDozator() :
{
	dozators_[0] = new Dozator(STEP_A, DIR_A);
	dozators_[1] = new Dozator(STEP_B, DIR_B);
}

MultiDozator::MultiDozator(const DataModel& data) :
{
	dozators_[0] = new Dozator(STEP_A, DIR_A);
	dozators_[1] = new Dozator(STEP_B, DIR_B);
	data_ = data;
}

MultiDozator::~MultiDozator() {
	delete dozators_[0];
	delete dozators_[1];
}

void MultiDozator::set_data(const DataModel& data) {
	data_ = data;
}

void MultiDozator::claculate() {
	float 

	float volume_A = data_.volume * data_.ratio_A;
	float volume_B = data_.volume * data_.ratio_B;

	float feedrate_A = data_.feedrate * data_.ratio_A / 60;
	float feedrate_B = data_.feedrate * data_.ratio_B / 60;

	float accel_A = data_.accel * data_.ratio_A / 60;
	float accel_B = data_.accel * data_.ratio_B / 60;

	init_dozator(dozators[0], volume_A, feedrate_A, accel_A, data_.gear_A);
	init_dozator(dozators[1], volume_A, feedrate_B, accel_B, data_.gear_B);
}

void MultiDozator::init_dozator(Dozator* dozator, float volume, float feedrate,
								float accel, float gear) 
{
	dozator->set_gear(volume);		//  set gear first
	dozator->set_volume(volume);
	dozator->set_feedrate(volume);
	dozator->set_accel(volume);
}

void MultiDozator::start() {
	claculate();
	dozator_A_->start_movable();
	dozator_B_->start_movable();
}

void MultiDozator::run() {
	while (1) {
		dozator_A_->run();
		dozator_B_->run();
	}
}