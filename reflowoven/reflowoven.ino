/*
    DESCRIPTION: Reflow oven control software
    AUTHORS: Ben, Mahnoor, Rebecca, Dylan

    Contains functions and logic for controlling UTAT Space System's reflow oven

    TODO:
    -   refactor all the code
*/


#include <PID_v1.h>
#include <stdint.h>
#include "constants.h"
#include "keypad.c"
#include "thermocouple.c"
//Reflow Steps
#define Preheat "PREHEAT"
#define Soak "SOAK"
#define ReflowZone "REFLOW"
#define ReflowZoneRise "REFLOW_RIZE"
#define ReflowZoneConst "REFLOW_CONST"
#define Cooling "COOL"

struct temp_profile {
    uint16_t    temps[4];
    uint16_t    times[4];
};

// Setup the parameters for the reflow curve
struct temp_profile reflow_curve;
reflow_curve.temps = [150, 150, 250, 0];
reflow_curve.times = [90, 180, 290, 340];

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
    Serial.begin(9600);
    pinMode(RELAY_PIN, OUTPUT);
    init_keypad();
    init_thermocouple();
    // eventually
    //init_lcd();

    // Start SPI for the thermocouple
    SPI.begin();

  //initialize the variables we're linked to
  Setpoint = 0;

  //tell the PID to range between 0 and the full window size
  myPID.SetOutputLimits(0, WindowSize);

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  windowStartTime = millis()/1000;
  startTimeProgram= millis()/1000;
  reflowStep=Preheat; //step 1 is Preheating

  delay(500);

  run_oven();

}

void run_oven(){
    double read_temp, output, setpoint;
    uint8_t running = 1;
    uint8_t reflow_step = 0;
    // Set up the PID

    PID oven_pid(&read_temp, &output, &setpoint, 2, 5, 1, DIRECT);
    oven_pid.SetTunings(50,0,0);

// Record the start time
    uint32_t START_TIME = millis() / 1000;
    uint32_t CURR_TIME;
    // preheat
    lcd.write('PREHEAT'); // placeholder
    while(running) {
//this is current time, if greater than next looking for...
        if () {

            reflow_step += 1;
            setpoint =reflow_curve.temps[reflow_step]
        }


        read_temp = thermocouple.readCelsius()
        oven_pid.compute()

        if () {

        }
    }
;


}


  if (reflowStep==Preheat){
    Setpoint=preHeatSetPoint;
    myPID.SetTunings(50,0,0);

    if (display_>=preheatTime){
      reflowStep=Soak;
      }
    }

  else if (reflowStep==Soak){
    Setpoint=soakSetPoint;
    if (display_>=soakTime)
      reflowStep=ReflowZoneRise;
    }

   //stages we will test afterwards
  else if (reflowStep==ReflowZoneRise){
    Setpoint=reflowSetPoint;
    myPID.SetTunings(50,0,0);
    if (display_>=reflowRiseTime){
      reflowStep=ReflowZoneConst;
    }
  }
  else if(reflowStep==ReflowZoneConst){
  Setpoint = reflowSetPoint;
  myPID.SetTunings(50,0,0);
    if (display_>= ReflowZoneConstTime){
      reflowStep=Cooling;
    }
  }
  else if (reflowStep==Cooling){
    Setpoint=cooling;
 if (isnan(c)&&c<=28.0) {
    on=false;

   }
    }
   double c = thermocouple.readCelsius();
   Serial.print(reflowStep);
 //Serial.print(", startTimeProgram:");
  // Serial.print(startTimeProgram);
  // Serial.print(", startTimeRReflow:");
//   Serial.print(startTimeReflow);
   Serial.print(", displayTime:");
   Serial.print(display_);

   Serial.print(", ");
   if (isnan(c)) {
     Serial.println("Something wrong with thermocouple!");
   } else {
     Serial.println(c);
   }

  unsigned long now = millis()/1000;
  if (now - windowStartTime > WindowSize)
  { //time to shift the Relay Window
    windowStartTime += WindowSize;
  }

  if (Output > now - windowStartTime) {
    digitalWrite(RelayPin, HIGH);
    }
  else {
    digitalWrite(RelayPin, LOW);
  }
   delay(1000);
  }
  else {
    Serial.println("NOT ON BITCH");
    on=false;
    startTimeReflow=startTimeProgram;
     digitalWrite(RelayPin, LOW);
    }
}
