#ifndef MULTI_DOZATOR_H
#define MULTI_DOZATOR_H

#include "params.h"
#include "Dozator.h"

class MultiDozator {
public:
	MultiDozator();
	~MultiDozator();

	void set_volume(int32_t volume);
	void set_feedrate(uint32_t feedrate);
	void set_accel(float accel);
	void set_reverse(int32_t reverse);
	void set_gear_A(float gear_A);
	void set_gear_B(float gear_B);
	void set_ratio_A(float ratio_A);
	void set_ratio_B(float ratio_B);
	void set_dir(bool dir);

	void calculate();

	void start();
	void reverse();
	void stop();
	void run();

private:
	int32_t volume_;
	uint32_t feedrate_;
	float accel_;
	int32_t reverse_;
	float gear_A_;
	float gear_B_;
	float ratio_A_;
	float ratio_B_;
	bool dir_;

	Dozator* dozator_A_;
	Dozator* dozator_B_;

	void init_dozator(Dozator* dozator, int32_t volume, uint32_t feedrate,
						float accel, int32_t reverse, float gear, float ratio);
}


#endif // MULTI_DOZATOR_H