#include "bit_manipulation.h"
#include "slider.h"
#include <avr/io.h>

void slider_init(void) {
	// set the button pins to input
	clear_bit(DDRB,DDB0);
	clear_bit(DDRB,DDB1);
}

uint8_t left_slider_button(void){
	if ((PINB & (1 << PINB1))){
		return 1;
	}
	else {
		return 0;
	}
}

uint8_t right_slider_button(void){
	if ((PINB & (1 << PINB0))){
		return 1;
	}
	else {
		return 0;
	}
}

slider_position find_slider_position(void){
	slider_position sp;
	sp.left = ADC_read(CH_LEFT_SLIDER);
	sp.right = ADC_read(CH_RIGHT_SLIDER);
	return sp;
}
void slider_test(){
	slider_position pos = find_slider_position();
	printf("Left slider position: %d\n",pos.left);
	printf("Right slider position: %d\n",pos.right);
	if (left_slider_button()){
		printf("Left button pressed!!\n");
	}
	if (right_slider_button()){
		printf("Right button pressed!!\n");
	}
}