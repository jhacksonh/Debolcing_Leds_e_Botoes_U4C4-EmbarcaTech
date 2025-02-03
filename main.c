#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#include "ws2812.pio.h" // Inclui o arquivo de configuração do PIO
#include "leds.h" // Arquivo de funções para controlar os LEDs
#include "numbers0_9.h" // Arquivo de funções para desenhar os números

#define PIN_LED_R 13 // Pino do LED vermelho
#define PIN_BUTTON_A 5 // Pino do botão A
#define PIN_BUTTON_B 6 // Pino do botão B
#define PIN_LEDS 7 // Pino dos LEDs
#define LED_COUNT 25 // Quantidade de LEDs na matriz
#define TIME_PIN_LED_R (1000/5)/2 // Tempo em microsegundos para piscar o LED vermelho
static uint32_t last_time = 0; // Variável para armazenar o tempo da última interrupção
static int index = 0; // Variável para armazenar o índice do número a ser exibido

// Protótipos das funções
void ConfigPins(uint pin, bool InorOut, bool pullUP);
void Setup_Button_IRQ(uint pin_button);
void Callback_With_Debouncing(uint gpio, uint32_t events);
void Set_Number_in_Matriz(uint gpio);
// Função principal do programa
int main(){
    stdio_init_all();// Inicializa toda a infra-estrutura de entrada e saída padrão
    ConfigPins(PIN_LED_R, GPIO_OUT, false);// Configura o pino do LED vermelho como saída
    Setup_Button_IRQ(PIN_BUTTON_A);// Configura a interrupção para o botão A
    Setup_Button_IRQ(PIN_BUTTON_B);// Configura a interrupção para o botão B
    ws2812_init(PIN_LEDS, LED_COUNT);// Inicializa os LEDs WS2812 no pino especificado
    ColorsGenerate();// Gera cores para os frames dos números 0 a 9

    // Ativa os LEDs para representar o número 0 na matriz de leds
    Map_leds_ON(framesNums0_9[index], colorsFrames[index], framesNums0_9[lenNumFrames-1][index], true);
    while(1){// Loop infinito que pisca o LED vermelho 5 vezes por segundo
        gpio_put(PIN_LED_R, !gpio_get(PIN_LED_R));// Liga ou desliga o LED vermelho
        sleep_ms(TIME_PIN_LED_R);// Aguarda o tempo especificado
    }
    return 0;
}
// Configura um pino como entrada/saída, (define se é pull-up, se sim,ignora saída, que inicia em 0.)
void ConfigPins(uint pin, bool InorOut, bool pullUP){
    // Inicializa o pino. Isso é necessário para que o pino possa ser configurado.
    gpio_init(pin);
    // Configura o pino como entrada ou saída.
    gpio_set_dir(pin, InorOut);
    // Se configurado como entrada com pull-up, o pino terá um resistor interno mantendo nível alto.  
    if(pullUP)gpio_pull_up(pin);
    else gpio_put(pin, 0);
}
// Setup_Button_IRQ configura a interrupção para detectar o pressionamento do botão.
void Setup_Button_IRQ(uint pin_button) {
    // Configura o pino do botão como entrada, sem resistor pull-up
    ConfigPins(pin_button, GPIO_IN, true);
    // Configura uma interrupção na borda de descida do botão, chamando Callback_With_Debouncing ao acionar.  
    gpio_set_irq_enabled_with_callback(pin_button, GPIO_IRQ_EDGE_FALL, true, &Callback_With_Debouncing);
}
//Função responsável por tratar as interrupções dos botões de pressionamento
void Callback_With_Debouncing(uint gpio, uint32_t events){
    //Obtém o tempo atual em microsegundos, desde o boot do sistema
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    // Verifica se o tempo desde a última interrupção excede o debounce antes de processar o pressionamento.  
    if (current_time - last_time > 200000){
        last_time = current_time; //Atualiza o tempo da última interrupção
        Set_Number_in_Matriz(gpio); // Chama a função para exibir o número na matriz
    }
}
void Set_Number_in_Matriz(uint gpio){
    //Verifica se o botão A foi pressionado
    if (gpio == PIN_BUTTON_A)index++;//Incrementa o índice do número a ser exibido
    //Verifica se o botão B foi pressionado
    if (gpio == PIN_BUTTON_B)index--;//Decrementa o índice do número a ser exibido
    //Verifica se o índice está dentro do range de números (se chegar ao 10 vai para o 0, se chegar ao -1 vai para o 9)
    index = index >= lenNumFrames-1?index = 0:index < 0?lenNumFrames-2:index;
    //Obtém a quantidade de LEDs que devem ser ligados para o número atual
    int lenLedsON = framesNums0_9[lenNumFrames-1][index];
    //Ativa os LEDs para exibir o número atual
    Map_leds_ON(framesNums0_9[index], colorsFrames[index], lenLedsON,true);
}