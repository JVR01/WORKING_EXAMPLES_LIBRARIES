#ifndef __DS1307_H
#define __DS1307_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define addr01 0x68

extern char buf[];
extern char *week[];

int ds1307SetTime();
void ds1307ReadTime();

#endif

