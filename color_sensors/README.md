# Color Sensors

## Introdução
A biblioteca para a placa dos sensores de cor foi desenvolvida para facilitar a leitura dos sensores de cor presentes na placa. Esses sensores são utilizados para detectar padrões coloridos que indicam mudança no estado atual do robô, sendo essenciais para o funcionamento do robô de competição desenvolvido.

## Inicialização da Biblioteca
Para utilizar a biblioteca, é necessário incluir o arquivo de cabeçalho e criar uma instância da classe `ColorSensors`. A inicialização requer a especificação dos pinos aos quais os sensores estão conectados.

```cpp
#include "color_sensors.h"

String colors[2];

ColorSensors colorSensors(sda1, scl1, sda2, scl2, colors);
```
- `sda1` e `scl1`: pinos do primeiro sensor de cor.
- `sda2` e `scl2`: pinos do segundo sensor de cor.
- `colors`: array de strings que armazenará as cores detectadas pelos sensores.

## Funcionalidades
A placa conta com 2 sensores de cor, que podem ser lidos em conjunto, já passando pela detecção real da cor de acordo com o RGB lido.


Para leitura conjunta de todos os sensores, a função `readColors` atualiza um array de 2 posições com os valores lidos dos sensores.:
```cpp
colorSensors.readColors();
```
- O array `colors` passado na inicialização da classe será atualizado com os valores lidos dos sensores.

A bibloteca também implementa uma função de debug que imprime os valores lidos dos sensores (como o RGB, clear, cor detectada) no monitor serial.:

```cpp
colorSensors.printRawValues();
```
- Esta função é útil para verificar se os sensores estão funcionando corretamente e para calibrar o robô.

## Exemplo
Um exemplo completo de utilização da biblioteca pode ser encontrado no arquivo de exemplo, para ver ele clique [AQUI](example/example.ino).