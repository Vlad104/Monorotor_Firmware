#include "src/params.h"
#include "mbed.h"
#include "src/Controller.h"

#ifdef TESTS

int tests(Controller& c);
void run_tests(Controller& c);
bool test_controler_str_to_float(Controller& c);
bool test_dozator_set_volume(Controller& c);
bool test_multidozator_calculate_volume(Controller& c);
bool test_multidozator_calculate_feedrate(Controller& c);
bool test_multidozator_calculate_accel(Controller& c);
bool test_multidozator_calculate_ratio(Controller& c);
bool test_multidozator_calculate_gear(Controller& c);
bool test_multidozator_calculate_all(Controller& c);
bool test_datamodel_flash_save_load(Controller& c);

#endif