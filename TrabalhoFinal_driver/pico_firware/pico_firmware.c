#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "tusb.h"

#define LED_PIN 15
#define BUFFER_SIZE 64

// Comandos USB
typedef enum {
    CMD_LED_ON = 0x01,
    CMD_LED_OFF = 0x02,
    CMD_LED_BLINK = 0x03,
    CMD_GET_STATUS = 0x04,
    CMD_SET_BLINK_RATE = 0x05
} usb_cmd_t;

// Buffer para receber comandos
uint8_t rx_buffer[BUFFER_SIZE];
uint8_t tx_buffer[BUFFER_SIZE];

void setup() {
    // Inicializa LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);
    
    // Inicializa USB
    stdio_init_all();
    tusb_init();
    
    printf("Pico USB Device Ready\n");
    printf("LED on GPIO %d\n", LED_PIN);
}

void process_command(uint8_t *data, uint8_t len) {
    if (len == 0) return;
    
    uint8_t cmd = data[0];
    uint8_t response_len = 0;
    
    switch(cmd) {
        case CMD_LED_ON:
            gpio_put(LED_PIN, 1);
            tx_buffer[0] = 0x01; // ACK
            tx_buffer[1] = 0x00; // LED ON
            response_len = 2;
            printf("LED ON\n");
            break;
            
        case CMD_LED_OFF:
            gpio_put(LED_PIN, 0);
            tx_buffer[0] = 0x01; // ACK
            tx_buffer[1] = 0x01; // LED OFF
            response_len = 2;
            printf("LED OFF\n");
            break;
            
        case CMD_LED_BLINK:
            if (len >= 2) {
                uint8_t times = data[1];
                for(int i = 0; i < times; i++) {
                    gpio_put(LED_PIN, 1);
                    sleep_ms(200);
                    gpio_put(LED_PIN, 0);
                    sleep_ms(200);
                }
                tx_buffer[0] = 0x01; // ACK
                tx_buffer[1] = times;
                response_len = 2;
                printf("LED blinked %d times\n", times);
            }
            break;
            
        case CMD_GET_STATUS:
            tx_buffer[0] = 0x02; // Status response
            tx_buffer[1] = gpio_get(LED_PIN);
            tx_buffer[2] = 0x01; // Device ready
            response_len = 3;
            printf("Status requested\n");
            break;
            
        default:
            tx_buffer[0] = 0xFF; // Error
            response_len = 1;
            printf("Unknown command: 0x%02X\n", cmd);
    }
    
    // Envia resposta via USB
    if (response_len > 0 && tud_ready()) {
        tud_vendor_write(tx_buffer, response_len);
        tud_vendor_flush();
    }
}

int main() {
    setup();
    
    while (true) {
        tud_task(); // Processa tarefas USB
        
        // Verifica se há dados para ler
        if (tud_vendor_available()) {
            uint32_t count = tud_vendor_read(rx_buffer, BUFFER_SIZE);
            if (count > 0) {
                process_command(rx_buffer, count);
            }
        }
        
        sleep_ms(1);
    }
    
    return 0;
}