#include <stdio.h>
#include "pico/stdlib.h"
#include "ws2812.pio.h"
#include "leds.h"
#include <string.h>

#define MAX_LEDS 100
PIO pio = pio0;
uint sm = 0; // Primeira state machine
static int LED_COUNT;
uint8_t colors[MAX_LEDS][3] = {0};

static void rgb_to_grb(uint8_t colors[MAX_LEDS][3]) {
    uint32_t grb[LED_COUNT];
    for (int i = 0; i < LED_COUNT; i++) {
        uint8_t r = colors[i][0];
        uint8_t g = colors[i][1];
        uint8_t b = colors[i][2];
        grb[i] = (g << 16) | (r << 8) | b;   
        sleep_us(1);
        pio_sm_put_blocking(pio, sm, grb[i] << 8u);
    }
}
// Função para inicializar o WS2812
void ws2812_init(uint pin, int len_leds){
    LED_COUNT = len_leds;
    // Carrega o programa PIO para o WS2812 na memória PIO
    uint offset = pio_add_program(pio, &ws2812_program);
    // Configura o estado da máquina (state machine) para rodar o programa
    ws2812_program_init(pio, sm, offset, pin, 800000, false);
    // Habilita o WS2812 na mAquina PIO
    pio_sm_set_enabled(pio, sm, true);
    Clear_leds();
    sleep_us(100);
}
//função para receber um array tridimencional[LED_COUNT][3] contendo as cores dos leds
void Map_leds_ON(uint8_t *LedsOn, uint8_t colorsOn[][3], int LedsOnCount, bool clear_cache){
    if (clear_cache){
        Clear_leds();
    }
    for (uint8_t i = 0; i < LedsOnCount; i++) {
        colors[LedsOn[i]][0] = colorsOn[i][0];
        colors[LedsOn[i]][1] = colorsOn[i][1];
        colors[LedsOn[i]][2] = colorsOn[i][2];
    }
    rgb_to_grb(colors);
}
void Clear_leds(){
    memset(colors, 0, sizeof(colors));
}