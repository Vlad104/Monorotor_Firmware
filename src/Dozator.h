#ifndef DOZATOR_H
#define DOZATOR_H

#include "mbed.h"
#include "params.h"
#include "AccelStepper.h"

class Dozator : public AccelStepper {
public:
    Dozator(PinName step_pin, PinName dir_pin);
    ~Dozator() {};

    void set_volume(float volume);
    void set_reverse(float reverse);
    void set_feedrate(float feedrate);
    void set_accel(float accel);

    void start_movement();
    void start_reverse();
    void stop_movement();
    void continues_movement();

    #ifdef DEBUG
        void print(Serial* port);
    #endif

#ifndef TESTS
private:
#endif
    long    volume_;    // imp
    long    reverse_;   // imp
    float   feedrate_;  // imp/sec
    float   accel_;     // imp/sec^2
};

#endif // Dozator