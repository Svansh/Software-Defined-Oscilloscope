#include "frame.h"
#include "peripheral.h"
#include "parameters.h"

extern uint8_t keepSampling;

Frame_t tx_frame1;
Frame_t tx_frame2;
Frame_t* current_frame = &tx_frame1;


uint8_t calc_checksum(Frame_t* f1){
	CRC_init();
	// Now CRC->DR is at 0xFFFF FFFF
	uint16_t i;
	for(i = 0; i < f1->length; i++){
		CRC->DR = f1->data_array[i];
	}

	f1->checksum = CRC->DR;

	return 1;
}


void Start_DMA_Transmission(Frame_t* f1){
	DMA1_Channel4->CCR &= ~(DMA_CCR4_EN);
	DMA1_Channel4->CMAR = (uint32_t)f1;
	DMA1_Channel4->CNDTR = sizeof(Frame_t);

	DMA1_Channel4->CCR |= DMA_CCR4_EN;
}


void Transmit_Frame(uint16_t* samples, uint16_t trigger_index){
	if(keepSampling){
		current_frame->header = 0xAA55;
		current_frame->footer = 0x55AA;
		current_frame->length = NUM_SAMPLES;
		current_frame->trigger_index = 32;
		calc_checksum(current_frame);

		for(uint16_t i = 0; i < current_frame->length; i++){
			current_frame->data_array[i] = samples[i];
		}
		Start_DMA_Transmission(current_frame);

		current_frame = (current_frame == &tx_frame1) ? &tx_frame2: &tx_frame1;
	}
}







