

#ifndef MENU_H_
#define MENU_H_
//#include "fonts.h"
#include "oled.h"
#include <stdint.h>
#include <stdlib.h>

typedef enum{
	EASY,
	MEDIUM,
	INSANE,
}GAME_MODE;

typedef struct menu_node menu_node;

struct menu_node{
	char* offset;
	char* name;
	uint8_t page_id;
	menu_node* sibling_left;
	menu_node* sibling_right;
	menu_node* child_menu;
	menu_node* parent_menu;
	void (*menu_function)();
};



menu_node* menu_create(char* name,void *myfunction(void), uint8_t page_nr);
void menu_init();
void menu_display(menu_node* menu);
void menu_move();
menu_node* menu_get_current_menu();
menu_node* get_main_menu();

/* --- MENU NODE FUNCTIONS --- */
void menu_play_pong();
void menu_replay_yes();
void menu_replay_no();
void menu_settings();
#endif /* MENU_H_ */