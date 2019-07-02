#ifndef DATAMODEL_H
#define DATAMODEL_H

#include "mbed.h"
#include "params.h"

struct DataModel {
    DataModel();

    char    dozator;
    float   volume;     // ml
    float   feedrate;   // ml/min
    float   accel;      // ml/min*sec
    float   reverse;    // ml
    float   gear_A;     // ml to impulses
    float   gear_B;     // ml to impulses
    float   ratio_A;    // ratio A to all
    float   ratio_B;    // ratio B to all


    FlashIAP flash;
    uint32_t flash_start;
    uint32_t page_size;
    uint32_t field_size;

    void flash_init();
    void flash_load();
    void flash_save();
};

#endif // DATAMODEL_H