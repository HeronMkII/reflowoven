///*
//    DESCRIPTION: MAX31855 Functions
//    AUTHORS: Dylan Vogel
//
//    Contains funcitons for interfacing with the MAX31855 K-thermocouple measurement IC.
//
//    TODO:
//    -   Test the code
//    -   Implement temperature compensation based on reference junction temp
//*/
//
//
//#include <SPI.h>
//#include <stdint.h>
////#include "constants.h"
//#include "thermocouple.h"
//#include "Arduino.h"
//
//void setup(){
//    SPI.begin();
//    pinMode(THERMOCOUPLE_CS, OUTPUT);
//    digitalWrite(THERMOCOUPLE_CS, HIGH);
//}
//
//uint32_t read_raw_thermocouple(){
//    uint32_t data = 0;
//    digitalWrite(THERMOCOUPLE_CS, LOW);
//    for (uint8_t i = 3; i >= 0; i--){
//       data |= SPI.transfer(0x00) << (8 * i);
//    }
//    digitalWrite(THERMOCOUPLE_CS, HIGH);
//    return data;
//}
//
//float read_thermocouple(){
//    uint32_t raw_data = read_raw_thermocouple();
//    uint16_t thm_temp, rj_temp;
//    int16_t sign = 1;
//    float temperature;
//    // bit 16 is fault bit
//    if ((raw_data >> 16) & 0x01){
//        // Return -251 if a fault was detected, to indicate that the reading is invalid
//        // Also add the fault bits from the end to indicate what kind of fault was detected
//        // -252 == OC (no thermocouple)
//        // -253 == SG (shorted to ground)
//        // -254 == SCV (shorted to VCC)
//        return (-251.0 - (raw_data & 0x03));
//    }
//    // bits 31:18 are the thermocouple temperature
//    thm_temp = (uint16_t)(raw_data >> 18);
//    rj_temp = (uint16_t)((raw_data >> 4) & 0x0FFF);
//
//    // Negative number
//    if ((thm_temp >> 13) & 0x01){
//        // Set sign to negative
//        sign = -1;
//        // Invert temp, add 1
//        thm_temp = (~thm_temp) + 1;
//    }
//    // The first 11 bits are the temp, last 2 bits are decimal
//    temperature = ((thm_temp >> 2) & 0x07FF) + (0.25 * (thm_temp & 0x0003));
//    temperature *= sign;
//
//    return temperature;
//
//}
//
//void loop(){
//  //nothing
//}

