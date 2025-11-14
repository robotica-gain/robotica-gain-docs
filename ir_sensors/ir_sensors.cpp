#include "ir_sensors.h"

SensorsIR::SensorsIR(
    uint8_t sensors_ir_pins[], 
    uint8_t led_ir_pin,
    int readings[]
): _led(led_ir_pin), _readings(readings) {

    for (int i = 0; i < 7; i++){
        _pins[i] = sensors_ir_pins[i];
        pinMode(_pins[i], INPUT);
    };
    pinMode(_led, OUTPUT);
};

void SensorsIR::readSensorsIR() {
    for (int i = 0; i < 7; i++) {
        _readings[i] = rawReadingSensorsIR(_pins[i]);
    };
};

int SensorsIR::rawReadingSensorsIR(uint8_t pin){
    const int numReadings = 5;
    int sumReadings = 0;
    int readIR, readAmb;

    for (int i = 0; i < numReadings; i++) {
        digitalWrite(_led, HIGH);
        delayMicroseconds(200);
        readIR = analogRead(pin);

        digitalWrite(_led, LOW);
        delayMicroseconds(200);
        readAmb = analogRead(pin);

        sumReadings += (readAmb - readIR);
    };

    return sumReadings / numReadings;
};

void SensorsIR::debugSensorsIR() {
    for (int i = 0; i < 7; i++) {
        Serial.print(String(_readings[i]) + " ");
    };
    Serial.println("");
};
