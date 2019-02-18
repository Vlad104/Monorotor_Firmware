#include "mbed.h"
#include "params.h"
//#include "AccelStepper.h"
//#include <FlashIAP.h>
//#include <stdlib.h>

DataModel		data;
MultiDozator	dozators;
Receiver		com_port;

Timer			t;


int main() {

	dozators.set_data(data);

	dozators.run();	

}
