#ifndef IR_SENSORS_H
#define IR_SENSORS_H

#include <Arduino.h>

class SensorsIR {
    public:
        SensorsIR(uint8_t sensors_ir_pins[], uint8_t led_ir_pin, int readings[]);
        void readSensorsIR();
        int rawReadingSensorsIR(uint8_t pin);
        void debugSensorsIR();
    private:
        uint8_t _pins[7];
        uint8_t _led;
        int* _readings;
};

#endif
