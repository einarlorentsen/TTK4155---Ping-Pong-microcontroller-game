/*
 * timer.c
 *
 * Created: 07.11.2018 20:04:35
 *  Author: johangh
 */ 
#include "timer.h"
#include "bit_manipulation.h"

#include <avr/io.h>

 void timer_init(){
	 //mode14 fra tabell 17.2
	 //Setter her fast-PWM
	 clear_bit(TCCR1A,WGM10);
	 set_bit(TCCR1A,WGM11);
	 set_bit(TCCR1B,WGM12);
	 set_bit(TCCR1B,WGM13); 
	 //Setter ouptput compare. Dette er et flagg som blir satt når TCNT og OCR matcher. Output compare register(OCR) blir kun satt høy ved en writefunksjon.
	 //TCNT blir automatisk oppdater
	 clear_bit(TCCR1A,COM1A0);
	 set_bit(TCCR1A,COM1A1); 
 }
