#ifndef _THERMOCOUPLE_
#define _THERMOCOUPLE_

#include <stdint.h>
#include "Arduino.h"
#include <SPI.h>

#define THERMOCOUPLE_CS 14

void init_thermocouple(void);
uint32_t read_raw_thermocouple(void);
float read_thermocouple(void);

#endif
