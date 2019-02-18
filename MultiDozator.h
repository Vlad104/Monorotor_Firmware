#ifndef MULTI_DOZATOR_H
#define MULTI_DOZATOR_H

#include "params.h"
#include "Dozator.h"
#include "DataModel.h"

class MultiDozator {
public:
	MultiDozator();
	MultiDozator(const DataModel& data);
	~MultiDozator();

	void set_data(const DataModel& data);

	void calculate();

	void start();
	void reverse();
	void stop();
	void run();

private:
	DataModel	data_;
	Dozator*	dozators_[2];

	void init_dozator(Dozator* dozator, float volume, float feedrate,
								float accel, float gear, float ratio);
};

#endif // MULTI_DOZATOR_H