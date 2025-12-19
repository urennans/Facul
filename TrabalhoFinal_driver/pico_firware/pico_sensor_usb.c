#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include <stdio.h>
#include <string.h>

#define LED_PIN 15
#define LDR_PIN 26

const int MIN_RAW = 300;
const int MAX_RAW = 3500;

void print_menu(void) {
    printf("\n=== MENU ===\n");
    printf("[0-9]  Brilho manual (0=off, 9=max)\n");
    printf("[a]    Modo automático sensor\n");
    printf("[m]    Modo manual fixo\n");
    printf("[t]    Teste sequencial LED\n");
    printf("[s]    Status atual\n");
    printf("[h]    Ajuda (este menu)\n");
    printf("[q]    Sair (msg apenas)\n");
    printf("============\n");
}

int main() {
    stdio_init_all();
    
    // Configura hardware
    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(LED_PIN);
    pwm_set_wrap(slice_num, 255);
    pwm_set_enabled(slice_num, true);
    
    adc_init();
    adc_gpio_init(LDR_PIN);
    adc_select_input(0);
    
    sleep_ms(2000);
    
    bool auto_mode = true;
    uint8_t manual_brightness = 0;
    char input_buffer[32];
    int buffer_index = 0;
    
    printf("\n================================\n");
    printf("   CONTROLE LED + SENSOR LDR\n");
    printf("================================\n");
    print_menu();
    printf("\nPronto para comandos...\n");
    
    while (1) {
        // Lê caracteres se disponíveis
        int ch = getchar_timeout_us(0);
        if (ch != PICO_ERROR_TIMEOUT) {
            if (ch == '\r' || ch == '\n') {
                if (buffer_index > 0) {
                    input_buffer[buffer_index] = '\0';
                    
                    // Processa comando de texto
                    if (strcmp(input_buffer, "help") == 0 || strcmp(input_buffer, "h") == 0) {
                        print_menu();
                    }
                    else if (strcmp(input_buffer, "status") == 0 || strcmp(input_buffer, "s") == 0) {
                        uint16_t raw = adc_read();
                        printf("Status: ");
                        if (auto_mode) {
                            printf("MODO AUTO - Sensor: %u\n", raw);
                        } else {
                            printf("MODO MANUAL - PWM: %d\n", manual_brightness);
                        }
                    }
                    else if (strcmp(input_buffer, "test") == 0 || strcmp(input_buffer, "t") == 0) {
                        printf("Teste sequencial LED:\n");
                        for (int i = 0; i <= 100; i += 10) {
                            int pwm = i * 255 / 100;
                            printf("  %3d%% -> PWM=%d\n", i, pwm);
                            pwm_set_gpio_level(LED_PIN, pwm);
                            sleep_ms(300);
                        }
                        pwm_set_gpio_level(LED_PIN, 0);
                        printf("Teste completo.\n");
                    }
                    else if (strcmp(input_buffer, "auto") == 0 || strcmp(input_buffer, "a") == 0) {
                        auto_mode = true;
                        printf("Modo AUTO ativado (controlado pelo sensor)\n");
                    }
                    else if (strcmp(input_buffer, "manual") == 0 || strcmp(input_buffer, "m") == 0) {
                        auto_mode = false;
                        printf("Modo MANUAL ativado. Use 0-9 para controlar.\n");
                    }
                    
                    buffer_index = 0;
                }
            }
            else if (ch >= '0' && ch <= '9') {
                auto_mode = false;
                manual_brightness = (ch - '0') * 28;
                if (ch == '9') manual_brightness = 255;
                
                pwm_set_gpio_level(LED_PIN, manual_brightness);
                printf("LED: PWM = %d (%d%%)\n", 
                       manual_brightness, 
                       manual_brightness * 100 / 255);
            }
            else if (ch == 'q') {
                printf("Comando 'q' recebido. Continuando operação...\n");
            }
            else if (buffer_index < 31) {
                // Armazena para comandos de texto
                input_buffer[buffer_index++] = ch;
            }
        }
        
        // Modo automático: controla pelo sensor
        if (auto_mode) {
            static uint32_t last_update = 0;
            uint32_t now = to_ms_since_boot(get_absolute_time());
            
            if (now - last_update > 100) {  // Atualiza a cada 100ms
                uint16_t raw = adc_read();
                int clipped = raw;
                if (clipped < MIN_RAW) clipped = MIN_RAW;
                if (clipped > MAX_RAW) clipped = MAX_RAW;
                
                float norm = (float)(clipped - MIN_RAW) / (float)(MAX_RAW - MIN_RAW);
                int duty = (int)((1.0f - norm) * 255.0f);
                
                pwm_set_gpio_level(LED_PIN, duty);
                last_update = now;
                
                // Mostra a cada 2 segundos
                static uint32_t last_print = 0;
                if (now - last_print > 2000) {
                    printf("AUTO: Sensor=%4u -> PWM=%3d\n", raw, duty);
                    last_print = now;
                }
            }
        }
        
        sleep_ms(10);
    }
    
    return 0;
}