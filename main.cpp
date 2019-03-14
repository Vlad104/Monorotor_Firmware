#include "mbed.h"
//#include "src/params.h"
#include "src/Controller.h"
//#include <FlashIAP.h>

Timer t;

int main() {
    t.start();
	Controller control;
    control.loop();
}