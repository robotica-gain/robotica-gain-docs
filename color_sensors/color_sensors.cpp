#include "color_sensors.h"

ColorSensors::ColorSensors(
    uint8_t sda1, 
    uint8_t scl1, 
    uint8_t sda2, 
    uint8_t scl2,
    String colors[]
): _sda1(sda1), _scl1(scl1), _sda2(sda2), _scl2(scl2), _colors(colors) {};

bool ColorSensors::begin() {
    bool success_status = true;

    Wire.begin(_sda1, _scl1, 400000);
    delay(100);
    
    Wire1.begin(_sda2, _scl2, 400000);
    delay(100);
    
    if (_sensor1.begin(TCS34725_ADDRESS, &Wire)) {
        _sensorActive[0] = true;
        delay(50);
        _sensor1.setGain(TCS34725_GAIN_4X);
        _sensor1.setIntegrationTime(TCS34725_INTEGRATIONTIME_154MS);
        _sensor1.enable();
    } else {
        _sensorActive[0] = false;
        success_status = false;
    };
    
    delay(100);
    
    if (_sensor2.begin(TCS34725_ADDRESS, &Wire1)) {
        _sensorActive[1] = true;
        delay(50);
        _sensor2.setGain(TCS34725_GAIN_4X);
        _sensor2.setIntegrationTime(TCS34725_INTEGRATIONTIME_154MS);
        _sensor2.enable();
    } else {
        _sensorActive[1] = false;
        success_status = false;
    }

    return success_status;
};

bool ColorSensors::readSensor(uint8_t sensorIndex, uint16_t &r, uint16_t &g, uint16_t &b, uint16_t &c) {
    if (sensorIndex >= NUM_SENSORS || !_sensorActive[sensorIndex]) {
        return false;
    }
    
    if (sensorIndex == 0) {
        _sensor1.getRawData(&r, &g, &b, &c);
    } else {
        _sensor2.getRawData(&r, &g, &b, &c);
    }
    
    return true;
};

String ColorSensors::detectColor(uint16_t r, uint16_t g, uint16_t b, uint16_t c) {
    if (c > _whiteThreshold) {
        return "W";
    }

    if (c < _blackThreshold) {
        return "B";
    }
    
    if (c >= _silverMinClear && c <= _silverMaxClear) {
        float maxRGB = max(max(r, g), b);
        float minRGB = min(min(r, g), b);

        if (maxRGB > 0 && (maxRGB - minRGB) / maxRGB < 0.3) {
            return "S";
        }
    }
    
    if (r > g && r > b) {
        if (g > 0 && (float)r / g > _redGreenRatio) {
            return "R";
        }
    }
    
    if (g > r && g > b) {
        if (b > 0 && (float)g / b > _greenBlueRatio) {
            return "G";
        }
    }
    
    if (r > g && r > b) return "R";
    if (g > r && g > b) return "G";
    
    return "W";
};

void ColorSensors::readColors() {
    uint16_t r[2], g[2], b[2], c[2];

    _colors[0] = "W";
    _colors[1] = "W";
    
    if (!_sensorActive[0] || !_sensorActive[1]) {
        return;
    }
    
    _sensor1.getRawData(&r[0], &g[0], &b[0], &c[0]);
    _sensor2.getRawData(&r[1], &g[1], &b[1], &c[1]);
    
    _colors[0] = detectColor(r[0], g[0], b[0], c[0]);
    _colors[1] = detectColor(r[1], g[1], b[1], c[1]);
};

void ColorSensors::printRawValues() {
    uint16_t r[2], g[2], b[2], c[2];
    
    if (_sensorActive[0]) {
        _sensor1.getRawData(&r[0], &g[0], &b[0], &c[0]);
        Serial.println("Color Sensor 1:");
        Serial.printf("  R:%d G:%d B:%d Clear:%d\n", r[0], g[0], b[0], c[0]);
        Serial.printf("  Cor detectada: %s\n", detectColor(r[0], g[0], b[0], c[0]).c_str());
    }
    
    if (_sensorActive[1]) {
        _sensor2.getRawData(&r[1], &g[1], &b[1], &c[1]);
        Serial.println("Color Sensor 2:");
        Serial.printf("  R:%d G:%d B:%d Clear:%d\n", r[1], g[1], b[1], c[1]);
        Serial.printf("  Cor detectada: %s\n", detectColor(r[1], g[1], b[1], c[1]).c_str());
    }
    
    Serial.println("===================================\n");
};

void ColorSensors::setCalibration(
    uint16_t whiteThreshold,
    uint16_t blackThreshold,
    float redGreenRatio,
    float greenBlueRatio,
    uint16_t silverMinClear,
    uint16_t silverMaxClear
) {
    _whiteThreshold = whiteThreshold;
    _blackThreshold = blackThreshold;
    _redGreenRatio = redGreenRatio;
    _greenBlueRatio = greenBlueRatio;
    _silverMinClear = silverMinClear;
    _silverMaxClear = silverMaxClear;
};