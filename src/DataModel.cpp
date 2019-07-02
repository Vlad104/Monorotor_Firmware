#include "DataModel.h"

DataModel::DataModel() :
    dozator('2'),
    volume(0.0),
    feedrate(0.0),
    accel(0.0),
    reverse(0.0),
    gear_A(1.0),
    gear_B(1.0),
    ratio_A(0.5),
    ratio_B(0.5),
    flash_start(FLASH_START_ADR),
    page_size(FLASH_PAGE_SIZES),
    // field_size(FLASH_FIELD_SIZE)
    field_size(4)
{
    flash_init();
}

void DataModel::flash_init() {
    flash.init();
    flash_load();
    flash_save();
}

void DataModel::flash_load() {
    flash.read(&dozator,    flash_start + field_size * 0, field_size);
    flash.read(&volume,     flash_start + field_size * 1, field_size);
    flash.read(&feedrate,   flash_start + field_size * 2, field_size);
    flash.read(&accel,      flash_start + field_size * 3, field_size);
    flash.read(&reverse,    flash_start + field_size * 4, field_size);
    flash.read(&gear_A,     flash_start + field_size * 5, field_size);
    flash.read(&gear_B,     flash_start + field_size * 6, field_size);
    flash.read(&ratio_A,    flash_start + field_size * 7, field_size);
    flash.read(&ratio_B,    flash_start + field_size * 8, field_size);
}

void DataModel::flash_save() {
    flash.erase(flash_start, page_size);
    flash.program(&dozator,    flash_start + field_size * 0, field_size);
    flash.program(&volume,     flash_start + field_size * 1, field_size);
    flash.program(&feedrate,   flash_start + field_size * 2, field_size);
    flash.program(&accel,      flash_start + field_size * 3, field_size);
    flash.program(&reverse,    flash_start + field_size * 4, field_size);
    flash.program(&gear_A,     flash_start + field_size * 5, field_size);
    flash.program(&gear_B,     flash_start + field_size * 6, field_size);
    flash.program(&ratio_A,    flash_start + field_size * 7, field_size);
    flash.program(&ratio_B,    flash_start + field_size * 8, field_size);
}