#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "ws2812.pio.h"
#include "leds.h"

//Config Pinos
#define PIN_LED_R 13
#define PIN_LED_G 11
#define PIN_BUTTON_A 5
#define PIN_BUTTON_B 6
#define PIN_LEDS 7 // Pino de dados para o WS2812

#define LED_COUNT 25


// PIO pio = pio0;
// uint sm = 0; // Primeira state machine

#define TIME_PIN_LED_R (1000/5)/2
static uint32_t last_time = 0;
static int index = 0;
/*
24,23,22,21,20
15,16,17,18,19
14,13,12,11,10
05,06,07,08,09
04,03,02,01,00
*/
uint8_t framesNums0_9[11][15] = {
    {1,2,3,5,9,10,14,15,19,21,22,23}, //frame número 0
    {0,1,2,3,4,7,12,15,16,17,22}, //frame número 1
    {0,1,2,3,4,5,13,12,11,19,21,22,23,24}, //frame número 2
    {1,2,3,4,9,11,12,13,14,19,21,22,23,24}, //frame número 3
    {1,5,6,7,8,9,11,14,15,18,21,24}, //frame número 4
    {1,2,3,4,9,12,13,14,15,11,21,22,23,24}, //frame número 5
    {1,2,3,5,9,11,12,13,14,15,21,22,23}, //frame número 6
    {2,7,11,19,20,21,22,23,24}, //frame número 7
    {1,2,3,5,9,11,12,13,15,19,21,22,23}, //frame número 8
    {1,2,3,9,10,11,12,13,15,19,21,22,23},  //frame número 9
    {12,11,14,14,12,14,13,9,13,13}// Tamanhos de cada frame
};
int lenNumFrames = sizeof(framesNums0_9)/sizeof(framesNums0_9[0]);
uint8_t colorsFrames[10][15][3];

void ConfigPins(uint pin, bool InorOut, bool pullUP);

void Debouncing(uint pin_button);
static void gpio_irq_handler(uint gpio, uint32_t events);

int main(){
    stdio_init_all();
    ConfigPins(PIN_LED_R, GPIO_OUT, false);
    ConfigPins(PIN_LED_G, GPIO_OUT, false);
    Debouncing(PIN_BUTTON_A);
    Debouncing(PIN_BUTTON_B);
    ws2812_init(PIN_LEDS, LED_COUNT);

    for (int colorsLen = 0; colorsLen < lenNumFrames-1; colorsLen++){    
        for (int frameLen= 0; frameLen< framesNums0_9[lenNumFrames-1][colorsLen]; frameLen++){
            colorsFrames[colorsLen][frameLen][0] = 0;
            colorsFrames[colorsLen][frameLen][1] = 5;
            colorsFrames[colorsLen][frameLen][2] = 30;
        }
    }
    Map_leds_ON(framesNums0_9[0], colorsFrames[0], 12, true);
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
    ConfigPins(pin_button, GPIO_IN, false);
    gpio_set_irq_enabled_with_callback(pin_button, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
}
void gpio_irq_handler(uint gpio, uint32_t events){
    uint32_t current_time = to_us_since_boot(get_absolute_time());// pega o tempo atual em microssegundos
    // Verifica se passou tempo suficiente desde o último evento
    if (current_time - last_time > 200000){ // 200ms de debouncing
        last_time = current_time; // Atualiza o tempo do último evento
        // gpio_put(PIN_LED_G, !gpio_get(PIN_LED_G));
        if (gpio_get(PIN_BUTTON_A)){
            index++;
        }
        if (gpio_get(PIN_BUTTON_B)){
            index--;
        }
        if (index >= lenNumFrames-1){
            index = 0;
        }
        if (index < 0){
            index = lenNumFrames-2;
        }
        printf("Numero em LEd: %d\n", index);
        int lenLedsON = framesNums0_9[lenNumFrames-1][index];
        Map_leds_ON(framesNums0_9[index], colorsFrames[index], lenLedsON,true);
    }
}