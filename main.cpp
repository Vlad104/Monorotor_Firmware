#include "mbed.h"
#include "src/params.h"
#include "src/Controller.h"
#include "src/Dozator.h"

#ifdef TESTS
    #include "tests/tests.h"
#endif

// Timer for AccelStepper lib
// makes delay for step driver
Timer t;

Dozator dozator_A(STEP_A, DIR_A);
Dozator dozator_B(STEP_B, DIR_B);

int main() {
    t.start();
    Controller control;
    control.start_control(&dozator_A, &dozator_B);

    #ifndef TESTS
        control.loop();
    #else 
        tests(control);
    #endif
}