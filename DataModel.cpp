#include "DataModel.h"
///#include <cmath>

DataModel::DataModel() :
	volume_(0), feedrate_(0), accel_(0), reverse_(0),
	gear_A_(1), gear_B_(1), ratio_A_(0), ratio_B_(0), dir_(true)
{}

DataModel::DataModel(int32_t volume, uint32_t feedrate, float accel, 
				int32_t reverse, float gear_A, float gear_B, 
				float ratio_A, float ratio_B, bool dir) :

	volume_(volume), feedrate_(feedrate), accel_(accel), 
	reverse_(reverse), gear_A_(gear_A), gear_B_(gear_B), 
	ratio_A_(ratio_A), ratio_B_(ratio_B), dir_(dir)
{}

void DataModel::volume(int32_t volume) {
	volume_ = volume;
}

void DataModel::set_feedrate(uint32_t feedrate) {
	feedrate_ = feedrate;
}

void DataModel::set_accel(float accel) {
	accel_ = accel;
}

void DataModel::set_reverse(uint32_t reverse) {
	reverse_ = reverse;
}

void DataModel::set_gear(float gear_A) {
	gear_A_ = gear_A;
}

void DataModel::set_gear(float gear_B) {
	gear_B_ = gear_B;
}

void DataModel::set_ratio(float ratio_A) {
	ratio_A_ = ratio_A;
}

void DataModel::set_ratio(float ratio_B) {
	ratio_B_ = ratio_B;
}

void DataModel::set_dir(bool dir) {
	dir_ = dir;
}