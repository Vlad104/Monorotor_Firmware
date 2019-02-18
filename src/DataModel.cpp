#include "DataModel.h"

DataModel::DataModel() :
	volume_(0.0), feedrate_(0.0), accel_(0.0),
	gear_A_(1.0), gear_B_(1.0), ratio_A_(0.0)
{}

DataModel::DataModel(float volume, float feedrate, float accel, 
				float gear_A, float gear_B, float ratio_A) :

	volume_(volume), feedrate_(feedrate), accel_(accel), 
	gear_A_(gear_A), gear_B_(gear_B), ratio_A_(ratio_A)
{}