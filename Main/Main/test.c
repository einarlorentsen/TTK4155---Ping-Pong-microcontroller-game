#include "adc.h"
#include "bit_manipulation.h"
#include <avr/io.h>
#include "memory.h"
#include "joystick.h"
#include "slider.h"
#define F_CPU 4915200
#include <util/delay.h>

void ADC_test(void) {
	int degree;
	find_joystick_center();
	DIRECTION dir;
	position current;
	slider_position current_slider;
	uint8_t left_button;
	uint8_t right_button;
	while(1){
		// code for decode testing
		/*
		address_sram[0] = 0xFF;
		_delay_ms(500);
		address_adc[0] = 0xFF;
		_delay_ms(500);
		*/
		if ((PINB & (1 << PINB1))) {
			printf("HØY_høyreknapp \n");
		}
		if ((PINB & (1 << PINB0))) {
			printf("HØY_venstreknapp \n");
		}
		current = find_position();
		degree = find_degree();
		dir = find_direction();
		current_slider = find_slider_position();
		left_button = left_slider_button();
		right_button = right_slider_button();
		printf("Joystick position: \n");
		printf("x_value: %d",current.x);
		printf("\n");
		printf("y_value: %d",current.y);
		printf("\n");
		printf("Slider positions: \n");
		printf("left slider: %d",current_slider.left);
		printf("\n");
		printf("right slider: %d",current_slider.right);
		printf("\n");
		printf("Left button: %d", left_button);
		printf("\n");
		printf("right button: %d", right_button);
		printf("\n");
		printf("degree: %d",degree);
		printf("\n");
		printf("Direction: %d",dir);
		printf("\n");
		_delay_ms(1000);
	}
}

