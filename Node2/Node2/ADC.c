/*
 * ADC.c
 *
 * Created: 09.11.2018 14:54:21
 *  Author: einarlor
 */ 

#include <avr/io.h>
#include <stdint.h>
#include "bit_manipulation.h"

void ADC_init(){
	//Enable ADC
	set_bit(ADCSRA,ADEN);
	//Left adjust to only read from ADCH. (Page 286, hmm?)
	set_bit(ADMUX,ADLAR);
	//set clock to between 50 KHz and 200 KHz. From page 271 datasheet
	set_bit(ADCSRA,ADPS0);
	set_bit(ADCSRA,ADPS1);
	set_bit(ADCSRA,ADPS2);
	//set PF0 as input. This is pin A0 on shield. On ADCL on node2 schematic
	clear_bit(DDRF,PF0);
	//set AREF voltage at 2.56 V
	clear_bit(ADMUX,REFS0);
	set_bit(ADMUX,REFS1);
	
}

uint8_t ADC_read(){
	//Start conversion
	set_bit(ADCSRA,ADSC);
	//loops until data is ready in the registers
	loop_until_bit_is_set(ADCSRA,ADIF);
	uint8_t data = ADCH;
	return data;
}
