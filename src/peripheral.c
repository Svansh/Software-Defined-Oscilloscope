#include "peripheral.h"
#include "parameters.h"
#include "frame.h"

extern Frame_t* current_frame;
extern uint16_t sIndex;
extern uint16_t trigger_index;

uint32_t ticks;
uint8_t triggered = 0;

extern uint8_t keepSampling;

void SysTick_Handler(void){
	ticks++;
}

void delay(uint32_t ms){
	ticks = 0;
	while(ticks < ms);
}

static void TL_init(void){

	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
	GPIOA->CRL &= ~(GPIO_CRL_CNF3 | GPIO_CRL_MODE3);
	GPIOA->CRL |= GPIO_CRL_CNF3_1 | GPIO_CRL_MODE3; // Alternate Function Push-Pull

	TIM2->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1; // 110 (PWM Mode 1)
	TIM2->CCMR2 |= TIM_CCMR2_OC4PE; // Output Compare 4 Preload enable
	TIM2->CR1 |= TIM_CR1_ARPE; // auto-reload Preload enable
	TIM2->EGR |= TIM_EGR_UG; // Update Generation
	TIM2->CCER |= TIM_CCER_CC4E; // Channel 4 enable

	TIM2->PSC = 0;
	TIM2->ARR = 71; // Timer frequency of 1MHz
	TIM2->CCR4 = 36; // 50% DUTY CYCLE

}


void TL_Start(void) {
	TIM2->CR1 |= TIM_CR1_CEN; // Counter Enable
}


static void TriggerIT_init(void){

	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
	GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);
	GPIOA->CRL |= GPIO_CRL_CNF5_1 | GPIO_CRL_MODE5;

	AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI5_PA;

	EXTI->IMR |= EXTI_IMR_MR5;
	EXTI->RTSR |= EXTI_RTSR_TR5;

	NVIC_EnableIRQ(EXTI9_5_IRQn);

}


void EXTI9_5_IRQ_IRQHandler(void){
	EXTI->PR |= EXTI_PR_PR5 | EXTI_PR_PR6 | EXTI_PR_PR7 | EXTI_PR_PR8 | EXTI_PR_PR9;
	triggered = 1;
	trigger_index = sIndex;

}


static void ADC_init(){

	// Clock Initialization for ADC1
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
	GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_MODE0); // Analog Input Mode

	ADC1->CR1 |= ADC_CR1_SCAN | ADC_CR1_EOCIE;
	ADC1->CR2 |= ADC_CR2_CONT;
	ADC1->SMPR2 |= ADC_SMPR2_SMP0; // 239.5 cycles between each conversion

}

void ADC1_2_IRQHandler(void){
	print_debug("Inside ADC1 ISR!!!\n\r");
}


void ADC_Start(uint8_t* keepSamplingPtr){
	if(*keepSamplingPtr == 0){
		print_debug("Inside ADC START!!!\n\r");
		ADC1->CR2 |= ADC_CR2_ADON;
		ADC1->CR2 |= ADC_CR2_CAL;
		while(ADC1->CR2 & ADC_CR2_CAL);
		ADC1->CR2 |= ADC_CR2_ADON;

		*keepSamplingPtr = 1;
	}
	print_debug("Exiting ADC START\n\r");
	print_debug("Value of keepSampling: %d\n\r", keepSampling);
}

void ADC_Stop(uint8_t* keepSamplingPtr){
	if(*keepSamplingPtr == 1){
		ADC1->CR2 &= ~(ADC_CR2_ADON);
		DMA1_Channel4->CCR &= ~(DMA_CCR4_EN);

		*keepSamplingPtr = 0;
	}
}


void Transmit_Single_ADC_Value(uint16_t adc_value){
	while(!(USART1->SR & USART_SR_TXE));
	USART1->DR = (uint8_t)adc_value;
	while(!(USART1->SR & USART_SR_TC));
	USART1->DR = (uint8_t)(adc_value >> 8);
	while(!(USART1->SR & USART_SR_TC));
}


static void DMA_init(void) {

	RCC->AHBENR |= RCC_AHBENR_DMA1EN;

	DMA1_Channel4->CPAR = (uint32_t)&(USART1->DR);
	DMA1_Channel4->CCR |= DMA_CCR4_TCIE | DMA_CCR4_CIRC | DMA_CCR4_DIR; // Transmitting from memory to peripheral

	NVIC_EnableIRQ(DMA1_Channel4_IRQn);

}

void DMA1_Channel4_IRQHandler(void){
	if(DMA1->ISR | DMA_ISR_TCIF4){
		DMA1->IFCR |= DMA_IFCR_CTCIF4;
		DMA1_Channel4->CCR &= ~(DMA_CCR4_EN);
	}
}


static void UART_init(void){

	//Clock Initialization for USART1
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
	// Setting A9 as AF Output Push-Pull and A10 as Input Pulled-up to 3.3V
	GPIOA->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9 | GPIO_CRH_MODE10 | GPIO_CRH_CNF10);
	GPIOA->CRH |= GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9 | GPIO_CRH_CNF10_1;
	GPIOA->ODR |= GPIO_ODR_ODR10;

	USART1->BRR = 0x1d4c; // 9600
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE | USART_CR1_RXNEIE; // Tx, Rx, USART, and Rx Not empty interrupt enable
	USART1->CR3 |= USART_CR3_DMAT; // Enabling DMA for USART1

	NVIC_EnableIRQ(USART1_IRQn); // Enabling Interrupt for USART1
}

void print_debug(const char* msg, ...){
	char buff[50];
	va_list args;
	va_start(args, msg);
	vsprintf(buff, msg, args);
	va_end(args);
	uint8_t i;
	for(i = 0; i < strlen(buff); i++){
		while(!(USART1->SR & USART_SR_TXE));
		USART1->DR = buff[i];
		while(!(USART1->SR & USART_SR_TC));
	}
}

void CRC_init(void){
	RCC->AHBENR |= RCC_AHBENR_CRCEN;
	CRC->CR = 1; // Resets the value of CRC->DR to 0xFFFF FFFF
}


void init_peripherals(void){

	SysTick_Config(SystemCoreClock / 1000);
	ADC_init();
	DMA_init();
	UART_init();
//	TL_init();
//	TriggerIT_init();

}




