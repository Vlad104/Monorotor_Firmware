#ifndef DOZATOR_H
#define DOZATOR_H

#include "params.h"

class Dozator : public AccelStepper {
public:
	Dozator(int step_pin, int dir_pin);
	~Dozator();

	void set_volume(uint32_t volume);
	void set_feedrate(uint32_t feedrate);
	void set_accel(uint32_t accel);
	void set_reverse(uint32_t reverse);
	void set_gear(uint32_t gear);
	void set_ratio(uint32_t ratio);
	void set_dir(bool dir);

	void start_movement();
	void stop_movement();

private:
	int step_pin_;
	int dir_pin_;

	uint32_t volume_;
	uint32_t feedrate_;
	uint32_t accel_;
	uint32_t reverse_;
	uint32_t gear_; ////////// double
	uint32_t ratio_; ////////// double
	bool dir_;

	AccelStepper* stepper_;

}

#endif // Dozator