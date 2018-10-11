#include <stdint.h>
#include "constants.h"
#include "Arduino.h"

#define COL1 11
#define COL2 12
#define COL3 13
#define ROW1 7
#define ROW2 8
#define ROW3 9
#define ROW4 10

void init_keypad(void);
uint8_t scan_row(int col);
int8_t scan_keypad(void);
