/*
    DESCRIPTION: Reflow oven control software
    AUTHORS: Ben, Mahnoor, Rebecca, Dylan, Andy
    Contains functions and logic for controlling UTAT Space System's reflow oven

    TODO:
    -   refactor all the code

    *New library rgb_lcd.h is refered to as lcd. Some functions you will need to know
    *to work with this new library:

    lcd.print() prints the message on the board.
    lcd.setCursor() moves the message to a new line. This is to prevent the message from going over. Set to (0,2) for new line.
    lcd.setRGB(r,g,b): set (255,0,0) for red, (0,255,0) for green, (0,0,255) for blue.
    lcd.clear() to clear the screen.
*/

#include <PID_v1.h>
#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>
#include "constants.h"
#include "keypad.c"
#include "thermocouple.c"
#include "rgb_lcd.h"
#include <stdint.h>
#include <SPI.h>

//Reflow Steps
// #define Preheat "PREHEAT"
// #define Soak "SOAK"
// #define ReflowZone "REFLOW"
// #define ReflowZoneRise "REFLOW_RIZE"
// #define ReflowZoneConst "REFLOW_CONST"
// #define Cooling "COOL"

rgb_lcd lcd;

//struct temp_profile {
//    uint16_t    temps[4];
//    uint16_t    times[5];
//};

// Setup the parameters for the reflow curve



//Define Variables we'll be connecting to

//String reflowStep;
//bool on=false;
// long int  = 90;
// long int soakTime = 180;
// long int reflowRiseTime = 290;
// long int ReflowZoneConstTime = 340;
// long int coolingTime = 380;

//Specify the links and initial tuning parameters

int WindowSize = 5000;
unsigned long windowStartTime;

void setup() {
    int key,startTimeProgram;
//    struct temp_profile reflow_curve;
    Serial.begin(9600);
    pinMode(RELAY_PIN, OUTPUT);
    int temps[4] = {150,180,280,0};
    int times[5] = {90,180,290,340,380};
//    reflow_curve.temps = {150, 180, 280, 0};
//    reflow_curve.times = {90, 180, 290, 340, 380};
    init_keypad();
    //init_thermocouple();
//
//    // Sets up the number of cols (16) and rows (2) that the LCD can use for display.
    lcd.begin(16,2);
//    // Start SPI for the thermocouple.
//    // Prints out the initialization message on the LCD.
    SPI.begin();
    lcd.print("The LCD is now");
    lcd.setCursor(0,2);
    lcd.print("active");
    delay(2000);
    lcd.clear();

//    // //initialize the variables we're linked to.
//    // Setpoint = 0;
//    // lcd.setRGB(0, 0, 255);  // set colour to blue
//    // lcd.print("Initializing");
//    // lcd.setCursor(0,2);
//    // lcd.print("variables");
//    // delay(2000);
//    // lcd.clear();
//
//    //tell the PID to range between 0 and the full window size
//    myPID.SetOutputLimits(0, WindowSize);
    lcd.setRGB(0, 0, 255);  // set colour of RGB to blue
    lcd.print("Setting Window");
    delay(2000);
    lcd.clear();

//    //selection options: start, stop, reset, power off
    lcd.print("Press 1 to Start");
    key = scan_keypad();
    delay(2000);
    lcd.clear();
    lcd.print("Press 2 to Turn off");
    key = scan_keypad();
    delay(2000);
    lcd.clear();

////    lcd.print("Press 3 to Stop");
////    key = scan_keypad();
//
    while (key != 1 and key != 2) {
      if (key == 1) {   //if 1 is selected: run oven
//        //turn the PID on
//        myPID.SetMode(AUTOMATIC);
        windowStartTime = millis()/1000;
        startTimeProgram= millis()/1000;
//        reflow_step=0; //step 0 is Preheating
        delay(500);
        lcd.print("Starting");
        run_oven();
      }
//  //    else if (key == 2) {
//        //***
//  //    }
      else {
        lcd.print("Invalid Input, select again");
        key = scan_keypad();
        delay(2000);
      }
    }
}
//
////void loop(){
//  //nothing
////}
//
void run_oven(){
    double read_temp, output, setpoint,startTimeProgram;
    uint32_t START_TIME;
    unsigned long now;
    int key;
    startTimeProgram = millis()/1000;
    uint8_t running=0;
    uint8_t reflow_step = 0;
    int temps[4] = {150,180,280,0};
    int times[5] = {90,180,290,340,380};
    // Set up the PID
//
//  ///  PID oven_pid(&read_temp, &output, &setpoint, 2, 5, 1, DIRECT);
//  ///  oven_pid.SetTunings(50,0,0);
//
    if (digitalRead(SWITCH)){
      if (running==0){
        running = 1;
//    ///    lcd.print('ON');
//    ///    delay(2000);
//    ///    lcd.clear();
      }
      else {
        running=0;
//     ///   lcd.print('OFF');
//     ///   delay(2000);
//     ///   lcd.clear();
        START_TIME = startTimeProgram;
        digitalWrite(RELAY_PIN, LOW);
      }
    }
//// Record the start time
    START_TIME = millis() / 1000;
    uint32_t CURR_TIME;

    CURR_TIME = startTimeProgram -  START_TIME;
//
    while(running) {
//  /*      lcd.print("Press 3 to Stop");
//        key = scan_keypad();
//        delay(2000);
//        lcd.clear();
//        if (key == 3) { // or switch(key)  case 3
//          //Stop
//          running = 0;
//          lcd.print('Turning off');
//          delay(2000);
//          lcd.clear();
//        }
//  */
   ///     read_temp = readCelsius()
//this is current time, if greater than next looking for...
        if (reflow_step==0) {
          lcd.print("PREHEAT");
          delay(2000);
          lcd.clear();

          setpoint = temps[reflow_step];
 //         setpoint = reflow_curve.temps[reflow_step]
          if (CURR_TIME >= times[0]) {
            reflow_step += 1;
          }
        }
        else if (reflow_step==1) {
          lcd.print("Soak");
          delay(2000);
          lcd.clear();
          setpoint = temps[reflow_step];
//          //pid??
          if (CURR_TIME >= times[1]) {
            reflow_step += 1;
          }
        }
        else if (reflow_step==2) {
          lcd.print("Reflow Zone Rise");
          delay(2000);
          lcd.clear();
          setpoint = temps[reflow_step];
          //pid??
          if (CURR_TIME >= times[2]) {
            reflow_step += 1;
          }
        }
        else if (reflow_step==3) {
          lcd.print("Reflow Zone Const");
          delay(2000);
          lcd.clear();
          setpoint = temps[reflow_step];
          //pid??
          if (CURR_TIME >= times[3]) {
            reflow_step += 1;
          }
        }
        else if (reflow_step==4) {
          lcd.print("Cooling");
          delay(2000);
          lcd.clear();
          setpoint = temps[reflow_step];
//          //pid??
          if (isnan(read_temp)&&read_temp<=28.0) {
            running = 0;
            lcd.print("OFF-something wrong");
            delay(2000);
            lcd.clear();
          }
        }
        if (digitalRead(SWITCH)==0) {
          running=0;
          lcd.print("OFF");
          delay(2000);
          lcd.clear();
        }

////        read_temp = readCelsius();

////      if (isnan(c)) {
////          Serial.println("Something wrong with thermocouple!");
////      } else {
        Serial.println(read_temp);
        lcd.print(read_temp);
        delay(1000);
        lcd.clear();
////      }

        now = millis()/1000 - windowStartTime;
//        //shift the relay window??
        if (now > WindowSize) {
          windowStartTime += WindowSize;
        }

        if (output > now) {
          digitalWrite(RELAY_PIN, HIGH);
        }
        else {
          digitalWrite(RELAY_PIN, LOW);
        }
        delay(1000);
    }
}
