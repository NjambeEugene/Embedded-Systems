#include <stdint.h>

// Register definitions 
#define USART1_BASE   0x40013800
#define USART1_SR     (*(volatile unsigned int *)(USART1_BASE + 0x00))
#define USART1_DR     (*(volatile unsigned int *)(USART1_BASE + 0x04))
#define USART1_CR1    (*(volatile unsigned int *)(USART1_BASE + 0x0C))
#define RCC_APB2ENR   (*(volatile unsigned int *)(0x40021018))
#define GPIOA_CRH     (*(volatile unsigned int *)(0x40010804))
#define USART1_BRR    (*(volatile unsigned int *)(USART1_BASE + 0x08))
	

//  Packet format 
#define MAX_PAYLOAD_SIZE 32
#define START_BYTE 0xAA


typedef struct {
    uint8_t start;
    uint8_t length;
    uint8_t payload[MAX_PAYLOAD_SIZE];
    uint8_t checksum;
} Packet;

//  UART byte-level functions 
void uart_send_byte(uint8_t byte) {
    while ((USART1_SR & 0x80) == 0) {}
    USART1_DR = byte;
}

uint8_t uart_receive_byte(void) {
    while ((USART1_SR & 0x20) == 0) {}
    return USART1_DR;
}

uint8_t calculate_checksum(uint8_t *data, uint8_t length) {
    uint8_t checksum = 0;
    for (uint8_t i = 0; i < length; i++) {
        checksum = checksum ^ data[i];
    }
    return checksum;
}
void build_packet(Packet *pkt, uint8_t *payload, uint8_t length) {
   pkt -> start =START_BYTE;
	 pkt -> length = length;
	 
	
	for(uint32_t i=0;i<length; i++){
	pkt -> payload[i]  =payload[i]; 
	} 
	
	pkt ->checksum =calculate_checksum(payload, length);
}

typedef enum {
    WAIT_FOR_START,
    READ_LENGTH,
    READ_PAYLOAD,
    READ_CHECKSUM
} ReceiverState;

void receive_byte(uint8_t b) {
    static ReceiverState state = WAIT_FOR_START;
    static Packet incoming;
    static uint8_t payload_index = 0;

    switch (state) {
        case WAIT_FOR_START:
            if (b == START_BYTE) {
                incoming.start = b;
                state = READ_LENGTH;
            }
            break;
						
				case READ_LENGTH:
						incoming.length =b;
						payload_index= 0;
						state = READ_PAYLOAD;
				    break;
					   	 					
        
				case READ_PAYLOAD:
					incoming.payload[payload_index] =b;
				  payload_index ++;
				  if( payload_index == incoming.length){
						state = READ_CHECKSUM;
					}
					break;
       
				 
        case READ_CHECKSUM:
					
				  incoming.checksum =b;
				  if(b ==calculate_checksum(incoming.payload,incoming.length)){
      				 // packet valid - checksum mmatched
                state = WAIT_FOR_START;}
					else{
						 // packet invalid - checksum mismatch, discard
					 state = WAIT_FOR_START;
					}
            break;
				}
			}

void uart_init(void) {
    RCC_APB2ENR |= (0x04 | 0x4000);
    GPIOA_CRH &= ~(0xFF << 4);
    GPIOA_CRH |= (0x4B << 4);
    USART1_BRR = 0x0341;
    USART1_CR1 |= (0x2000 | 0x08 | 0x04);
}
 int main(void){
   uint8_t testPayload[3] = {0x12, 0x34, 0x56};
    Packet testPkt;
    
	 uart_init();
	 uart_send_byte('A');
    build_packet(&testPkt, testPayload, 3);
     
	 receive_byte(0xAA);
receive_byte(0x03);
receive_byte(0x12);
receive_byte(0x34);
receive_byte(0x56);
receive_byte(0x70);
    while (1) {
       
    }
}
			
