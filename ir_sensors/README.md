# IR Sensors

## Introdução
Biblioteca para a placa dos sensores infravermelhos utilizada nos robôs da equipe de Robótica GAIN. Esta biblioteca facilita a leitura dos valores dos sensores infravermelhos conectados à placa, permitindo uma integração eficiente com o sistema de controle do robô.

## Inicialização da Biblioteca
Para utilizar a biblioteca, é necessário incluir o arquivo de cabeçalho e criar uma instância da classe `SensorsIR`. A inicialização requer a especificação dos pinos aos quais os sensores estão conectados.

```cpp
#include "ir_sensors.h"

int[7] readings;
int[7] ir_pins = {A0, A1, A2, A3, A4, A5, A6};

SensorsIR irSensors(ir_pins, ledPin, readings);
```
- `ir_pins`: Pinos analógicos conectados aos sensores infravermelhos.
- `ledPin`: Pino digital que controla os LEDs infravermelhos.
- `readings`: Array para armazenar os valores lidos dos sensores.

## Funcionalidades
A placa conta com 7 sensores infravermelhos, que podem ser lidos individualmente ou em conjunto, juntamente com o pino que controla todos os LEDs infravermelhos.


Para leitura conjunta de todos os sensores, a função `readSensorsIR` atualiza um array de 7 posições com os valores lidos dos sensores.:
```cpp
irSensors.readSensorsIR();
```
- O array `readings` passado na inicialização da classe será atualizado com os valores lidos dos sensores.

Para leitura individual de um sensor específico, a função `rawReadingSensorsIR` retorna o valor inteiro da leitura do sensor.:

```cpp
int sensorValue = irSensors.rawReadingSensorsIR(irPin);
```
- `irPin`: Pino do sensor a ser lido.

A bibloteca também implementa uma função de debug que imprime os valores lidos dos sensores no monitor serial.:

```cpp
irSensors.debugSensorsIR();
```
- Esta função é útil para verificar se os sensores estão funcionando corretamente e para calibrar o robô.

## Exemplo
Um exemplo completo de utilização da biblioteca pode ser encontrado no arquivo de exemplo, para ver ele clique [AQUI](example/example.ino).