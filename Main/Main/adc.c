
#include "adc.h"
#include "bit_manipulation.h"
#include <avr/io.h>
#include "memory.h"
#include "joystick.h"
#include "slider.h"
#define F_CPU 4915200
#include <util/delay.h>

void ADC_init(void){
	clear_bit(DDRB,DDB3);
}
uint8_t ADC_read(ADC_CHANNEL ch){
	*address_adc = ch;
	loop_until_bit_is_clear(PINB,PINB3);
	return *address_adc;
}
