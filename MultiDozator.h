#ifndef MULTI_DOZATOR_H
#define MULTI_DOZATOR_H

#include "params.h"
#include "Dozator.h"
#include "DataModel.h"

class MultiDozator {
public:
	MultiDozator();
	//MultiDozator(const DataModel& data);
	~MultiDozator();

	void calculate();

	void start();
	void reverse();
	void stop();
	void run();

private:
	DataModel data_;

	Dozator* dozator_A_;
	Dozator* dozator_B_;

	void init_dozator(Dozator* dozator, int32_t volume, uint32_t feedrate,
						float accel, int32_t reverse, float gear, float ratio);
}


#endif // MULTI_DOZATOR_H