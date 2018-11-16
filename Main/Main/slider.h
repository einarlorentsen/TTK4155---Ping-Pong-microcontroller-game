#include <stdint.h>
#include "adc.h"

typedef struct {
	uint8_t left;
	uint8_t right;
	}slider_position;


void slider_init(void);

uint8_t left_slider_button(void);

uint8_t right_slider_button(void);

slider_position find_slider_position(void);

void slider_test();