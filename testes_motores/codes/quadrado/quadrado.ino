#include "../motor_pid.h"

void setup(){
  inicializarMotores();
}

void loop(){
  frente();
  delay(2000);
  parar();
  direita();
  delay(730);
  parar();
}

void frente(){
  setVelocidade("D", 100);
  setVelocidade("E", 100);
}

void direita(){
  setVelocidade("E", -100);
  setVelocidade("D", 100);
}

void parar(){
  setVelocidade("D", 0);
  setVelocidade("E", 0);
}
