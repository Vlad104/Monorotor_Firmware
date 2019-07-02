#include "tests.h"

#ifdef TESTS

int tests(Controller& c) {
    run_tests(c);
    return 0;
}

// ДОБАВИТЬ ТЕСТ НА ОКРУГЛЕНИЕ РЕВЕРСА И ТЕСТ НА ЕГО РАСЧЕТ
void run_tests(Controller& c) {
    Serial* port = c.usb_;
    port->printf("\r\nSTART TESTING...\r\n");
    port->printf("Controler: str_to_float:\t\t %s\r\n",           test_controler_str_to_float(c)          ? "PASS" : "FAILED");
    port->printf("Dozator: set_volume:\t\t %s\r\n",               test_dozator_set_volume(c)              ? "PASS" : "FAILED");
    port->printf("MultiDozator: calculate_volume:\t %s\r\n",      test_multidozator_calculate_volume(c)   ? "PASS" : "FAILED");
    port->printf("MultiDozator: calculate_feedrate:\t %s\r\n",    test_multidozator_calculate_feedrate(c) ? "PASS" : "FAILED");
    port->printf("MultiDozator: calculate_accel:\t\t %s\r\n",     test_multidozator_calculate_accel(c)    ? "PASS" : "FAILED");
    port->printf("MultiDozator: calculate_ratio:\t\t %s\r\n",     test_multidozator_calculate_ratio(c)    ? "PASS" : "FAILED");
    port->printf("MultiDozator: calculate_gear:\t\t %s\r\n",      test_multidozator_calculate_gear(c)     ? "PASS" : "FAILED");
    port->printf("MultiDozator: calculate_all:\t\t %s\r\n",       test_multidozator_calculate_all(c)      ? "PASS" : "FAILED");
    port->printf("DataModel: flash_save_load:\t\t %s\r\n",        test_datamodel_flash_save_load(c)       ? "PASS" : "FAILED");
}

bool test_controler_str_to_float(Controller& c) {
    bool flag = true;
    const float eps = 0.001;

    Serial* port = c.usb_;

    const char str_buffer1[] = "1234567.999999";
    float temp1 = c.str_to_float(str_buffer1);
    if (std::abs(temp1 - 1234567.999999) > eps) {
        flag = false;
        port->printf("EXPECTED: %f\t ACTUAL: %f\r\n", 1234567.999999, temp1);
    }

    const char str_buffer2[] = "-12398.22";
    float temp2 = c.str_to_float(str_buffer2);
    if (std::abs(temp2 + 12398.22) > eps) {
        flag = false;
        port->printf("EXPECTED: %f\t ACTUAL: %f\r\n", -12398.22, temp2);
    }

    const char str_buffer3[] = "9000000";
    float temp3 = c.str_to_float(str_buffer3);
    if (std::abs(temp3 - 9000000) > eps) {
        flag = false;
        port->printf("EXPECTED: %f\t ACTUAL: %f\r\n", 9000000, temp3);
    }

    const char str_buffer4[] = "0";
    float temp4 = c.str_to_float(str_buffer4);
    if (std::abs(temp4 - 0) > eps) {
        flag = false;
        port->printf("EXPECTED: %f\t ACTUAL: %f\r\n", 0, temp4);
    }

    const char str_buffer5[] = "10AA10";
    float temp5 = c.str_to_float(str_buffer5);
    if (std::abs(temp5 - 10) > eps) {
        flag = false;
        port->printf("EXPECTED: %f\t ACTUAL: %f\r\n", 10, temp5);
    }

    const char str_buffer6[] = "12334.39r";
    float temp6 = c.str_to_float(str_buffer6);
    if (std::abs(temp6 - 12334.39) > eps) {
        flag = false;
        port->printf("EXPECTED: %f\t ACTUAL: %f\r\n", 12334.3339, temp6);
    }

    return flag;
}

bool test_dozator_set_volume(Controller& c) {
    bool flag = true;
    Serial* port = c.usb_;

    c.dozators_.dozator_A_->set_volume(123456789.1234);
    if (c.dozators_.dozator_A_->volume_ != 123456789) {
        flag = false;
        port->printf("EXPECTED: %ld\t ACTUAL: %ld\r\n", 123456789, c.dozators_.dozator_A_->volume_);
    }

    c.dozators_.dozator_A_->set_volume(1234567891.0);
    if (c.dozators_.dozator_A_->volume_ != 1234567891) {
        flag = false;
        port->printf("EXPECTED: %ld\t ACTUAL: %ld\r\n", 1234567891, c.dozators_.dozator_A_->volume_);
    }

    c.dozators_.dozator_A_->set_volume(7892.8);
    if (c.dozators_.dozator_A_->volume_ != 7893) {
        flag = false;
        port->printf("EXPECTED: %ld\t ACTUAL: %ld\r\n", 7893, c.dozators_.dozator_A_->volume_);
    }

    c.dozators_.dozator_A_->set_volume(0.5);
    if (c.dozators_.dozator_A_->volume_ != 1) {
        flag = false;
        port->printf("EXPECTED: %ld\t ACTUAL: %ld\r\n", 1, c.dozators_.dozator_A_->volume_);
    }

    c.dozators_.dozator_A_->set_volume(-1999.2);
    if (c.dozators_.dozator_A_->volume_ != -1999) {
        flag = false;
        port->printf("EXPECTED: %ld\t ACTUAL: %ld\r\n", -1999, c.dozators_.dozator_A_->volume_);
    }

    c.dozators_.dozator_A_->set_volume(-1999.9);
    if (c.dozators_.dozator_A_->volume_ != -2000) {
        flag = false;
        port->printf("EXPECTED: %ld\t ACTUAL: %ld\r\n", -2000, c.dozators_.dozator_A_->volume_);
    }

    c.dozators_.dozator_A_->set_volume(12345678.1234);
    if (c.dozators_.dozator_A_->volume_ != 12345678) {
        flag = false;
        port->printf("EXPECTED: %ld\t ACTUAL: %ld\r\n", 12345678, c.dozators_.dozator_A_->volume_);
    }

    c.dozators_.dozator_A_->set_volume(1000000000.0);
    if (c.dozators_.dozator_A_->volume_ != 1000000000) {
        flag = false;
        port->printf("EXPECTED: %ld\t ACTUAL: %ld\r\n", 1000000000, c.dozators_.dozator_A_->volume_);
    }

    return flag;
}

bool test_multidozator_calculate_volume(Controller& c) {
    bool flag = true;
    flag = false;
    return flag;
}

bool test_multidozator_calculate_feedrate(Controller& c) {
    bool flag = true;
    flag = false;
    return flag;
}

bool test_multidozator_calculate_accel(Controller& c) {
    bool flag = true;
    flag = false;
    return flag;
}

bool test_multidozator_calculate_ratio(Controller& c) {
    bool flag = true;
    flag = false;
    return flag;
}

bool test_multidozator_calculate_gear(Controller& c) {
    bool flag = true;
    flag = false;
    return flag;
}

bool test_multidozator_calculate_all(Controller& c) {
    bool flag = true;
    flag = false;
    return flag;
}

bool test_datamodel_flash_save_load(Controller& c) {
    bool flag = true;
    float eps = 0.001;

    c.dozators_.data_.dozator = '1';
    c.dozators_.data_.volume = 100;
    c.dozators_.data_.feedrate = 200.7;
    c.dozators_.data_.accel = 1000.2;
    c.dozators_.data_.reverse = 0.02;
    c.dozators_.data_.gear_A = 2050.123456;
    c.dozators_.data_.gear_B = 2050.123457;
    c.dozators_.data_.ratio_A = 0.49999;
    c.dozators_.data_.ratio_B = 0.51111;
    c.dozators_.data_.flash_save();

    c.dozators_.data_.dozator = '2';
    c.dozators_.data_.volume = 0;
    c.dozators_.data_.feedrate = 0.0;
    c.dozators_.data_.accel = 0.0;
    c.dozators_.data_.reverse = 0;
    c.dozators_.data_.gear_A = 1;
    c.dozators_.data_.gear_B = 1;
    c.dozators_.data_.ratio_A = 0.5;
    c.dozators_.data_.ratio_B = 0.5;
    c.dozators_.data_.flash_load();

    Serial* port = c.usb_;
    if (c.dozators_.data_.dozator != '1') {
        flag = false;
        port->printf("D: EXPECTED: %c\t ACTUAL: %c\r\n", '1', c.dozators_.data_.dozator);
    }
    if (std::abs(c.dozators_.data_.volume - 100) > eps) {
        flag = false;
        port->printf("V: EXPECTED: %f\t ACTUAL: %f\r\n", 100, c.dozators_.data_.volume);
    }
    if (std::abs(c.dozators_.data_.feedrate - 200.7) > eps) {
        flag = false;
        port->printf("F: EXPECTED: %f\t ACTUAL: %f\r\n", 200.7, c.dozators_.data_.feedrate);
    }
    if (std::abs(c.dozators_.data_.accel - 1000.2) > eps) {
        flag = false;
        port->printf("A: EXPECTED: %f\t ACTUAL: %f\r\n", 1000.2, c.dozators_.data_.accel);
    }
    if (std::abs(c.dozators_.data_.reverse - 0.02) > eps) {
        flag = false;
        port->printf("R: EXPECTED: %f\t ACTUAL: %f\r\n", 0.02, c.dozators_.data_.reverse);
    }
    if (std::abs(c.dozators_.data_.gear_A - 2050.123456) > eps) {
        flag = false;
        port->printf("Wa: EXPECTED: %f\t ACTUAL: %f\r\n", 2050.123456, c.dozators_.data_.gear_A);
    }
    if (std::abs(c.dozators_.data_.gear_B - 2050.123457) > eps) {
        flag = false;
        port->printf("Wb: EXPECTED: %f\t ACTUAL: %f\r\n", 2050.123457, c.dozators_.data_.gear_B);
    }
    if (std::abs(c.dozators_.data_.ratio_A - 0.49999) > eps) {
        flag = false;
        port->printf("a: EXPECTED: %f\t ACTUAL: %f\r\n", 0.49999, c.dozators_.data_.ratio_A);
    }
    if (std::abs(c.dozators_.data_.ratio_B - 0.51111) > eps) {
        flag = false;
        port->printf("b: EXPECTED: %f\t ACTUAL: %f\r\n", 0.51111, c.dozators_.data_.ratio_B);
    }

    return flag;
}

#endif