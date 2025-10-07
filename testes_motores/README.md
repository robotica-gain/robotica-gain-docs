# Motores do robô e testes

## A biblioteca "motores_pid.h"
A biblioteca "motores_pid.h" nos permite ter um controle dos motores do robô conectados a placa de aprendizado atual de forma menos verbosa e mais simples.

Durante os testes mais básicos dessa biblioteca foi identificado uma sugestão de alteração em relação ao seu conteúdo, que mantém duas responsabilidades distintas (Controle dos motores e o método PID) em uma mesma classe de forma desorganizada.

Uma sujestão de próxima tarefa seria refatorar esse código a fim de economizar memória, por exemplo em códigos que eu nao fosse usar o método PID e queira apenas o controle dos motores, refatorando também para deixar o código mais legível e com maior facilidade de manutenção futura

## Testes para definição dos ângulos de giro
Antes de começarmos a realizar as atividades propostas (tragetória quadrada e triangular), fizemos testes para definição do tempo de espera necessário para o robô realizar uma curva de 90º em 100% de velocidade.

Com os testes obtemos o tempo de espera médio de 730ms para completar 90º, o que nos aproxima de 81,1ms para cada grau de angulação.

## Métodos auxiliares para implementação do código
```cpp
void frente(int velocidade = 100){
    setVelocidade("D", velocidade);
    setVelocidade("E", velocidade);
}
void tras(int velocidade = 100){
    setVelocidade("D", -velocidade);
    setVelocidade("E", -velocidade);
}
void direita(int velocidade = 100){
    setVelocidade("D", -velocidade);
    setVelocidade("E", velocidade);
}
void esquerda(int velocidade = 100){
    setVelocidade("D", velocidade);
    setVelocidade("E", -velocidade);
}
void parar(){
    setVelocidade("D", 0);
    setVelocidade("E", 0);
}

```

## Tragetória quadrada
Para realização da tragetória quadrada, espera-se que o robô ande para frente por um tempo definido e faça uma curva de 90º e assim sucessivamente.

Logo a nossa estrutura de código consistiu em importar a biblioteca "motores_pid.h", em seguida utilizamos o método para inicialização dos motores disponível na própria biblioteca dentro do setup, e definimos a tragetoria quadrada e angulo de giro dentro do loop():
```arduino
#include "motores_pid.h"

void setup() {
    inicializarMotores();
}

void loop() {
    frente();
    delay(2000); // Tempo arbitrário
    parar();
    direita();
    delay(730); // Tempo médio obtido nos testes para um giro de 90º
    parar();
}
```

## Tragetória quadrada
Para realização da tragetória quadrada, espera-se que o robô ande para frente por um tempo definido e faça uma curva de 120º e assim sucessivamente.

A estrutura segue a mesma da tragetória quadrada:
```arduino
#include "motores_pid.h"

void setup() {
    inicializarMotores();
}

void loop() {
    frente();
    delay(2000); // Tempo arbitrário
    parar();
    direita();
    delay(973); // Tempo calculado para um angulo de 120º
    parar();
}
```

# Controle remoto do Robô

## Blynk
O blynk é um software que nos permite fazer uma conexão do smartphone ou afins com o hardware do ESP, mantendo o servidor deles como intermediário, o que pode acarretar em um leve tempo de resposta.

Ele tem a característica de ser intuitivo e de fácil entendimento, sendo uma opção viável para automações residenciais, controle de irrigação, controle remoto de aparelhos, etc.

Sua utilização em código depende apenas da importação da biblioteca deles e um modelo já criado no site ou app do Blynk.

## Implementação
A implementação em código segue a estrutura básica de código de hardware do tipo ESP32 e Arduino, mas tem sessões chaves básicas como leitura de "Pinos virtuais", que são "pinos" configuráveis pelo Site/App do Blynk, com base na leitura desses pinos virtuais, movemos o robô para trás, frente, esquerda, etc. Podemos também, com um controle deslizante, controlar a velocidade de movimento do robô
