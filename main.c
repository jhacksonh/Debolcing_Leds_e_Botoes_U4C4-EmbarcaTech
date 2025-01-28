#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "ws2812.pio.h"

//Config Pinos
#define PIN_LED_R 13
#define PIN_LED_G 11
#define PIN_BUTTON_A 5
#define PIN_LEDS 7 // Pino de dados para o WS2812

#define LED_COUNT 25
PIO pio = pio0;
uint sm = 0; // Primeira state machine

#define TIME_PIN_LED_R (1000/5)/2
uint32_t last_time = 0;
uint counter = 0;

uint8_t ledCenterON[3] = {12};
uint8_t colorsLedCenterON[3][3] = {{0, 10, 0}};

void ConfigPins(uint pin, bool InorOut, bool pullUP);

void Debouncing(uint pin_button);
static void gpio_irq_handler(uint gpio, uint32_t events);

void ws2812_init(uint pin);
void ws2812_send_colors(uint8_t colors[LED_COUNT][3]);
void Map_leds_ON(uint8_t *LedsOn, uint8_t colorsOn[][3], int LedsOnCount);
void Clear_leds();

int main(){
    stdio_init_all();
    ConfigPins(PIN_LED_R, GPIO_OUT, false);
    ConfigPins(PIN_LED_G, GPIO_OUT, false);
    Debouncing(PIN_BUTTON_A);
    ws2812_init(PIN_LEDS);

    while(1){
        gpio_put(PIN_LED_R, 1);
        sleep_ms(TIME_PIN_LED_R);
        gpio_put(PIN_LED_R, 0);
        sleep_ms(TIME_PIN_LED_R);
    }
    return 0;
}

void ConfigPins(uint pin, bool InorOut, bool pullUP){
    gpio_init(pin);
    gpio_set_dir(pin, InorOut);
    if(!pullUP){
        gpio_pull_up(pin);
    }else{
        gpio_put(pin, 0);
    }
}

//Funçoes Trabalhando com Debouncing
void Debouncing(uint pin_button){
    ConfigPins(PIN_BUTTON_A, GPIO_IN, false);
    gpio_set_irq_enabled_with_callback(pin_button, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
}
void gpio_irq_handler(uint gpio, uint32_t events){
    uint32_t current_time = to_us_since_boot(get_absolute_time());// pega o tempo atual em microssegundos
    // Verifica se passou tempo suficiente desde o último evento
    if (current_time - last_time > 200000){ // 200ms de debouncing
        last_time = current_time; // Atualiza o tempo do último evento
        // gpio_put(PIN_LED_G, !gpio_get(PIN_LED_G));
        Map_leds_ON(ledCenterON, colorsLedCenterON, sizeof(ledCenterON)/sizeof(ledCenterON[0]));
    }
}

// Função para inicializar o WS2812
void ws2812_init(uint pin) {
    // Carrega o programa PIO para o WS2812 na memória PIO
    uint offset = pio_add_program(pio, &ws2812_program);
    // Configura o estado da máquina (state machine) para rodar o programa
    ws2812_program_init(pio, sm, offset, pin, 800000, false);
    // Habilita o WS2812 na mAquina PIO
    pio_sm_set_enabled(pio, sm, true);
    Clear_leds();
}
//função para receber um array tridimencional[LED_COUNT][3] contendo as cores dos leds
void ws2812_send_colors(uint8_t colors[LED_COUNT][3]) {
    for (int i = 0; i < LED_COUNT; i++) {
        uint32_t color = (colors[i][0] << 16) | (colors[i][1] << 8) | colors[i][2];
        pio_sm_put_blocking(pio, sm, color);
    }
}
void Map_leds_ON(uint8_t *LedsOn, uint8_t colorsOn[][3], int LedsOnCount){
    uint8_t colors[LED_COUNT][3] = {0};
    for (uint8_t i = 0; i < LedsOnCount; i++) {
        colors[LedsOn[i]][0] = colorsOn[i][0];
        colors[LedsOn[i]][1] = colorsOn[i][1];
        colors[LedsOn[i]][2] = colorsOn[i][2];
    }
    ws2812_send_colors(colors);
}
void Clear_leds(){
    uint8_t colors[LED_COUNT][3] = {0};
    ws2812_send_colors(colors);
}

