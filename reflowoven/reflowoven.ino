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
#include "r"
#include "rgb_lcd.h"

//Reflow Steps
#define Preheat "PREHEAT"
#define Soak "SOAK"
#define ReflowZone "REFLOW"
#define ReflowZoneRise "REFLOW_RIZE"
#define ReflowZoneConst "REFLOW_CONST"
#define Cooling "COOL"

rgb_lcd lcd;

struct temp_profile {
    uint16_t    temps[4];
    uint16_t    times[5];
};

// Setup the parameters for the reflow curve
struct temp_profile reflow_curve;
reflow_curve.temps = [150, 180, 280, 0];
reflow_curve.times = [90, 180, 290, 340,380];

//Define Variables we'll be connecting to

String reflowStep;
bool on=false;
long int  = 90;
long int soakTime = 180;
long int reflowRiseTime = 290;
long int ReflowZoneConstTime = 340;
long int coolingTime = 380;

//Specify the links and initial tuning parameters

int WindowSize = 5000;
unsigned long windowStartTime;

void setup() {
<<<<<<< HEAD
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);

  //initialize the variables we're linked to
//  setpoint = 0;

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  windowStartTime = millis()/1000;
  startTimeProgram= millis()/1000;
  reflow_step=0; //step 0 is Preheating

  delay(500);

  run_oven();

///look at lcd code -- start, stop, reset

=======
    Serial.begin(9600);
    pinMode(RELAY_PIN, OUTPUT);
    init_keypad();
    init_thermocouple();
    
    // Sets up the number of cols (16) and rows (2) that the LCD can use for display.
    lcd.begin(16,2);
    // Start SPI for the thermocouple.
    // Prints out the initialization message on the LCD.
    SPI.begin();
    lcd.print("The LCD is now");
    lcd.setCursor(0,2);
    lcd.print("active");
    delay(2000);
    lcd.clear();

    //initialize the variables we're linked to.
    Setpoint = 0;
    lcd.setRGB(0, 0, 255);
    lcd.print("Initializing");
    lcd.setCursor(0,2);
    lcd.print("variables");
    delay(2000);
    lcd.clear();
    
    //tell the PID to range between 0 and the full window size
    myPID.SetOutputLimits(0, WindowSize);
    lcd.setRGB(0, 0, 255);
    lcd.print("Setting Window");
    delay(2000);
    lcd.clear();
    
    //turn the PID on
    myPID.SetMode(AUTOMATIC);
    windowStartTime = millis()/1000;
    startTimeProgram= millis()/1000;
    reflowStep=Preheat; //step 1 is Preheating
    delay(500);
    run_oven();
>>>>>>> bf677c018a0a649b91dfd17695083aed6b383e73
}

void run_oven(){
    double read_temp, output, setpoint;
    unsigned long now;
    uint8_t running=0;
    uint8_t reflow_step = 0;
    // Set up the PID

    PID oven_pid(&read_temp, &output, &setpoint, 2, 5, 1, DIRECT);
    oven_pid.SetTunings(50,0,0);

    if (digitalRead(SWITCH)){
      if (running==0){
        running = 1;
        lcd.write('ON'); //placeholder
      }
      else {
        running=0;
        lcd.write('OFF');
        START_TIME = startTimeProgram;
        digitalWrite(RelayPin, LOW);
      }
    }
// Record the start time
    uint32_t START_TIME = millis() / 1000;
    uint32_t CURR_TIME;

    CURR_TIME = startTimeProgram -  START_TIME;

    while(running) {
        read_temp = thermocouple.readCelsius()
//this is current time, if greater than next looking for...
        if (reflow_step==0) {
          lcd.write('PREHEAT'); // placeholder
          setpoint = reflow_curve.temps[reflow_step]
          if (CURR_TIME >= reflow_curves.times[0]) {
            reflow_step += 1;
          }
        }
        else if (reflow_step==1) {
          lcd.write('Soak'); // placeholder
          setpoint = reflow_curve.temps[reflow_step]
          //pid??
          if (CURR_TIME >= reflow_curves.times[1]) {
            reflow_step += 1;
          }
        }
        else if (reflow_step==2) {
          lcd.write('Reflow Zone Rise'); // placeholder
          setpoint = reflow_curve.temps[reflow_step]
          //pid??
          if (CURR_TIME >= reflow_curves.times[2]) {
            reflow_step += 1;
          }
        }
        else if (reflow_step==3) {
          lcd.write('Reflow Zone Const'); // placeholder
          setpoint = reflow_curve.temps[reflow_step]
          //pid??
          if (CURR_TIME >= reflow_curves.times[3]) {
            reflow_step += 1;
          }
        }
        else if (reflow_step==4) {
          lcd.write('Cooling'); // placeholder
          setpoint = reflow_curve.temps[reflow_step]
          //pid??
          if (isnan(read_temp)&&read_temp<=28.0) {
            running = 0;
            lcd.write('OFF-something wrong'); //placeholder
          }
        }
        if (digitalRead(SWITCH)==0) {
          running=0;
          lcd.write('OFF'); //placeholder
        }

//        read_temp = thermocouple.readCelsius();

<<<<<<< HEAD
//      if (isnan(c)) {
//          Serial.println("Something wrong with thermocouple!");
//      } else {
        Serial.println(read_temp);
  //    }
=======
  if (reflowStep==Preheat){
    Setpoint=preHeatSetPoint;
    myPID.SetTunings(50,0,0);

    if (display_>=preheatTime){
      reflowStep=Soak;
      }
    }
>>>>>>> bf677c018a0a649b91dfd17695083aed6b383e73

        now = millis()/1000 - WindowStartTime;
        //shift the relay window??
        if (now > WindowSize) {
          windowStartTime += WindowSize;
        }

        if (Output > now) {
          digitalWrite(RelayPin, HIGH);
        }
        else {
          digitalWrite(RelayPin, LOW);
        }
        delay(1000);
    }
}
