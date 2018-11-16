/*
 * PWM.c
 *
 * Created: 07.11.2018 22:41:22
 *  Author: johangh
 */ 
// INVERTED = 1 when the servo is inverted
#define INVERTED 1
#define F_CPU 16000000
#define PERIOD 0.02
#define PRESCALER 64
#define OFFSET 5
#include "bit_manipulation.h"
#include <avr/io.h>


void PWM_init(){
	//setter prescaler til 64. Dette er fordi vi må delaye med opp til 20 ms på servoen. Uten prescaler ville timer count blitt 319999
	//Dette er utenfor 16-bit timeren sin rekkevidde siden den kun kan telle til 65535. Hvis vi deler klokkefrekvens på 64 så kommer vi på 4999
	//Det klarer 16 bit timeren å telle til
	set_bit(TCCR1B,CS10);
	set_bit(TCCR1B,CS11);
	clear_bit(TCCR1B,CS12);
	//Setter TOP-verdien til TCNT lik en periode. Ser fra tabell 17.2
	ICR1 = (F_CPU/PRESCALER) * PERIOD;
	//setter PB5 til output
	set_bit(DDRB,PB5);
	PWM_set_pulse(1.5);
}

void PWM_set_pulse(float ms){
	if (ms < 0.9){
		ms = 0.9;
	}
	else if (ms > 2.1){
		ms = 2.1;
	}
	OCR1A = (F_CPU/64) * ms/1000;
}


float PWM_joystick_to_PWM(uint8_t position){
	if (INVERTED){
		position = 255-position;
	}
	return (0.9 + (2.0/425)*position);
}
