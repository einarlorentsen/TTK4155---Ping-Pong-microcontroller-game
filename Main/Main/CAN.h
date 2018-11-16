/*
 * CAN.h
 *
 * Created: 17.10.2018 12:24:28
 *  Author: johangh
 */ 


typedef struct{
	uint8_t id;
	uint8_t length;
	uint8_t data[8];
}can_msg;

typedef enum{
	JOYSTICK_X,
	JOYSTICK_Y,
	JOYSTICK_BUTTON,
	SLIDER_LEFT,
	SLIDER_RIGHT,
	SLIDER_BUTTON_LEFT,
	SLIDER_BUTTON_RIGHT,
	NA
}USB_PACKAGE;

void can_init();

void can_send(can_msg msg);

can_msg can_recieve();

void print_msg(can_msg msg);

void can_test_loopback_2(can_msg msg, can_msg msg2);

void can_test_loopback_1(can_msg msg);
// CAN_package: values from the USB board

can_msg CAN_package(void);

