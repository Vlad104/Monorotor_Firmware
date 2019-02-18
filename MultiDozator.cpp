#include "MultiDozator.h"
#include <cmath>

MultiDozator::MultiDozator() :
{
	dozator_A_ = new Dozator(STEP_A, DIR_A);
	dozator_B_ = new Dozator(STEP_B, DIR_B);
}

MultiDozator::~MultiDozator() {
	delete dozator_A_;
	delete dozator_B_;
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

	init_dozator(dozator_A_, volume_A, feedrate_A, reverse_A, accel_A, gear_A_, ratio_A_);
	init_dozator(dozator_B_, volume_B, feedrate_B, reverse_B, accel_B, gear_B_, ratio_B_);
}

void MultiDozator::init_dozator(Dozator* dozator, int32_t volume, uint32_t feedrate,
						float accel, int32_t reverse, float gear, float ratio) 
{
	dozator->set_volume(volume);
	dozator->set_feedrate(volume);
	dozator->set_accel(volume);
	dozator->set_reverse(volume);
	dozator->set_gear(volume);
	dozator->set_ratio(volume);
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