/*
 * main.c
 *
 * Created: 05.11.2018 11:27:46
 *  Author: einarlor
 */ 
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "CAN.h"
#include "MCP2515.h"
#include "SPI.h"
#include "UART.h"
#include "bit_manipulation.h"
#include "PWM.h"
#include "ADC.h"
#include "game.h"
#include "solenoid_driver.h"
#include "DAC.h"
#include "motor.h"
#include "PD.h"

int main(){
	/* ---------------------- DRIVER INIT ---------------------- */
	uart_init();
	mcp2515_init();
	can_init();
	timer_init();
	PWM_init();
	ADC_init();
	solenoid_init();
	DAC_init();
	printf("test print");
	motor_init();
	motor_calibration();
	PD_init();
	_delay_ms(1000);
	
	/* ---------------------- RUN GAME ---------------------- */
	
	while (1){
		can_msg msg_usb_board = can_recieve();
		PWM_set_pulse(PWM_joystick_to_PWM(msg_usb_board.data[JOYSTICK_X]));
		//printf("The slider position: %d\n",msg_usb_board.data[SLIDER_LEFT]);
		solenoid_hit(msg_usb_board.data[SLIDER_BUTTON_RIGHT]);
		//printf("This should be the digital signal of IR: %d\n",ADC_read());
		PD_set_position_reference(msg_usb_board.data[SLIDER_RIGHT]);
	}
	
}
