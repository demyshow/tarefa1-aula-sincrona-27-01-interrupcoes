# Controle de Matriz de LEDs WS2812 com Interrupções

## 1. Introdução
Este projeto foi implementado e testado por Edemir de Carvalho Rodrigues, utilizando o microcontrolador RP2040 e a placa BitDogLab. O sistema controla uma matriz de LEDs WS2812 e um LED RGB com interrupções e debounce, garantindo uma resposta eficiente aos botões. Os testes foram realizados tanto no simulador Wokwi no VS Code quanto na placa RP2040 física para validar sua funcionalidade.

## 2. Objetivos
- Aplicar interrupções e debounce em microcontroladores;
- Controlar LEDs comuns e LEDs WS2812;
- Utilizar resistores de pull-up internos em botões físicos;
- Criar um sistema funcional e interativo.

## 3. Demonstração
[Vídeo demonstrativo](https://drive.google.com/file/d/103cKUlCGyTDEn5tWDFDRoPzKEX31FT8b/view?usp=drive_link)

## 4. Funcionalidades
- O LED vermelho do LED RGB pisca 5 vezes por segundo;
- Botão A incrementa o número exibido na matriz de LEDs apenas quando pressionado;
- Botão B decrementa o número exibido na matriz apenas quando pressionado;
- A matriz WS2812 exibe números de 0 a 9 com efeitos visuais.

## 5. Implementação Técnica
- **Hardware:** RP2040, LED RGB, matriz WS2812, botões físicos;
- **Software:** Código estruturado com interrupções, debounce e controle de LEDs;
- **Fluxo:** Inicialização, captura de eventos dos botões e atualização da matriz.

## 6. Autor
- **Nome:** Edemir de Carvalho Rodrigues
- **Matrícula:** TIC370100576
