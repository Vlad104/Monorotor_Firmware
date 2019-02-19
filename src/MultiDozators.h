#ifndef MULTI_DOZATOR_H
#define MULTI_DOZATOR_H

#include "params.h"
#include "Dozator.h"
#include "DataModel.h"

class MultiDozator {
public:
	MultiDozator();
	MultiDozator(const DataModel& data);	// ??
	~MultiDozator();

	void set_data(const DataModel& data);	// ??

	void calculate_volume(float volume);
	void calculate_feedrate(float feedrate);
	void calculate_accel(float feedrate);
	void calculate_all(float ratio_A);
	void set_dozator_gear(int num, float gear);

	void start();
	void stop();
	void continues_start();
	void run(bool& was_stopped);

private:
	DataModel	data_;
	Dozator*	dozators_[2];
	bool		is_run_;
};

#endif // MULTI_DOZATOR_H