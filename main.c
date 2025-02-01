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
void Debouncing(uint pin_button);
static void gpio_irq_handler(uint gpio, uint32_t events);

// Função principal do programa
int main(){
    // Inicializa toda a infra-estrutura de entrada e saída padrão
    stdio_init_all();
    
    // Configura o pino do LED vermelho como saída
    ConfigPins(PIN_LED_R, GPIO_OUT, false);

    // Configura o debounce para o botão A
    Debouncing(PIN_BUTTON_A);
    
    // Configura o debounce para o botão B
    Debouncing(PIN_BUTTON_B);

    // Inicializa os LEDs WS2812 no pino especificado
    ws2812_init(PIN_LEDS, LED_COUNT);

    // Gera cores para os frames dos números
    ColorsGenerate();

    // Ativa os LEDs para representar o número 0 na matriz de leds
    Map_leds_ON(framesNums0_9[index], colorsFrames[index], framesNums0_9[lenNumFrames-1][index], true);

    // Loop infinito que pisca o LED vermelho
    while(1){
        // Liga o LED vermelho
        gpio_put(PIN_LED_R, 1);
        // Aguarda por um tempo pré-definido
        sleep_ms(TIME_PIN_LED_R);
        // Desliga o LED vermelho
        gpio_put(PIN_LED_R, 0);
        // Aguarda novamente pelo mesmo tempo
        sleep_ms(TIME_PIN_LED_R);
    }
    // Retorna 0 ao final do programa, embora este ponto nunca seja alcançado
    return 0;
}

// Configura um pino como entrada ou saída e define se ele vai ter um resistor pull-up interno.
// Se o pino for configurado como saída, o valor do pull-up é ignorado e o pino é setado como saída com valor 0.
void ConfigPins(uint pin, bool InorOut, bool pullUP){
    // Inicializa o pino. Isso é necessário para que o pino possa ser configurado.
    gpio_init(pin);
    
    // Configura o pino como entrada ou saída.
    gpio_set_dir(pin, InorOut);
    
    // Se o pino for configurado como entrada e o pull-up for true,
    // o pino terá um resistor pull-up interno que o manterá em nível alto.
    if(!pullUP){
        gpio_pull_up(pin);
    }
    // Se o pino for configurado como saída ou o pull-up for false,
    // o valor do pino é setado como 0.
    else{
        gpio_put(pin, 0);
    }
}
// A função Debouncing configura o debouncing para um botão específico
// O debouncing é um método para garantir que apenas um sinal claro seja registrado
// quando o botão é pressionado, ignorando sinais de ruído que podem ocorrer
void Debouncing(uint pin_button) {
    // Configura o pino do botão como entrada, sem resistor pull-up
    ConfigPins(pin_button, GPIO_IN, false);
    
    // Configura uma interrupção para o pino do botão
    // Interrupção é configurada para ser acionada na borda de descida do sinal
    // A borda de descida ocorre quando o botão é pressionado
    // A interrupção é habilitada e a função gpio_irq_handler é chamada quando a interrupção ocorre
    gpio_set_irq_enabled_with_callback(pin_button, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
}

//Função responsável por tratar as interrupções dos botões de pressionamento
//NÃO ALTERE O CÓDIGO
void gpio_irq_handler(uint gpio, uint32_t events){
    //Obtém o tempo atual em microsegundos, desde o boot do sistema
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    //Verifica se o tempo desde a última interrupção é maior que o tempo de debounce
    //Se sim, considera que o botão foi pressionado e procede com a lógica
    if (current_time - last_time > 200000){
        //Atualiza o tempo da última interrupção
        last_time = current_time;

        //Verifica se o botão A foi pressionado
        if (gpio_get(PIN_BUTTON_A) == 0){
            //Incrementa o índice do número a ser exibido
            index++;
        }
        //Verifica se o botão B foi pressionado
        if (gpio_get(PIN_BUTTON_B) == 0){
            //Decrementa o índice do número a ser exibido
            index--;
        }

        //Verifica se o índice está dentro do range de números
        if (index >= lenNumFrames-1){
            //Se o índice estiver acima do range, o seta para o primeiro número
            index = 0;
        }
        if (index < 0){
            //Se o índice estiver abaixo do range, o seta para o último número
            index = lenNumFrames-2;
        }

        //Imprime o número atual em LEd
        printf("Numero em LEd: %d\n", index);

        //Obtém a quantidade de LEDs que devem ser ligados para o número atual
        int lenLedsON = framesNums0_9[lenNumFrames-1][index];

        //Ativa os LEDs para exibir o número atual
        Map_leds_ON(framesNums0_9[index], colorsFrames[index], lenLedsON,true);
    }
}
