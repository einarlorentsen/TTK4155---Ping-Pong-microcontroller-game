#define MAX_VOLTAGE 5
#include "adc.h"
#include <stdint.h>
#pragma once

typedef enum
{
	LEFT = 0,
	RIGHT = 1,
	UP = 2,
	DOWN = 3,
	NEUTRAL = 4
}DIRECTION;

void joystick_init(void);

void find_joystick_center(void);

position find_position(void);

void print_position();

DIRECTION find_direction(void);

int find_degree(void);

uint8_t joystick_button(void);


