/*
 * game.c
 *
 * Created: 10.11.2018 13:53:19
 *  Author: johangh
 */ 
#define F_CPU 16000000UL
#include <util/delay.h>
#include "ADC.h"
#include <stdint.h>

uint8_t game_over(){
	if (ADC_read() <= 10){
		printf("GAME OVER LAD!!\n");
		//_delay_ms(1000);
		return 1;
	}
	else
	{
		return 0;
	}
}