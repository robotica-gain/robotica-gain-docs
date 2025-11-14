/*
================================
| CODE EXAMPLE FOR THE LIBRARY |
================================
*/

#include "color_sensors.h"

String colors[2];
ColorSensors sensors(8, 9, 10, 11, colors);

void setup(void) {
  sensors.begin();
  Serial.begin(115200);
}

void loop(void) {
  sensors.readColors();

  for (int i = 0; i < 2; i++) {
    Serial.printf("Color %i: %s\n", i, colors[i]);
  };
  Serial.println("=========================");

  delay(100);
}
