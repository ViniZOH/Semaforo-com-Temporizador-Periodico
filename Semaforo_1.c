#include <stdio.h>
#include "pico/stdlib.h"

// Defina pinos para cada LED
#define LED_VERMELHO 13
#define LED_AZUL  12 //Será o nosso Amarelo
#define LED_VERDE    11

// Intervalo do semáforo (3 segundos = 3000 ms)
#define SEMAFORO_INTERVALO_MS 3000

// Variável global do estado do semáforo
static int estado_atual = 0;

// Callback chamado pelo timer de 3s
bool semaforo_callback(struct repeating_timer *t) {
    // Avança para o próximo estado (0→1→2→0)
    estado_atual = (estado_atual + 1) % 3;

    // Ajusta LEDs conforme o novo estado
    switch (estado_atual) {
        case 0:  // Vermelho
            gpio_put(LED_VERMELHO, 1);
            gpio_put(LED_AZUL,  0);
            gpio_put(LED_VERDE,    0);
            printf("Callback Timer -> Estado: VERMELHO\n");
            break;
        case 1:  // Amarelo
            gpio_put(LED_VERMELHO, 0);
            gpio_put(LED_AZUL,  1);
            gpio_put(LED_VERDE,    0);
            printf("Callback Timer -> Estado: AMARELO\n");
            break;
        case 2:  // Verde
            gpio_put(LED_VERMELHO, 0);
            gpio_put(LED_AZUL,  0);
            gpio_put(LED_VERDE,    1);
            printf("Callback Timer -> Estado: VERDE\n");
            break;
    }

    // Retorne true para continuar repetindo
    return true;
}

int main() {
    stdio_init_all();

    // Inicializa pinos como saída
    gpio_init(LED_VERMELHO);
    gpio_set_dir(LED_VERMELHO, GPIO_OUT);
    gpio_init(LED_AZUL);
    gpio_set_dir(LED_AZUL, GPIO_OUT);
    gpio_init(LED_VERDE);
    gpio_set_dir(LED_VERDE, GPIO_OUT);

    // Estado inicial = 0 (Vermelho aceso)
    estado_atual = 0;
    gpio_put(LED_VERMELHO, 1);
    gpio_put(LED_AZUL,  0);
    gpio_put(LED_VERDE,    0);

    printf("Iniciando semáforo (estado: VERMELHO)\n");

    // Cria um timer repetitivo de 3s para mudar o semáforo
    struct repeating_timer timer;
    add_repeating_timer_ms(SEMAFORO_INTERVALO_MS, semaforo_callback, NULL, &timer);

    // Variável para controlar a impressão a cada 1s
    uint64_t ultima_impressao_us = time_us_64(); // Marca tempo inicial em microssegundos

    // Loop principal
    while (true) {
        // Verifica se 1 segundo (1.000.000 us) já se passou
        uint64_t agora = time_us_64();
        if (agora - ultima_impressao_us >= 1000000) {
            ultima_impressao_us = agora; // Atualiza a marca de tempo

            // Imprime info conforme o estado_atual
            switch (estado_atual) {
                case 0:
                    printf("[Loop Principal] LED VERMELHO aceso\n");
                    break;
                case 1:
                    printf("[Loop Principal] LED AMARELO aceso\n");
                    break;
                case 2:
                    printf("[Loop Principal] LED VERDE aceso\n");
                    break;
            }
        }

        // O tight_loop_contents() ajuda a manter o loop "vivo"
        tight_loop_contents();
    }

    return 0;
}
