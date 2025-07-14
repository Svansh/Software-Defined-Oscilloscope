#include "peripheral.h"
#include "capture.h"
#include "frame.h"
#include "parameters.h"

extern uint16_t ch1_samples[2048];
extern uint8_t* keepSamplingPtr;

typedef enum {
	UART_WAITING_FOR_COMMAND,
	UART_WAITING_FOR_DATA
} UART_STATE_t;

volatile UART_STATE_t uart_state = UART_WAITING_FOR_COMMAND;
volatile uint8_t trigger_level = 0;

void send_ack(void){
	while(!(USART1->SR & USART_SR_TXE));
	USART1->DR = 0xff; // ACK
	while(!(USART1->SR & USART_SR_TC));
}

void send_nack(void){
	while(!(USART1->SR & USART_SR_TXE));
	USART1->DR = 0x00; // NACK
	while(!(USART1->SR & USART_SR_TC));
}

int main(void){

    init_peripherals();
    print_debug("Peripherals Initialized!!!\n\r");

    while(1) {
    	print_debug("Sampling Loop!!!\n\r");
    	captureSignals_loop();
    	print_debug("Value of first element: 0x%02X\n\rValue of last element: 0x%02X\n\r", ch1_samples[0], ch1_samples[2047]);
    	delay(1000);
   }
}


void USART1_IRQHandler(void){
	if(USART1->SR & USART_SR_RXNE){

		uint8_t data = USART1->DR;
		switch(uart_state){
			case UART_WAITING_FOR_COMMAND:
				if(data == CONVERSION_START){
					ADC_Start(keepSamplingPtr);
					send_ack();
					print_debug("Value of %d\n\r", *keepSamplingPtr);
				}
				else if(data == CONVERSION_STOP){
					ADC_Stop(keepSamplingPtr);
					send_ack();
					print_debug("Value of %d\n\r", *keepSamplingPtr);

				}
				else if(data == TRIGGER_LEVEL){
					uart_state = UART_WAITING_FOR_DATA;
					send_ack();
				}
				else if(data == BAUD_RATE){
					uart_state = UART_WAITING_FOR_DATA;
					send_ack();
				}
				else if(data == RECV_ACK){

				}
				else if(data == 0x44){
			    	print_debug("Value of first element: 0x%02X\n\rValue of last element: 0x%02X\n\r", ch1_samples[0], ch1_samples[2047]);
				}
				else{
					send_nack();
				}
				break;
			case UART_WAITING_FOR_DATA:
				trigger_level = data;
				uart_state = UART_WAITING_FOR_COMMAND;
				print_debug("Trigger Level received: %d", trigger_level);
				break;
		}
	}
}




