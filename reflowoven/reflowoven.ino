///*
//    DESCRIPTION: Reflow oven control software
//    AUTHORS: Ben, Mahnoor, Rebecca, Dylan, Andy
//    Contains functions and logic for controlling UTAT Space System's reflow oven
//
//    TODO:
//    -   refactor all the code
//
//    *New library rgb_lcd.h is refered to as lcd. Some functions you will need to know
//    *to work with this new library:
//
//    lcd.print() prints the message on the board.
//    lcd.setCursor() moves the message to a new line. This is to prevent the message from going over. Set to (0,2) for new line.
//    lcd.setRGB(r,g,b): set (255,0,0) for red, (0,255,0) for green, (0,0,255) for blue.
//    lcd.clear() to clear the screen.
//*/
//orange 5V
//pin2 blue - if high on if low off

#include <stdint.h>
#include <Wire.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <math.h>

#include "Arduino.h"
#include "PID_v1.h"
#include "constants.h"
#include "keypad.h"
#include "rgb_lcd.h"
#include "thermocouple.h"
#include "DFRobot_MAX31855.h"

// Seems to take 150 seconds to reach 150

reflow_curve_t sn63_soldering = {
  "SN63 SOLDER",
  {"PREHEAT", "SOAK", "REFLOW", "COOLING"},
  .temps = {150,180,250,0},
  .times = {90,180,240,245},
};

reflow_curve_t microfluidics = {
  "TESTING",
  {"PREHEAT", "SOAK", "REFLOW", "COOLING"},
  .temps = {150,0,0,0},
  .times = {1000,2000,3000,4000},
};

reflow_curve_t monitor_temp = {
  "VIEW TEMP",
  {""},
  .temps = {0},
  .times = {0},
};

reflow_profiles_t reflow_profiles = {
  {sn63_soldering, microfluidics, monitor_temp}
};

long WINDOW_SIZE = 5000;

oven_state_t oven_state = STOPPED;
rgb_lcd lcd;
double read_temp = 0, output, setpoint = 0;
PID oven_pid(&read_temp, &output, &setpoint, 60, 0, 0, DIRECT);

DFRobot_MAX31855 thermocouple;

void setup(void) {
  Serial.begin(9600);
  init_keypad();

  lcd.begin(16,2);            // num cols to 16 and num rows to 2
  lcd.setRGB(255,255,255);    // set colour to white

  // PID SETUP
  setpoint = 0;
  oven_pid.SetOutputLimits(0, WINDOW_SIZE);   // limit outputs
  oven_pid.SetSampleTime(1000);               // set sample time to 1s
  oven_pid.SetMode(AUTOMATIC);                // set mode to automatic

  pinMode(RELAY_PIN, OUTPUT); // set up the relay control switch
  digitalWrite(RELAY_PIN, LOW);

  pinMode(SWITCH, INPUT);     // set up the switch to control the oven

  // calls loop
} // setup

void stop_oven(lcd_state_t* lcd_state){
  digitalWrite(RELAY_PIN, LOW);   // ensure the heaters are turned off
  lcd.clear();

  if (*lcd_state == UPDATE){       // run once to set up for turning back on
    lcd.clear();
    lcd.setRGB(0,0,0);
    lcd.setCursor(1,1);
    *lcd_state = HOLD;
  }
  delay(10);                      // delay for kicks
} // stop_oven

void run_setup(lcd_state_t* lcd_state, int* profile_num){

  if (*lcd_state == UPDATE) {
    lcd.clear();                // clear display
    lcd.setRGB(255,255,255);
    lcd.setCursor(0,0);
    // lcd.print(reflow_profiles.profiles[profile_num].name); // TODO: See if this works
    lcd.print(reflow_profiles.profiles[*profile_num].name);
    //lcd.print("SN63 SOLDER");
    lcd.setCursor(0,1);
    lcd.print("START: 1 NEXT: 3");
    *lcd_state = HOLD;
  }

  int key = scan_keypad();
  if (key == 1) {
    oven_state = RUNNING;       // set oven state to running
  } else if (key == 3) {
    *profile_num = (*profile_num + 1) % NUM_PROFILES; // increment profile
    *lcd_state = UPDATE;
    delay(300);
  }
} // run_setup

void run_oven_cycle(lcd_state_t* lcd_state, int profile_num, int* reflow_step){
  static int error_count = 0;
  static unsigned long windowStart, programStart;
  uint16_t* times = reflow_profiles.profiles[profile_num].times;
  uint16_t* temps = reflow_profiles.profiles[profile_num].temps;

  double current_temp;
  unsigned long now = millis();

  if ((*lcd_state == UPDATE) && (*reflow_step == 0)){
    windowStart = millis();
    programStart = millis()/1000;
  }

  if (*lcd_state == UPDATE){
    lcd.clear();
    lcd.setRGB(255,255,255);
    lcd.setCursor(0,0);
    lcd.print(reflow_profiles.profiles[profile_num].steps[*reflow_step]);
    *lcd_state = HOLD;
  }

  setpoint = temps[*reflow_step];

  current_temp = thermocouple.readCelsius();
  if(!isnan(current_temp)){           // check if the thermocouple returned nan
    read_temp = current_temp;
  } else {
    error_count++;
  }

  Serial.println(read_temp);


  if (error_count > MAX_ERRORS){
    oven_state = ERROR;
    error_count = 0;
    *reflow_step = 0;
    return;
  }

  lcd.setCursor(0,1);
  lcd.print(read_temp);
  lcd.print(" C  ");
  lcd.setCursor(13,1);
  lcd.print(now/1000 - programStart);
  oven_pid.Compute();

  /*
  Serial.print("SETPOINT: ");
  Serial.println(setpoint);
  Serial.print("TIME: ");
  Serial.println(now/1000);
  Serial.print("OUTPUT: ");
  Serial.println(output);
  Serial.print("WINDOW TIME: ");
  Serial.println(now - windowStart);
  Serial.println("\n");
  */

  if (now - windowStart > WINDOW_SIZE) {
    windowStart += WINDOW_SIZE;
  }

  if (output > (now - windowStart)) {
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }

  if (((now/1000) - programStart) > times[*reflow_step]){
    *reflow_step += 1;                          // increment reflow step
    *lcd_state = UPDATE;
    if (*reflow_step == MAX_CURVE_LENGTH){   // check if we've finished
      oven_state = COOLING;
    }
  }
  delay(400);
} // run_oven_cycle

void run_cooling(lcd_state_t* lcd_state){
  digitalWrite(RELAY_PIN, LOW);
  if (*lcd_state == UPDATE){
    lcd.clear();
    lcd.setRGB(255,255,255);
    lcd.setCursor(0,0);
    lcd.print("COOLING");
    *lcd_state = HOLD;
  }
  lcd.setCursor(0,1);
  read_temp = thermocouple.readCelsius();
  lcd.print(read_temp);
  lcd.print(" C  ");
  delay(500);
} // run_cooling

void run_error(lcd_state_t* lcd_state){
  digitalWrite(RELAY_PIN, LOW);
  if (*lcd_state == UPDATE){
    lcd.clear();
    lcd.setRGB(255,0,0);
    lcd.setCursor(0,0);
    lcd.print("ERROR");
    *lcd_state = HOLD;
  }
  lcd.setCursor(0,1);
  lcd.print("TEMP: ");
  read_temp = thermocouple.readCelsius();
  lcd.print(read_temp);
  delay(500);
}

void run_oven(lcd_state_t* lcd_state){
  static int profile_num = 0;
  static int reflow_step = 0;

  switch(oven_state){
    case STOPPED:
      profile_num = 0;
      stop_oven(lcd_state);
      break;
    case SETUP:
      reflow_step = 0;
      run_setup(lcd_state, &profile_num);
      break;
    case RUNNING:
      run_oven_cycle(lcd_state, profile_num, &reflow_step);
      break;
    case COOLING:
      run_cooling(lcd_state);
      break;
    case ERROR:
      run_error(lcd_state);
      break;
  }
} // run_oven

void loop(void){
  static oven_state_t last_state = oven_state;
  static lcd_state_t lcd_state = UPDATE;

  if (!digitalRead(SWITCH)){
    // stop the oven
    oven_state = STOPPED;
  } else {
    // start the oven, or continue running
    if (oven_state == STOPPED){
      oven_state = SETUP;
    }
  }
  if (last_state != oven_state){
    lcd_state = UPDATE;
  }
  last_state = oven_state;
  run_oven(&lcd_state);
  delay(100);
} // loop
