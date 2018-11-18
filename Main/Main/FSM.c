/*
 * FSM.c
 *
 * Created: 18.11.2018 18:11:34
 *  Author: johangh
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
#include "oled.h"
#include "menu.h"
#include "SPI.h"
#include "CAN.h"
#include "MCP2515.h"
#include <math.h>


void FSM(){
	menu_move();
	_delay_ms(100);
	if(menu_get_current_menu()->menu_function != NULL && (joystick_button())){
		_delay_ms(200);
		menu_get_current_menu()->menu_function();
	}
}