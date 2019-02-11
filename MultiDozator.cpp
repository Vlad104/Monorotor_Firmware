#include "MultiDozator.h"ъ
#include <cmath>

MultiDozator::MultiDozator() :
	volume_(0), feedrate_(0), accel_(0), reverse_(0),
	gear_A_(1), gear_B_(1), ratio_A_(0), ratio_B_(0), dir_(true)
{
	dozator_A_ = new Dozator(STEP_A, DIR_A);
	dozator_B_ = new Dozator(STEP_B, DIR_B);
}

MultiDozator::~MultiDozator() {
	delete dozator_A_;
	delete dozator_B_;
}

void MultiDozator::volume(int32_t volume) {
	volume_ = volume;
}

void MultiDozator::set_feedrate(uint32_t feedrate) {
	feedrate_ = feedrate;
}

void MultiDozator::set_accel(float accel) {
	accel_ = accel;
}

void MultiDozator::set_reverse(uint32_t reverse) {
	reverse_ = reverse;
}

void MultiDozator::set_gear(float gear_A) {
	gear_A_ = gear_A;
}

void MultiDozator::set_gear(float gear_B) {
	gear_B_ = gear_B;
}

void MultiDozator::set_ratio(float ratio_A) {
	ratio_A_ = ratio_A;
}

void MultiDozator::set_ratio(float ratio_B) {
	ratio_B_ = ratio_B;
}

void MultiDozator::set_dir(bool dir) {
	dir_ = dir;
}

void MultiDozator::claculate() {
	float f_volume = volume_;
	float f_feedrate = feedrate_;
	float f_accel = accel_;
	float f_reverse = reverse_;

	float f_volume_A = f_volume * gear_A_ * ratio_A_;
	float f_volume_B = f_volume * gear_B_ * ratio_B_;

	float round_f_volume_A = roundf(f_volume_A);
	float round_f_volume_B = roundf(f_volume_B);

	int32_t volume_A = (int32_t) round_f_volume_A;
	int32_t volume_B = (int32_t) round_f_volume_B;

	uint32_t reverse_A = (uint32_t) (f_reverse * gear_A_ * ratio_A_);
	uint32_t reverse_B = (uint32_t) (f_reverse * gear_B_ * ratio_B_);

	float round_accuracy_A = round_f_volume_A / f_volume_A;
	float round_accuracy_B = round_f_volume_B / f_volume_B;

	float feedrate_A = f_feedrate * gear_A_ * ratio_A_ / (round_accuracy_A * 60);
	float feedrate_B = f_feedrate * gear_B_ * ratio_B_ / (round_accuracy_B * 60);

	float accel_A = f_accel * gear_A_ * ratio_A_ / (round_accuracy_A * 60);
	float accel_B = f_accel * gear_B_ * ratio_B_ / (round_accuracy_B * 60);

	init_dozator(/*  */);
	init_dozator(/*  */);
}

void MultiDozator::init_dozator(Dozator* dozator, int32_t volume, uint32_t feedrate,
						float accel, int32_t reverse, float gear, float ratio) {

}

void MultiDozator::start() {
	claculate();
	dozator_A_->start_movable();
	dozator_A_->stop_movable();
}