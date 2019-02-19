#include "MultiDozator.h"

MultiDozator::MultiDozator()
{
    dozators_[0] = new Dozator(STEP_A, DIR_A);
    dozators_[1] = new Dozator(STEP_B, DIR_B);
}

MultiDozator::MultiDozator(const DataModel& data)
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

void MultiDozator::calculate_volume(float volume) {
    data_.volume = volume;

    float volume_A = data_.volume * data_.ratio_A;
    float volume_B = data_.volume * data_.ratio_B;

    dozators_[0]->set_volume(volume_A); 
    dozators_[1]->set_volume(volume_B); 
}

void MultiDozator::calculate_feedrate(float feedrate) {
    data_.feedrate = feedrate;

    float feedrate_A = data_.feedrate * data_.ratio_A / 60;
    float feedrate_B = data_.feedrate * data_.ratio_B / 60;

    dozators_[0]->set_feedrate(feedrate_A); 
    dozators_[1]->set_feedrate(feedrate_B); 
}

void MultiDozator::calculate_accel(float accel) {
    data_.accel = accel;

    float accel_A = data_.accel * data_.ratio_A / 60;
    float accel_B = data_.accel * data_.ratio_B / 60;

    dozators_[0]->set_accel(accel_A);   
    dozators_[1]->set_accel(accel_B);   
}  

void MultiDozator::calculate_all(float ratio_A) {
    data_.ratio_A = ratio_A;
    data_.ratio_B = 1.0 - ratio_A;

    calculate_volume(data_.volume);
    calculate_feedrate(data_.feedrate);
    calculate_accel(data_.accel);
}

void MultiDozator::set_dozator_gear(int num, float gear) {
    if (num > 0 && num < 2) {
        dozators_[num]->set_gear(gear);
    }
}

void MultiDozator::start() {
    dozators_[0]->start_movable();
    dozators_[1]->start_movable();
}

void MultiDozator::stop() {
    dozators_[0]->stop_movable();
    dozators_[1]->stop_movable();
}

void MultiDozator::continues_start() {
    dozators_[0]->continues_movable();
    dozators_[1]->continues_movable();
}

void MultiDozator::run() {
    while (1) {
		dozators_[0]->run();
		dozators_[1]->run();
    }
}