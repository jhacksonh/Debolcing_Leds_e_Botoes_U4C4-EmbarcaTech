#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#include "ws2812.pio.h"
#include "leds.h"
#include "numbers0_9.h"

//Config Pinos
#define PIN_LED_R 13
#define PIN_LED_G 11
#define PIN_BUTTON_A 5
#define PIN_BUTTON_B 6
#define PIN_LEDS 7 // Pino de dados para o WS2812
#define LED_COUNT 25
#define TIME_PIN_LED_R (1000/5)/2
static uint32_t last_time = 0;
static int index = 0;

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

    ColorsGenerate();

    Map_leds_ON(framesNums0_9[index], colorsFrames[index], framesNums0_9[lenNumFrames-1][index], true);

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