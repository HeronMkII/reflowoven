#ifndef _KEYPAD_
#define _KEYPAD_

#include <stdint.h>
#include <HardwareSerial.h>
#include "Arduino.h"

#define COL1 6
#define COL2 8
#define COL3 4
#define ROW1 7
#define ROW2 2
#define ROW3 3
#define ROW4 5

void init_keypad(void);
uint8_t scan_row(int col);
int8_t scan_keypad(void);
void test_keypad(void);

#endif
