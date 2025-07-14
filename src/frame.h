/*
 * frame.h
 *
 *  Created on: 07-Jul-2025
 *      Author: Svansh
 */

#ifndef FRAME_H_
#define FRAME_H_

#include <stdint.h>

typedef struct {
	uint16_t header;
	uint16_t length;
	uint16_t trigger_index;
	uint16_t data_array[256];
	uint32_t checksum;
	uint16_t footer;

} Frame_t;


uint8_t calc_checksum(Frame_t*);
void Start_DMA_Transmission(Frame_t*);
void Transmit_Frame(uint16_t*, uint16_t);



#endif /* FRAME_H_ */
