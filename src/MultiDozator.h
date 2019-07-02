#ifndef MULTI_DOZATOR_H
#define MULTI_DOZATOR_H

#include "params.h"
#include "Dozator.h"
#include "DataModel.h"

#ifdef DEBUG   
    #include "mbed.h"
#endif

class MultiDozator {
public:
    MultiDozator();
    ~MultiDozator();

    void activate_dozators(Dozator* A, Dozator* B);

    void calculate_volume(float volume);
    void calculate_feedrate(float feedrate);
    void calculate_accel(float feedrate);
    void calculate_reverse(float reverse);
    void calculate_ratio(float ratio_A);
    void calculate_gear(float gear_A, float gear_B);
    void calculate_all();

    void set_dozator(char dozator);

    void start();
    void reverse();
    void stop();
    void continues_start();

    void save_params();

    // inline func
    void run(bool& was_stopped) {
        int counter_A = 0;
        int counter_B = 0;

        // ход конем так сказать
        while (dozator_A_->run() && ++counter_A < times_A_);
        while (dozator_B_->run() && ++counter_B < times_B_);

        if (is_run_ && dozator_A_->distanceIsNull() && dozator_B_->distanceIsNull()) {
            is_run_ = false;
            if (need_reverse_) {
                reverse();
            } else {
                was_stopped = true;
            }
        }
    }

    #ifdef DEBUG
        void print(Serial* port);
    #endif
    
#ifndef TESTS
private:
#endif
    DataModel   data_;
    Dozator*    dozator_A_;
    Dozator*    dozator_B_;
    bool        is_run_;
    bool        need_reverse_;

    int times_A_;
    int times_B_;
};

#endif // MULTI_DOZATOR_H