#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "pio_led.pio.h"
#include "hardware/timer.h"

const uint OUT_PIN = 7, PIN_BTN_A = 5, PIN_BTN_B = 6,PIN_LED = 13;
const uint MATRIX_DIMENSION = 5;

// Apelida o tipo de dado "uinsigned char" para "uchar"
typedef unsigned char uchar;

uint32_t LAST_EVENT_A = 0, LAST_EVENT_B = 0;

uint32_t CURRENT_INDEX_DRAW = 0;

// Matriz com todos os desenhos dos números
const float draws[10][5][5] = {

    // Desenhos que serão usados nos padrões de leds
    // Número 0
    {{0.7, 0.7, 0.7, 0.7, 0.7},
     {0.7, 0.0, 0.0, 0.0, 0.7},
     {0.7, 0.0, 0.0, 0.0, 0.7},
     {0.7, 0.0, 0.0, 0.0, 0.7},
     {0.7, 0.7, 0.7, 0.7, 0.7}},

    // Número 1
    {{0.0, 0.0, 0.7, 0.0, 0.0},
     {0.0, 0.7, 0.7, 0.0, 0.0},
     {0.7, 0.0, 0.7, 0.0, 0.0},
     {0.0, 0.0, 0.7, 0.0, 0.0},
     {0.0, 0.7, 0.7, 0.7, 0.0}},

    // Número 2
    {{0.7, 0.7, 0.7, 0.7, 0.7},
     {0.0, 0.0, 0.0, 0.0, 0.7},
     {0.7, 0.7, 0.7, 0.7, 0.7},
     {0.7, 0.0, 0.0, 0.0, 0.0},
     {0.7, 0.7, 0.7, 0.7, 0.7}},

    // Número 3
    {{0.7, 0.7, 0.7, 0.7, 0.7},
     {0.0, 0.0, 0.0, 0.0, 0.7},
     {0.7, 0.7, 0.7, 0.7, 0.7},
     {0.0, 0.0, 0.0, 0.0, 0.7},
     {0.7, 0.7, 0.7, 0.7, 0.7}},

    // Número 4
    {{0.7, 0.0, 0.0, 0.0, 0.7},
     {0.7, 0.0, 0.0, 0.0, 0.7},
     {0.7, 0.7, 0.7, 0.7, 0.7},
     {0.0, 0.0, 0.0, 0.0, 0.7},
     {0.0, 0.0, 0.0, 0.0, 0.7}},

    // Número 5
    {{0.7, 0.7, 0.7, 0.7, 0.7},
     {0.7, 0.0, 0.0, 0.0, 0.0},
     {0.7, 0.7, 0.7, 0.7, 0.7},
     {0.0, 0.0, 0.0, 0.0, 0.7},
     {0.7, 0.7, 0.7, 0.7, 0.7}},

    // Número 6
    {{0.7, 0.7, 0.7, 0.7, 0.7},
     {0.7, 0.0, 0.0, 0.0, 0.0},
     {0.7, 0.7, 0.7, 0.7, 0.7},
     {0.7, 0.0, 0.0, 0.0, 0.7},
     {0.7, 0.7, 0.7, 0.7, 0.7}},

    // Número 7
    {{0.7, 0.7, 0.7, 0.7, 0.7},
     {0.0, 0.0, 0.0, 0.7, 0.0},
     {0.0, 0.0, 0.7, 0.0, 0.0},
     {0.0, 0.7, 0.0, 0.0, 0.0},
     {0.7, 0.0, 0.0, 0.0, 0.0}},

    // Número 8
    {{0.7, 0.7, 0.7, 0.7, 0.7},
     {0.7, 0.0, 0.0, 0.0, 0.7},
     {0.7, 0.7, 0.7, 0.7, 0.7},
     {0.7, 0.0, 0.0, 0.0, 0.7},
     {0.7, 0.7, 0.7, 0.7, 0.7}},

    // Número 9
    {{0.7, 0.7, 0.7, 0.7, 0.7},
     {0.7, 0.0, 0.0, 0.0, 0.7},
     {0.7, 0.7, 0.7, 0.7, 0.7},
     {0.0, 0.0, 0.0, 0.0, 0.7},
     {0.0, 0.0, 0.0, 0.0, 0.7}},
    ///////////////////////////////////////////////////////////////////
};

PIO pio;
uint offset;
uint sm;

// Função que retorna a cor rgb em formato de um inteiro de 32 bits
uint32_t rgbColor(float r, float g, float b)
{
    // Trata os valores recebidos nos parâmetros da função para que não ultrapassem o valor 1 e também não fiquem abaixo do valor 0
    uchar R = (r > 1 || r < 0 ? 0.5 : r) * 255;
    uchar G = (g > 1 || g < 0 ? 0.5 : g) * 255;
    uchar B = (b > 1 || b < 0 ? 0.5 : b) * 255;

    return (R << 24) | (G << 16) | (B << 8);
}

void showNumberDraw(PIO *pio, uint *sm, uint32_t *drawIndex);

void gpio_irq_handler(uint gpio, uint32_t events);
int main()
{

    stdio_init_all();

    // Inicio da configuração inicial do firmaware
    pio = pio0;
    set_sys_clock_khz(128000, false);

    gpio_init(PIN_BTN_A);
    gpio_init(PIN_BTN_B);
    gpio_init(PIN_LED);

    gpio_set_dir(PIN_BTN_A, GPIO_IN);
    gpio_set_dir(PIN_BTN_B, GPIO_IN);
    gpio_set_dir(PIN_LED,GPIO_OUT);

    gpio_pull_up(PIN_BTN_A);
    gpio_pull_up(PIN_BTN_B);

    offset = pio_add_program(pio, &pio_led_program);
    sm = pio_claim_unused_sm(pio, true);
    pio_led_program_init(pio, sm, offset, OUT_PIN);
    //Habilita a interrupção no GPIO
    gpio_set_irq_enabled_with_callback(PIN_BTN_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(PIN_BTN_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    // Fim da configuração inicial do firmaware

    // Chama a função para desenhar o primeiro número
    showNumberDraw(&pio, &sm, &CURRENT_INDEX_DRAW);

    while (true)
    {
        gpio_put(PIN_LED,1);
        sleep_ms(100);
        gpio_put(PIN_LED,0);
        sleep_ms(100);
    }

    return 0;
}
void showNumberDraw(PIO *pio, uint *sm, uint32_t *drawIndex)
{
    // Liga os Leds no padrão do desenho representado na matriz
    for (int j = 0; j < MATRIX_DIMENSION; j++)
    {
        for (int k = 0; k < MATRIX_DIMENSION; k++)
        {
            // Envia as informações de cor e intensidade do led para a maquina de estado
            pio_sm_put_blocking(*pio, *sm, rgbColor(draws[*drawIndex][MATRIX_DIMENSION - 1 - j][(j + 1) % 2 == 0 ? k : MATRIX_DIMENSION - k - 1], 0.0, 0.0));
        }
    }
}
void gpio_irq_handler(uint gpio, uint32_t events)
{

    uint32_t current_time = to_us_since_boot(get_absolute_time());
    
    if (gpio == PIN_BTN_A)
    {
        // Condição para fazer o deboucing do botão A
        if (current_time - LAST_EVENT_A > 200000)
        {
            LAST_EVENT_A = current_time;
            if(CURRENT_INDEX_DRAW < 9){
                CURRENT_INDEX_DRAW++;
            }
        }
    }

    else if(gpio == PIN_BTN_B){
        // Condição para fazer o deboucing do botão B
        if (current_time - LAST_EVENT_B > 200000)
        {
            LAST_EVENT_B = current_time;
            if(CURRENT_INDEX_DRAW > 0 ){
                CURRENT_INDEX_DRAW--;
            }
        }
    }
    
    showNumberDraw(&pio, &sm, &CURRENT_INDEX_DRAW);
}      