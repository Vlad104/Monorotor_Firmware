#ifndef DATA_MODEL_H
#define DATA_MODEL_H

class DataModel {
public:
	DataModel();
	DataModel(int32_t volume, uint32_t feedrate, float accel, 
				int32_t reverse, float gear_A, float gear_B, 
				float ratio_A, float ratio_B, bool dir);

	~DataModel() = default;
	
	void set_volume(int32_t volume);
	void set_feedrate(uint32_t feedrate);
	void set_accel(float accel);
	void set_reverse(int32_t reverse);
	void set_gear_A(float gear_A);
	void set_gear_B(float gear_B);
	void set_ratio_A(float ratio_A);
	void set_ratio_B(float ratio_B);
	void set_dir(bool dir);

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
};

#endif // DATA_MODEL_H