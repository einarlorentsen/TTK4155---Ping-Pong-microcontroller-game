/*
 * PID.c
 *
 * Created: 15.11.2018 16:43:55
 *  Author: einarlor
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/interrupt.h>
#include <avr/io.h>
#include "bit_manipulation.h"
#include "ADC.h"
#include "motor.h"
#include "PD.h"
#define MAX_SPEED 150
uint8_t reference = 127;
int16_t position = 0;
int16_t previous_error = 0;
double dt = 0.016;
double Kp = 4;
double Kd = 0.0002;
int16_t lower_limit;
int16_t upper_limit;
double range;

void PD_init(){
	//Vil bruke timer 2 for å skrive til en 8 bits adresse.
	//Dette er for å få en oppdatering på PD-regulatoren hvert 16ms => 1/(16 000 000 / 1024 (prescaler) / 256 (8 bits))
	cli();
	//Setter interrupt enable ved overflow på timer 2
	set_bit(TIMSK2,TOIE2);
	
	//Velger timer 2 og setter prescaler til 1024
	set_bit(TCCR2B,CS20);
	set_bit(TCCR2B,CS21);
	set_bit(TCCR2B,CS22);
	
	//Det sendes nå en interruptvektor hvert 61 ms.
	//motor_calibration må kjøres før PD_init.
	lower_limit = motor_get_min_encoder();
	upper_limit = motor_get_max_encoder();
	range = upper_limit-lower_limit;
	sei();
}

int16_t PD_set_position_reference(int16_t ref){
	//referanse (hvor fingeren er på slideren)
	reference = ref;
}

ISR(TIMER2_OVF_vect){
	//P-regulator
	// This code is motor-depended. If the friction is too high, set max reference lower than 255
	// If the 
	/*
	if (reference > 230){
		reference = 255;
	}
	*/
	position = motor_read_encoder();
	int16_t scaled_pos = (position/range)*(255);
	int16_t error = reference - scaled_pos;
	int16_t actuation = Kp * error + (Kd * (error - previous_error))/dt;
	
	if (error > 0){
		motor_direction(RIGHT);
		if (actuation < MAX_SPEED){
			motor_speed((uint8_t)(actuation));
		}
		else{
			motor_speed((uint8_t)MAX_SPEED);
		}
	}
	else if(error <= 0){
		motor_direction(LEFT);
		if (-actuation < MAX_SPEED){
			motor_speed((uint8_t)(-actuation));
		}
		else {
			motor_speed((uint8_t)(MAX_SPEED));
		}
	}
	previous_error = error;
	//printf("Upper limit -> should be over position: %d\n",upper_limit);
	//printf("Error -> should be ~= 128: %d\n",(uint8_t)(-actuation));
	set_bit(TIFR2,TOV2);
}

void PD_set_game_mode(uint8_t difficulty){
	switch(difficulty) {
		
		case EASY:
			Kp = 4;
			Kd = 0.0002;
			break;
		
		case MEDIUM:
			Kp = 6;
			Kd = 0.02;
			break; 
			
		case INSANE:
			Kp = 8;
			Kd = 0.2;
			break;
			
		default: 
			Kp = 4;
			Kd = 0.0002;
			break;
	}
	
}
