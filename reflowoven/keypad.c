/*
    DESCRIPTION: Keypad interfacing functions
    AUTHORS: Dylan Vogel

    Contains functions for interfacing with this type of keypad:
    https://cdn.sparkfun.com/assets/7/e/f/6/f/sparkfun_keypad.pdf

    USAGE:
    - Call init_keypad to setup the pins associated with each row and column
    - Call scan_keypad() to return a uint8_t corresponding to the position of the key that was pressed
        - scan_keypad() only returns a single value per press, so it doesn't support multi-button presses
        - scan_keypad maps "*" to 10, and "#" to 12.
        - -1 is returned to indicate that nothing was pressed

    TODO:
    -   Test that the library still works with the whole oven
*/
#include "keypad.h"

void init_keypad() {
  // put your setup code here, to run once:
  pinMode(COL1, OUTPUT);
  pinMode(COL2, OUTPUT);
  pinMode(COL3, OUTPUT);
  pinMode(ROW1, INPUT_PULLUP);
  pinMode(ROW2, INPUT_PULLUP);
  pinMode(ROW3, INPUT_PULLUP);
  pinMode(ROW4, INPUT_PULLUP);

  digitalWrite(COL1, HIGH);
  digitalWrite(COL2, HIGH);
  digitalWrite(COL3, HIGH);
}

uint8_t scan_row(int col) {
  int row;
  digitalWrite(col, LOW);
  delay(1);
  if (digitalRead(ROW1) == LOW){
    row = 1;
  } else if (digitalRead(ROW2) == LOW){
    row = 2;
  } else if (digitalRead(ROW3) == LOW){
    row = 3;
  } else if (digitalRead(ROW4) == LOW){
    row = 4;
  } else {
    row = 0;
  }
  digitalWrite(col, HIGH);
  return row;
}

int8_t scan_keypad() {
  int row;
  row = scan_row(COL1);
  if (row != 0){
    switch (row){
      case 1:
        return 1;
      case 2:
        return 4;
      case 3:
        return 7;
      case 4:
        return 10;
    }
  }
  row = scan_row(COL2);
  if (row != 0){
    switch (row){
      case 1:
        return 2;
      case 2:
        return 5;
      case 3:
        return 8;
      case 4:
        return 0;
    }
  }
  row = scan_row(COL3);
  if (row != 0){
    switch (row){
      case 1:
        return 3;
      case 2:
        return 6;
      case 3:
        return 9;
      case 4:
        return 12;
    }
  }
  return -1;
}

/*
void setup() {
  Serial.begin(9600);
  Serial.println("Serial initialized");
  init_keypad();

}
void loop() {
  int key = scan_keypad();

//  digitalWrite(COL1, LOW);
//  Serial.println(digitalRead(ROW1));
//  Serial.println(digitalRead(ROW2));
//  Serial.println(digitalRead(ROW3));
//  Serial.println(digitalRead(ROW4));
//  digitalWrite(COL1, HIGH);
//  Serial.println(digitalRead(ROW1));
//  Serial.println(digitalRead(ROW2));
//  Serial.println(digitalRead(ROW3));
//  Serial.println(digitalRead(ROW4));


  switch (key){
    case -1:
      break;
    case 1:
      Serial.println("1");
      break;
    case 2:
      Serial.println("2");
      break;
    case 3:
      Serial.println("3");
      break;
    case 4:
      Serial.println("4");
      break;
    case 5:
      Serial.println("5");
      break;
    case 6:
      Serial.println("6");
      break;
    case 7:
      Serial.println("7");
      break;
    case 8:
      Serial.println("8");
      break;
    case 9:
      Serial.println("9");
      break;
    case 10:
      Serial.println("*");
      break;
    case 0:
      Serial.println("0");
      break;
    case 12:
      Serial.println("#");
      break;
  }
  delay(100);
}
*/
