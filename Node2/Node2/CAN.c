/*
 * CAN.c
 *
 * Created: 17.10.2018 12:24:12
 *  Author: johangh
 */ 
#include "MCP2515.h"
#include "SPI.h"
#include "CAN.h"
#define F_CPU 4915200
#include <util/delay.h>
#define NULL_msg can_msg{.id = 0, .length = 0, data[0] = 0}

void can_init(){
	SPI_MasterInit();
	mcp2515_reset();
	mcp2515_bit_modify(MCP_RXB0CTRL,MCP_RXB0CTRL_RECIEVE_BUFFER_MODE,0xff); // set receive buffer operating mode to 11 -> receive only valid messages with standard identifiers that meet filter criteria.
	mcp2515_bit_modify(MCP_RXB0CTRL,MCP_RXB0CTRL_ROLLOVER,0x00); // no rollover
	mcp2515_bit_modify(MCP_CANINTE,0x01,0xff);
	mcp2515_bit_modify(MCP_CANCTRL,MCP_CANCTRL_SET_OPERATION_MODE,MODE_NORMAL);
}
void can_send(can_msg msg){
	// SIDL og SIDH "identifier bits". SIDH er standard intenifier high, mens SIDL er standard identifier low
	// DLC setter antall bytes som skal sendes. Length må skrives til denne!
	// TXBnDm -> TRANSMIT BUFFER n DATA BYTE m, må loope gjennom å skrive hver byte fra can_msg til data buffer
	while((mcp2515_read(MCP_TXB0CTRL) & MCP_TXBnCTRL_TXREQ)){} // må sjekke om TXB0CTRL.TXREQ = 1, looper helt til den leser en verdi 1 fra kontrollregisteret
	mcp2515_write(MCP_TXB0_SIDH,msg.id << 5);
	mcp2515_write(MCP_TXB0_SIDL,msg.id >> 3);
	mcp2515_write(MCP_TXB0DLC,msg.length);
	for (int i = 0; i < msg.length; i++){
		mcp2515_write(MCP_TXB0Dm+i,msg.data[i]);		
	}
	mcp2515_request_to_send(1);
}

can_msg can_recieve(){
	can_msg msg;
	// set everything to zero 
	msg.id = 0;
	msg.length = 0;
	for(int i = 0; i < 8; i++){
		msg.data[i] = 0;
	}
	while(!(mcp2515_read(MCP_CANINTF) & MCP_RX0IF));
	msg.id = (mcp2515_read(MCP_RXB0_SIDL) >> 5) | (mcp2515_read(MCP_RXB0_SIDH) << 3);
	msg.length = (mcp2515_read(MCP_RXB0_DLC) & (0x0f));
	for (int i = 0; i < msg.length; i++){
		msg.data[i] = mcp2515_read(MCP_RXB0_Dm + i);
	}
	mcp2515_bit_modify(MCP_CANINTF,0x01,0x00); // set RX0IF to zero, ensuring that the MCU has finished with the message before a new message is loaded
	return msg;
}

void print_msg(can_msg msg){
	printf("id: %d\nlength: %d \n",msg.id,msg.length);
	printf("Data:\n{");
	for (int i = 0;i < msg.length; i++){
		printf("%d ",msg.data[i]);
	}
	printf("}\n");
}

void can_test_loopback_2(can_msg msg, can_msg msg2){
	printf("This is the first message to be sent:\n");
	print_msg(msg);
	can_send(msg);
	print_msg(can_recieve(msg));
	printf("\nthe first message is received.");
	printf("\nThis is the second message to be sent:\n");
	print_msg(msg2);
	can_send(msg2);
	print_msg(can_recieve(msg2));
	printf("\nthe second message is received.");
	
}
void can_test_loopback_1(can_msg msg){
	printf("This is the first message to be sent:\n");
	print_msg(msg);
	can_send(msg);
	print_msg(can_recieve());
	printf("\nthe first message is received.");
}
void can_test_normal_mode(){
	printf("we expect the following message from node 1:\n");
	can_msg msg2;
	msg2.id = 0x01;
	msg2.length = 8;
	for (int i = 0; i < msg2.length; i++){
		msg2.data[i] = 0x04*i;
	}
	print_msg(msg2);
	printf("The message we recieved:\n");
	print_msg(can_recieve());
}

//can_msg CAN_USB_package()