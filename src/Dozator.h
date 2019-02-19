#ifndef DOZATOR_H
#define DOZATOR_H

#include "params.h"
#include "AccelStepper.h"

class Dozator {
public:
	Dozator(int step_pin, int dir_pin);
	~Dozator();

	void set_gear(float gear);			// gear sets firstly
	void set_volume(float volume);
	void set_feedrate(float feedrate);
	void set_accel(float accel);

	void start_movement();
	void stop_movement();
	void continues_movement();
	void run();

private:
	int step_pin_;
	int dir_pin_;

	float		gear_;		// ul to imp
	int32_t		volume_;	// imp
	uint32_t	feedrate_;	// imp/sec
	uint32_t	accel_;		// imp/sec^2

	AccelStepper* stepper_;
};

#endif // Dozator