#include "DataModel.h"

DataModel::DataModel() :
	volume(0.0), feedrate(0.0), accel(0.0),
	gear_A(1.0), gear_B(1.0), ratio_A(0.0)
{}

DataModel::DataModel(float new_volume, float new_feedrate, float new_accel, 
				float new_gear_A, float new_gear_B, float new_ratio_A) :

	volume(volume), feedrate(feedrate), accel(accel), 
	gear_A(gear_A), gear_B(gear_B), ratio_A(ratio_A)
{}