#ifndef __DS3231_H
#define __DS3231_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define addr_clock 0x68

extern char buf[];
extern char *week[];

int ds3231SetTime();
void ds3231ReadTime();

#endif

