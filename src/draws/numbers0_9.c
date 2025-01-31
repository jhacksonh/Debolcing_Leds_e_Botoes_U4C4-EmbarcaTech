#include "pico/stdlib.h"
#include "numbers0_9.h"
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

void ColorsGenerate(){
    for (int colorsLen = 0; colorsLen < lenNumFrames-1; colorsLen++){    
        for (int frameLen= 0; frameLen< framesNums0_9[lenNumFrames-1][colorsLen]; frameLen++){
            colorsFrames[colorsLen][frameLen][0] = 0;
            colorsFrames[colorsLen][frameLen][1] = 13;
            colorsFrames[colorsLen][frameLen][2] = 80;
        }
    }
}