#ifndef COLOR_SENSORS_H
#define COLOR_SENSORS_H

#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

class ColorSensors {
    public:
        ColorSensors(uint8_t sda1, uint8_t scl1, uint8_t sda2, uint8_t scl2, String colors[]);
        
        bool begin();
        
        void readColors();
        
        void printRawValues();
        void setCalibration(
            uint16_t whiteThreshold,
            uint16_t blackThreshold,
            float redGreenRatio,
            float greenBlueRatio,
            uint16_t silverMinClear,
            uint16_t silverMaxClear
        );

    private:
        static const uint8_t NUM_SENSORS = 2;
        
        uint8_t _sda1, _scl1;
        uint8_t _sda2, _scl2;
        String _colors[2];
        bool _sensorActive[NUM_SENSORS];
        
        Adafruit_TCS34725 _sensor1;
        Adafruit_TCS34725 _sensor2;
        
        // Calibração
        uint16_t _whiteThreshold = 10000;  // Clear > este valor = Branco
        uint16_t _blackThreshold = 500;    // Clear < este valor = Preto
        float _redGreenRatio = 1.3;        // R/G > este valor = Vermelho
        float _greenBlueRatio = 1.2;       // G/B > este valor = Verde
        uint16_t _silverMinClear = 3000;   // Limites para detectar Prateado
        uint16_t _silverMaxClear = 9000;
        
        String detectColor(uint16_t r, uint16_t g, uint16_t b, uint16_t c);
        bool readSensor(uint8_t sensorIndex, uint16_t &r, uint16_t &g, uint16_t &b, uint16_t &c);
};

#endif