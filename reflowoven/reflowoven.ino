#include <PID_v1.h>
#include <Adafruit_MAX31855.h>
#include <stdint.h>
#include "keypad.c"
#include "constants.h"
//Reflow Steps
#define Preheat "PREHEAT"
#define Soak "SOAK"
#define ReflowZone "REFLOW"
#define ReflowZoneRise "REFLOW_RIZE"
#define ReflowZoneConst "REFLOW_CONST"
#define Cooling "COOL"

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

// initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

//Specify the links and initial tuning parameters


int WindowSize = 5000;
unsigned long windowStartTime;

void setup() {
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

//      if (isnan(c)) {
//          Serial.println("Something wrong with thermocouple!");
//      } else {
        Serial.println(read_temp);
  //    }

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
