#ifndef LEDS_H
#define LEDS_H

#include <pico/stdlib.h>

void ws2812_init(uint pin, int len_leds);
void Map_leds_ON(uint8_t *LedsOn, uint8_t colorsOn[][3], int LedsOnCount, bool clear_cache);
void Clear_leds();

#endif