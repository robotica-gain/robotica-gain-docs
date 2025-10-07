#define BLYNK_TEMPLATE_ID "TMPL2eBZFSIG0"
#define BLYNK_TEMPLATE_NAME "Controle Robô Gain"
#define BLYNK_AUTH_TOKEN "5-a9ZzDqf0O0XVTrCqTSN0-dN9i6-jIQ"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "../motor_pid.h"

char ssid[] = "CelularJV";
char pass[] = "jv28042009";

#define LED 48

int joystickX = 0;
int joystickY = 0;
int buttonV0 = 0; // Frente
int buttonV1 = 0; // Trás
int buttonV2 = 0; // Direita
int buttonV3 = 0; // Esquerda
int sliderV4 = 50;

void setup() {
  Serial.begin(115200);
  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  
  inicializarMotores();
  
  Serial.println("Conectando ao Blynk...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  digitalWrite(LED, HIGH);
  Serial.println("Robô conectado ao Blynk! LED aceso.");
}

void loop() {
  Blynk.run();
  
  if (buttonV0 || buttonV1 || buttonV2 || buttonV3) {
    controlarComBotoes();
  } else {
    controlarComJoystick();
  }
  
  delay(50);
}

void controlarComBotoes() {
  if (buttonV0) frente(sliderV4);
  else if (buttonV1) tras(sliderV4);
  else if (buttonV2) direita(sliderV4);
  else if (buttonV3) esquerda(sliderV4);
  else parar();
}

void controlarComJoystick() {
  // Zona morta
  if (abs(joystickX) < 20 && abs(joystickY) < 20) {
    parar();
    return;
  }
  
  int velocidadeDireita = (-joystickY) - joystickX;
  int velocidadeEsquerda = (-joystickY) + joystickX;
  
  // Aplicar limite de velocidade
  velocidadeDireita = map(velocidadeDireita, -255, 255, -sliderV4, sliderV4);
  velocidadeEsquerda = map(velocidadeEsquerda, -255, 255, -sliderV4, sliderV4);
  
  velocidadeDireita = constrain(velocidadeDireita, -100, 100);
  velocidadeEsquerda = constrain(velocidadeEsquerda, -100, 100);
  
  setVelocidade("D", velocidadeDireita);
  setVelocidade("E", velocidadeEsquerda);
}

void frente(int velocidade) {
  setVelocidade("D", velocidade);
  setVelocidade("E", velocidade);
}

void tras(int velocidade) {
  setVelocidade("D", -velocidade);
  setVelocidade("E", -velocidade);
}

void direita(int velocidade) {
  setVelocidade("D", -velocidade);
  setVelocidade("E", velocidade);
}

void esquerda(int velocidade) {
  setVelocidade("D", velocidade);
  setVelocidade("E", -velocidade);
}

void parar() {
  setVelocidade("D", 0);
  setVelocidade("E", 0);
}


BLYNK_WRITE(V0) { buttonV0 = param.asInt(); }
BLYNK_WRITE(V1) { buttonV1 = param.asInt(); }
BLYNK_WRITE(V2) { buttonV2 = param.asInt(); }
BLYNK_WRITE(V3) { buttonV3 = param.asInt(); }

BLYNK_WRITE(V4) {
  sliderV4 = param.asInt();
  if (sliderV4 < 10) sliderV4 = 10;
}

BLYNK_WRITE(V5) { joystickX = param.asInt(); }
BLYNK_WRITE(V6) { joystickY = param.asInt(); }