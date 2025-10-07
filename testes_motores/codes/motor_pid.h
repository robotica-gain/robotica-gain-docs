// ==== motor_pid.h ====
#ifndef MOTOR_PID_H
#define MOTOR_PID_H

#include <Ticker.h>

#define PULSOS_POR_VOLTA 11
#define REDUCAO 75
#define PWM_FREQ 7000
#define PWM_RESOLUCAO 12
//#define PWM_MAX 4095
#define PWM_MAX 4000
#define STBY 7

// ==== Definição dos pinos dos motores ====
#define PWM_A 4
#define IN1_A 6
#define IN2_A 5
#define ENC_A1 11
#define ENC_A2 14

#define PWM_B 17
#define IN1_B 15
#define IN2_B 16
#define ENC_B1 41
#define ENC_B2 42

// ==== Ganhos PID dos motores ====
#define KP_A 0.5
#define KI_A 0.6
#define KD_A 0.0

#define KP_B 0.5
#define KI_B 0.6
#define KD_B 0.0

class MotorPID {
  public:

    String nome;           // Nome do motor (D ou E)
    int pwm_pin;           // Pino de saída PWM
    int in1_pin, in2_pin;  // Pinos de direção
    int encoderA_pin, encoderB_pin; // Pinos do encoder
    int canal_pwm;         // Canal PWM exclusivo do motor

    volatile long pulsos = 0;  // Contagem de pulsos do encoder
    volatile int direcao = 1;  // Direção do movimento

    float setpoint_rpm = 0;    // RPM desejado
    float erro_acumulado = 0;  // Integral do erro
    float erro_anterior = 0;   // Erro anterior para cálculo do derivativo

    float Kp = 1.0;            // Ganho proporcional
    float Ki = 0.0;            // Ganho integral
    float Kd = 0.0;            // Ganho derivativo

    // Construtor inclui o canal PWM exclusivo do motor
    MotorPID(String nomeMotor, int pwm, int in1, int in2, int encA, int encB, int canal)
      : nome(nomeMotor), pwm_pin(pwm), in1_pin(in1), in2_pin(in2),
        encoderA_pin(encA), encoderB_pin(encB), canal_pwm(canal) {}

    void setGanhoPID(float kp, float ki, float kd) {
      Kp = kp;
      Ki = ki;
      Kd = kd;
    }

    void iniciar(void (*isr)()) {
      pinMode(in1_pin, OUTPUT);
      pinMode(in2_pin, OUTPUT);
      pinMode(encoderA_pin, INPUT_PULLUP);
      pinMode(encoderB_pin, INPUT_PULLUP);

      ledcAttachChannel(pwm_pin, PWM_FREQ, PWM_RESOLUCAO, canal_pwm);
      attachInterrupt(digitalPinToInterrupt(encoderA_pin), isr, RISING);
    }

    void atualizarControle() {
      if (setpoint_rpm == 0) {
        // Desabilita o motor
       // digitalWrite(STBY, LOW);
        // Garante que os pinos de direção estejam em estado seguro
        digitalWrite(in1_pin, LOW);
        digitalWrite(in2_pin, LOW);
        // Zera PWM
        ledcWrite(pwm_pin, 0);

        // Serial.print("Motor ");
        // Serial.print(nome);
        // Serial.println(" desligado (setpoint = 0 RPM)");
        pulsos = 0;
        erro_acumulado = 0;
        return; // Sai da função
      }
      digitalWrite(STBY, HIGH);
      noInterrupts();
      long pulsos_atual = pulsos;
      pulsos = 0;
      interrupts();

      float rpm = (pulsos_atual / (float)PULSOS_POR_VOLTA) * (60.0 / 0.1) / REDUCAO;

      float erro = setpoint_rpm - rpm;
      float delta = erro - erro_anterior;

      float P = Kp * erro;
      float D = Kd * delta;
      float controle_pre = P + D + Ki * erro_acumulado;

      int pwm_teste = map(abs(controle_pre), 0, 100, 0, PWM_MAX);
      bool saturado = pwm_teste >= PWM_MAX;

      if (!saturado) {
        erro_acumulado += erro;
      }

      erro_anterior = erro;

      float controle = Kp * erro + Ki * erro_acumulado + Kd * delta;
      int pwm = constrain(map(abs(controle), 0, 100, 0, PWM_MAX), 0, PWM_MAX);

      if (controle > 0) {
        digitalWrite(in2_pin, LOW);
        digitalWrite(in1_pin, HIGH);
      } else if (controle < 0) {
        digitalWrite(in2_pin, HIGH);
        digitalWrite(in1_pin, LOW);
      } else {
        digitalWrite(in1_pin, LOW);
        digitalWrite(in2_pin, LOW);
      }

      ledcWrite(pwm_pin, pwm);
/*
      Serial.print("Motor ");
      Serial.print(nome);
      Serial.print(" | Setpoint: ");
      Serial.print(setpoint_rpm);
      Serial.print(" RPM | Medido: ");
      Serial.print(rpm);
      Serial.print(" RPM | PWM: ");
      Serial.println(pwm); */
    }
};


// ==== Instâncias dos motores com canais PWM diferentes ====
MotorPID motorA("D", PWM_A, IN1_A, IN2_A, ENC_A1, ENC_A2, 0); // Motor A -> canal 0
MotorPID motorB("E", PWM_B, IN1_B, IN2_B, ENC_B1, ENC_B2, 1); // Motor B -> canal 1

void IRAM_ATTR isrEncoderA() {
  bool sinalA = digitalRead(motorA.encoderA_pin);
  bool sinalB = digitalRead(motorA.encoderB_pin);
  if (sinalA == sinalB) {
    motorA.pulsos++;
    motorA.direcao = 1;
  } else {
    motorA.pulsos--;
    motorA.direcao = -1;
  }
}

void IRAM_ATTR isrEncoderB() {
  bool sinalA = digitalRead(motorB.encoderA_pin);
  bool sinalB = digitalRead(motorB.encoderB_pin);
  if (sinalA == sinalB) {
    motorB.pulsos++;
    motorB.direcao = 1;
  } else {
    motorB.pulsos--;
    motorB.direcao = -1;
  }
}

// ==== Inicialização completa dos motores ====
Ticker controleTicker;

void inicializarMotores() {
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);

  motorA.iniciar(isrEncoderA);
  motorB.iniciar(isrEncoderB);

  motorA.setGanhoPID(KP_A, KI_A, KD_A);
  motorB.setGanhoPID(KP_B, KI_B, KD_B);

  controleTicker.attach_ms(100, []() {
    motorA.atualizarControle();
    motorB.atualizarControle();
  });
}

// ==== Interface de velocidade ====
inline void setVelocidade(String motor, int porcentagem) {
  porcentagem = constrain(porcentagem, -100, 100);
  if (motor == "D") motorA.setpoint_rpm = -porcentagem;
  if (motor == "E") motorB.setpoint_rpm = -porcentagem;
}

#endif