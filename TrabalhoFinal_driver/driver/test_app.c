#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define DEVICE_PATH "/dev/pico_usb0"
#define CMD_LED_ON 0x01
#define CMD_LED_OFF 0x02
#define CMD_LED_BLINK 0x03
#define CMD_GET_STATUS 0x04

void send_command(int fd, unsigned char cmd, unsigned char param) {
    unsigned char buffer[2];
    ssize_t ret;
    
    buffer[0] = cmd;
    buffer[1] = param;
    
    printf("Enviando comando: 0x%02X 0x%02X\n", cmd, param);
    
    ret = write(fd, buffer, 2);
    if (ret < 0) {
        perror("Erro ao escrever");
    } else {
        printf("Comando enviado (%ld bytes)\n", ret);
    }
}

void read_response(int fd) {
    unsigned char buffer[64];
    ssize_t ret;
    
    usleep(100000); // Aguarda resposta
    
    ret = read(fd, buffer, sizeof(buffer));
    if (ret < 0) {
        perror("Erro ao ler");
    } else if (ret == 0) {
        printf("Nenhum dado recebido\n");
    } else {
        printf("Resposta recebida (%ld bytes): ", ret);
        for (int i = 0; i < ret; i++) {
            printf("0x%02X ", buffer[i]);
        }
        printf("\n");
        
        if (buffer[0] == 0x01) {
            printf("ACK recebido\n");
            if (buffer[1] == 0x00) printf("LED está LIGADO\n");
            if (buffer[1] == 0x01) printf("LED está DESLIGADO\n");
        } else if (buffer[0] == 0x02) {
            printf("Status: LED=%s, Device=READY\n", 
                   buffer[1] ? "ON" : "OFF");
        }
    }
}

int main() {
    int fd;
    int choice;
    unsigned char param;
    
    printf("=== Teste Driver USB Pico ===\n");
    
    // Abre dispositivo
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Erro ao abrir dispositivo");
        return 1;
    }
    
    printf("Dispositivo aberto com sucesso\n");
    
    while (1) {
        printf("\n=== Menu ===\n");
        printf("1. Ligar LED\n");
        printf("2. Desligar LED\n");
        printf("3. Piscar LED\n");
        printf("4. Ver status\n");
        printf("5. Sair\n");
        printf("Escolha: ");
        
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                send_command(fd, CMD_LED_ON, 0);
                read_response(fd);
                break;
                
            case 2:
                send_command(fd, CMD_LED_OFF, 0);
                read_response(fd);
                break;
                
            case 3:
                printf("Quantas vezes? ");
                scanf("%hhu", &param);
                send_command(fd, CMD_LED_BLINK, param);
                read_response(fd);
                break;
                
            case 4:
                send_command(fd, CMD_GET_STATUS, 0);
                read_response(fd);
                break;
                
            case 5:
                close(fd);
                printf("Encerrando...\n");
                return 0;
                
            default:
                printf("Opção inválida\n");
        }
    }
    
    close(fd);
    return 0;
}