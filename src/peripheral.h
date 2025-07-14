#ifndef PERIPHERAL_H_
#define PERIPHERAL_H_

#include "stm32f10x.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>


void init_peripherals(void);
void ADC_Start(uint8_t*);
void ADC_Stop(uint8_t*);
void CRC_init(void);
void Transmit_Single_ADC_Value(uint16_t adc_value);
void delay(uint32_t ms);
void print_debug(const char* msg, ...);

#endif /* PERIPHERAL_H_ */
