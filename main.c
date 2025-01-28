#include "pico/stdlib.h"

#define PIN_R 13
#define TIME_PIN_R (1000/5)/2

void configPins(uint pin, bool InorOut, bool pullUP);

int main(){
    configPins(PIN_R, GPIO_OUT, false);

    while(1){
        gpio_put(PIN_R, 1);
        sleep_ms(TIME_PIN_R);
        gpio_put(PIN_R, 0);
        sleep_ms(TIME_PIN_R);
    }
    return 0;
}

void configPins(uint pin, bool InorOut, bool pullUP){
    gpio_init(pin);
    gpio_set_dir(pin, InorOut);
    if(!pullUP){
        gpio_pull_up(pin);
    }else{
        gpio_put(pin, 0);
    }
}