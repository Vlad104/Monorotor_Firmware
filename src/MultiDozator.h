#ifndef MULTI_DOZATOR_H
#define MULTI_DOZATOR_H

#include "params.h"
#include "Dozator.h"

#ifdef TEST   
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
    void calculate_ratio(float ratio_A);
    void calculate_gear(float gear_A, float gear_B);
    void calculate_all();

    void set_dozator(char dozator);

    void start();
    void stop();
    void continues_start();

    // inline func
    void run(bool& was_stopped)  {
    if (is_run_ && dozator_A_->run() && dozator_B_->run()) {
        is_run_ = false;
        was_stopped = true;
    }
}

    #ifdef TEST   
        void print(Serial* port);
    #endif

private:
    struct DataModel {
        DataModel() :
            dozator('2'),
            volume(0),
            feedrate(0),
            accel(0),
            gear_A(1),
            gear_B(1),
            ratio_A(0.5),
            ratio_B(0.5)
        {}
        char    dozator;
        float   volume;     // ul
        float   feedrate;   // ul/min
        float   accel;      // ul/min*sec
        float   gear_A;     // ul to impulses
        float   gear_B;     // ul to impulses
        float   ratio_A;    // ratio A to all
        float   ratio_B;    // ratio A to all
    };
    DataModel   data_;
    Dozator*    dozator_A_;
    Dozator*    dozator_B_;
    bool        is_run_;
};

#endif // MULTI_DOZATOR_H