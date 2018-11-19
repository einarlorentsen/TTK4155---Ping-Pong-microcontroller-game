/*
 * Main.cpp
 *
 * Created: 29.08.2018 09:27:21
 *  Author: Johangh
 */ 

#define F_CPU 4915200 // clock frequency in Hz

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include "UART.h"
#include "sram.h"
#include "joystick.h"
#include "adc.h"
#include "memory.h"
#include "bit_manipulation.h"
#include "test.h"
#include "oled.h"
#include "menu.h"
#include "SPI.h"
#include "CAN.h"
#include "MCP2515.h"
#include "FSM.h"
#include <math.h>

int main(void){
	cli();
	uart_init();
	memory_init();
	SRAM_test();
	ADC_init();
	oled_init();
	joystick_init();
	oled_reset();
	find_joystick_center();
	can_init();
	menu_init();
	_delay_ms(100);
	can_send(CAN_package(0,get_difficulty()));
	while(1){
		FSM();
	}
}
