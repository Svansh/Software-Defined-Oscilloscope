#include "capture.h"
#include "peripheral.h"

extern Frame_t* current_frame;

uint8_t keepSampling = 0;
uint8_t* keepSamplingPtr = &keepSampling;

extern uint8_t triggered;
uint8_t* triggeredPtr = &triggered;

uint16_t sIndex = 0;
uint16_t* sIndexPtr = &sIndex;

uint16_t trigger_index = 0;

uint16_t lCtr;

uint16_t ch1_samples[2048] = {0};


void captureSignals_loop(){
	uint16_t length = current_frame->length;
	const char msg[] = "Inside wait_Adc loop!!!\n\r";

	asm volatile(

			"			ldrh r9, [%[sIndex]]  					\n\t" // loading sIndex into R9

			"sample_loop:"
			"			ldrb r0, [%[keepSampling]]              \n\t"
			"			cbz r0, finished                        \n\t"
			"			ldr r1,=0x40012400                      \n\t"
			""
			"wait_adc:"
			"			push {r0-r3, r9, lr}					\n\t"
			"			mov r0, %[debug_msg]					\n\t"
			"			blx %[debug_func]						\n\t"
			"			pop {r0-r3, r9, lr}						\n\t"
			"			ldr r0, [r1, #0]						\n\t"
			"			lsls r0, r0, #30						\n\t"
			"			bpl	wait_adc							\n\t"

			"			ldr r0, [r1, #0x4C]						\n\t" // Loading ADC1 DR Register
			"			strh r0, [%[ch1], r9, lsl #1]			\n\t"
			"			adds r9, r9, #1							\n\t"
			"			cmp r9, %[nSamp]						\n\t"
			"			bne not_overflowed						\n\t"
			"			mov r9, #0								\n\t"
			"			b finished								\n\t"


			"not_overflowed:"
			"			strh r9, [%[sIndex]]					\n\t"
			"			ldrb r0, [%[triggered]]					\n\t"
			"			cbz r0, not_triggered					\n\t"
			"			mov r2, %[lCtr]							\n\t"
			"			adds r2, #1								\n\t"
			"			cmp r2, %[half_samples]					\n\t"
			"			beq finished							\n\t"
			""
			"not_triggered:"
			"			b sample_loop							\n\t"
			"finished:"

			:
			:[keepSampling] "r" (keepSamplingPtr), [sIndex] "r" (sIndexPtr), [triggered] "r" (triggeredPtr),
				[ch1] "r" (ch1_samples), [lCtr] "r" (lCtr),
				[nSamp] "r" (length), [half_samples] "r" (length/2), [debug_func] "r" (print_debug), [debug_msg] "r" (msg)
			:"r0", "r1", "r2", "r9", "memory", "cc"
	);

	print_debug("Sampling finished, final index: %d\n", *sIndexPtr);

	Transmit_Frame(ch1_samples, trigger_index);
	print_debug("Transmission Finished!!!\n\r");
}

/*
void sampling_test(void){
	asm volatile(

			"ldr r1, =0x40012400		\n\r"
			"wait_adc:"
			"		ldr r0, [r1, #0]			\n\r" // loading ADC1 SR
			"		lsls r0, r0, #30			\n\r" // left shifting r0 by 30 bits
			"		bpl wait_adc				\n\r"
			""
			"		ldr r0, [r1, #0x4C]			\n\r" // loading ADC1 DR Register
			"		strh r0, [%[raw_adc]]		\n\r"
			"finished:"
			"		ldr r1, =0x40013800			\n\r"
			"		ldrh r2, [%[raw_adc]]		\n\r"
			"		strh r2, [r1, #0x04]					\n\r"
			:
			:[raw_adc] "r" (raw_adc_value_ptr)
			:"r0", "r1", "r2", "memory", "cc"

			);
}*/


