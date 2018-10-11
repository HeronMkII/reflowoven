#include <stdint.h>
#include "Arduino.h"

#define THERMOCOUPLE_CS 14

void init_thermocouple(void);
uint32_t read_raw_thermocouple(void);
float read_thermocouple(void);
