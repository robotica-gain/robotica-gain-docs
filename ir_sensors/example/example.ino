/*
================================
| CODE EXAMPLE FOR THE LIBRARY |
================================
*/

#include "ir_sensors.h"

uint8_t pins[7] = {16, 17, 18, 8, 3, 9, 10};
int readings[7];

SensorsIR sensors(pins, 15, readings);

void setup(){
    Serial.begin(115200);
};

void loop(){
    sensors.readSensorsIR();
    // change array readings with new sensors readingsa

    delay(100);
};