#include "mbed.h"
#include "src/params.h"
#include "src/Controller"
//#include <FlashIAP.h>

Timer t;
Controller control;

int main() {
    t.start();
    control.loop();
}