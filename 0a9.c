#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "ws2812.pio.h"  // Biblioteca para LEDs WS2812

#define LED_R 13
#define LED_G 12
#define LED_B 11
#define BOTAO_A 5
#define BOTAO_B 6
#define WS2812_PIN 7

volatile int numero_atual = 0; // Número exibido na matriz
volatile bool debounce_a = false, debounce_b = false; // Flags de debounce

// Prototipação das funções
static void gpio_irq_handler(uint gpio, uint32_t events);
bool debounce_timer_callback(struct repeating_timer *t);
bool piscar_led_callback(struct repeating_timer *t);
void exibir_numero_na_matriz(int numero);
void desenhar_matriz(uint32_t matriz[5][5]);

// Timer para debounce e piscar LED
struct repeating_timer debounce_timer, led_timer;

int main() {
    stdio_init_all();
    ws2812_init(WS2812_PIN);

    // Configuração dos LEDs
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);

    // Configuração dos botões com pull-up
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);

    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);

    // Configuração das interrupções para os botões
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Timer para debounce (roda a cada 50ms)
    add_repeating_timer_ms(50, debounce_timer_callback, NULL, &debounce_timer);

    // Timer para piscar LED (100ms = 5Hz)
    add_repeating_timer_ms(100, piscar_led_callback, NULL, &led_timer);

    // Exibe o número inicial na matriz
    exibir_numero_na_matriz(numero_atual);

    while (true) {
        tight_loop_contents();
    }
}

// Interrupção dos botões com debounce
void gpio_irq_handler(uint gpio, uint32_t events) {
    if (gpio == BOTAO_A && !debounce_a) {
        debounce_a = true;
        numero_atual = (numero_atual + 1) % 10; // Alterna entre 0 e 9
        exibir_numero_na_matriz(numero_atual);
    } else if (gpio == BOTAO_B && !debounce_b) {
        debounce_b = true;
        numero_atual = (numero_atual - 1 + 10) % 10; // Alterna entre 0 e 9
        exibir_numero_na_matriz(numero_atual);
    }
}

// Timer de debounce
bool debounce_timer_callback(struct repeating_timer *t) {
    debounce_a = false;
    debounce_b = false;
    return true;
}

// Timer para piscar o LED vermelho
bool piscar_led_callback(struct repeating_timer *t) {
    static bool estado = false;
    gpio_put(LED_R, estado);
    estado = !estado;
    return true;
}

// Padrões para os números 0 a 9 em uma matriz 5x5
uint32_t numeros[10][5][5] = {
    { // Número 0
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    },
    { // Número 1
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 1, 0}
    },
    { // Número 2
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 1, 1}
    },
    { // Número 3
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {0, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    },
    { // Número 4
        {0, 1, 0, 0, 1},
        {1, 0, 0, 1, 0},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 1, 0},
        {0, 1, 0, 0, 0}
    },
    { // Número 5
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    },
    { // Número 6
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    },
    { // Número 7
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0}
    },
    { // Número 8
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    },
    { // Número 9
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    }
};

// Exibe um número na matriz WS2812
void exibir_numero_na_matriz(int numero) {
    desenhar_matriz(numeros[numero]);
}

// Função para exibir a matriz 5x5 na WS2812
void desenhar_matriz(uint32_t matriz[5][5]) {
    for (int y = 4; y >= 0; y--) {
        for (int x = 0; x < 5; x++) {
            if (matriz[y][x]) {
                ws2812_put_pixel(0xFFFFFF);//branco
            } else {
                ws2812_put_pixel(0x000000);//apagado
            }
        }
    }
}




