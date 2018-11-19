/*
 * menu.c
 *
 * Created: 03.10.2018 12:21:21
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
#include "test.h"
#include "oled.h"
#include "menu.h"
#include "SPI.h"
#include "CAN.h"
#include "MCP2515.h"
/*
void scroll_up(void);
void scroll_down(void);
//highlighte pagen vi er på nå
void highlight_page(uint8_t page);
//skrive inn og fjerne selve menyteksten
uint8_t left_button_pressed(void);
uint8_t right_button_pressed(void);
uint8_t joystick_button_pressed(void);

*/
#define MENU_PAGES 5
static uint8_t menu_page = 0;
static menu_node* main_menu;
static menu_node* current_menu;
static uint8_t difficulty = EASY;



menu_node* menu_create(char* name,void *myfunction(void), uint8_t page_nr){
	// allokerer først minne til menyen samt eventuelle undermenyer
	menu_node* menu = malloc(sizeof(menu_node));
	menu->name = name;
	menu->page_id = page_nr;
	menu->sibling_left = NULL;
	menu->sibling_right = NULL;
	menu->child_menu = NULL;
	menu->parent_menu = NULL;
	menu->menu_function = myfunction;
	menu->offset = "  ";
	return menu;
}

void menu_init(){
	main_menu = menu_create("Main Menu",NULL,0);
	
	/* --- play_pong --- */
	
	main_menu->child_menu = menu_create("Play Pong",&menu_play_pong,1);
	menu_node* play_pong = main_menu->child_menu;
	play_pong->parent_menu = main_menu;
	
	/* --- play_pong submenu -> replay? --- */
	
	play_pong->child_menu = menu_create("Replay?",NULL,1);
	menu_node* replay = play_pong->child_menu;
	replay->parent_menu = play_pong;
	
	/* --- replay submenu -> yes! --- */
	
	replay->child_menu = menu_create("Yes!",&menu_replay_yes,1);
	menu_node * yes = replay->child_menu;
	yes->parent_menu = replay;
	
	/* --- replay submenu -> no =( --- */
	
	yes->sibling_right = menu_create("No=(",&menu_replay_no,2);
	menu_node* no = yes->sibling_right;
	no->sibling_left = yes;
	no->parent_menu = replay;	
	
	/*  Difficulty --- */
	
	play_pong->sibling_right = menu_create("Difficulty", &menu_difficulty,2);
	menu_node* difficulty = play_pong->sibling_right;
	difficulty->sibling_left = play_pong;
	difficulty->parent_menu = main_menu;
	
	/* Difficulty submeny -> Easy --- */
	
	difficulty->child_menu = menu_create("Easy", &difficulty_easy,1);
	menu_node* easy = difficulty->child_menu;
	easy->parent_menu = difficulty;
	
	/* Difficulty submeny -> Medium --- */
	
	easy->sibling_right = menu_create("Medium", &difficulty_medium,2);
	menu_node* medium = easy->sibling_right;
	medium->sibling_left = easy;
	medium->parent_menu = difficulty;
	
	/* Difficulty submeny -> Insane --- */
	
	medium->sibling_right = menu_create("Insane", &difficulty_insane,3);
	menu_node* insane = medium->sibling_right;
	insane->sibling_left = medium;
	insane->parent_menu = difficulty;
	
	/* --- set global current menu --- */
	
	current_menu = main_menu;
	menu_display(current_menu);
}


void menu_display(menu_node* menu){
	oled_reset();
	if (menu->child_menu == NULL){
		return;
	}
	menu_node* current = menu->child_menu;
	/* start by printing the menu name */
	oled_goto_page(0);
	oled_goto_column(0);
	oled_printf("---");
	oled_printf(current->parent_menu->name);
	oled_printf("---");
	oled_goto_page(1);
	oled_goto_column(0);
	oled_printf("->");
	oled_printf(current->name);
	//printf(current->name);
	uint8_t i = 2;
	while (current->sibling_right != NULL){
		current = current->sibling_right;
		oled_goto_page(i);
		oled_goto_column(0);
		oled_printf("  ");
		oled_printf(current->name);
		//printf(current->name);
		i++;
	}
	// hva skjer her?
	current_menu = menu->child_menu;
}


void menu_move(){
	if (find_direction() == UP){
		if (current_menu->sibling_left == NULL || current_menu->page_id == 1){
			return;
		}
		oled_goto_page(current_menu->page_id);
		oled_goto_column(0);
		oled_printf(current_menu->offset);
		current_menu = current_menu->sibling_left;
		oled_goto_page(current_menu->page_id);
		oled_goto_column(0);
		oled_printf("->");
	}
	else if (find_direction() == DOWN){
		if (current_menu->sibling_right == NULL){
			return;
		}
		oled_goto_page(current_menu->page_id);
		oled_goto_column(0);
		oled_printf(current_menu->offset);
		current_menu = current_menu->sibling_right;
		oled_goto_page(current_menu->page_id);
		oled_goto_column(0);
		oled_printf("->");
	}
	/*
	else if (find_direction() == RIGHT || joystick_button()){
		if (current_menu->child_menu == NULL){
			return;
		}
		current_menu = current_menu->child_menu;
		menu_display(current_menu->parent_menu);
	}
	*/
	else if (find_direction() == LEFT){
		if (current_menu->name == main_menu->name){
			//
		}
		else{
			current_menu = current_menu->parent_menu;
			// hva skjer her? Skulle det ikke vært current_menu? (testet, men funket ikke).
			menu_display(main_menu);
		}
	}
}

menu_node* menu_get_current_menu(){
	return current_menu;
}
menu_node* get_main_menu(){
	return main_menu;
}

/* --------- MENU NODE FUNCTIONS ------------- */

void menu_play_pong(){
	current_menu = main_menu->child_menu;
	oled_reset();
	oled_goto_page(2);
	oled_goto_column(0);
	oled_printf("   Good luck");
	oled_goto_page(3);
	oled_goto_column(0);
	oled_printf("   have fun!");
	while(1){
		_delay_ms(100);
		can_send(CAN_package(1,get_difficulty()));
		if(can_recieve().data[0] == 1){
			oled_reset();
			oled_goto_column(0);
			oled_goto_page(3);
			oled_printf("-- Game over --");
			can_send(CAN_package(0,get_difficulty()));
			_delay_ms(4000);
			current_menu = current_menu->child_menu;
			menu_display(current_menu);
			break;
		}
	}
}
void menu_replay_yes(){
	current_menu = main_menu->child_menu;
	current_menu->menu_function();
}
void menu_replay_no(){
	oled_reset();
	current_menu = main_menu->child_menu;
	menu_display(main_menu);
}

void menu_difficulty(){
	oled_reset();
	menu_display(main_menu->child_menu->sibling_right);
}


void difficulty_easy(){
	difficulty = EASY;
	menu_display(main_menu);
}

void difficulty_medium(){
	difficulty = MEDIUM;
	menu_display(main_menu);
}

void difficulty_insane(){
	difficulty = INSANE;
	menu_display(main_menu);
}

uint8_t get_difficulty(){
	return difficulty;
}
