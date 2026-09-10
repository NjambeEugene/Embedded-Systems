#include <stdint.h>

// ---- Register definitions ----
#define USART1_BASE   0x40013800
#define USART1_SR     (*(volatile unsigned int *)(USART1_BASE + 0x00))
#define USART1_DR     (*(volatile unsigned int *)(USART1_BASE + 0x04))
#define USART1_CR1    (*(volatile unsigned int *)(USART1_BASE + 0x0C))

// ---- Packet format ----
#define MAX_PAYLOAD_SIZE 32

typedef struct {
    uint8_t start;
    uint8_t length;
    uint8_t payload[MAX_PAYLOAD_SIZE];
    uint8_t checksum;
} Packet;

// ---- UART byte-level functions ----
void uart_send_byte(uint8_t byte) {
    while ((USART1_SR & 0x80) == 0) {}
    USART1_DR = byte;
}

uint8_t uart_receive_byte(void) {
    while ((USART1_SR & 0x20) == 0) {}
    return USART1_DR;
}

int main(void) {
    while (1) {
        // nothing here yet - we'll call our functions
        // once the checksum + packet builder + receiver
        // are done in Module 4.3 and Module 5
    }
}
			