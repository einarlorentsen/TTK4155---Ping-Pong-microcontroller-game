/*
 * menu.c
 *
 * Created: 03.10.2018 12:21:21
 *  Author: johangh
 */ 
#include "menu.h"
#include "joystick.h"
#include "oled.h"
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
#define MAX_DEPTH 2
static uint8_t menu_page = 0;
static menu_node* main_menu;
static menu_node* current_menu;




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
	// first submenu
	main_menu->child_menu = menu_create("Play",NULL,0);
	main_menu->child_menu->parent_menu = main_menu;
	// second submenu
	main_menu->child_menu->sibling_right = menu_create("Settings",NULL,1);
	main_menu->child_menu->sibling_right->parent_menu = main_menu;
	main_menu->child_menu->sibling_right->sibling_left = main_menu->child_menu;
	// first submenu of second submenu
	main_menu->child_menu->sibling_right->child_menu= menu_create("Brightness",NULL,0);
	main_menu->child_menu->sibling_right->child_menu->parent_menu = main_menu->child_menu->sibling_right;
	// second submenu of second submenu
	main_menu->child_menu->sibling_right->child_menu->sibling_right = menu_create("Difficulty",NULL,1);
	main_menu->child_menu->sibling_right->child_menu->sibling_right->sibling_left = main_menu->child_menu->sibling_right->child_menu;
	
	// set global current menu
	current_menu = main_menu;
	menu_display(current_menu);
}


void menu_display(menu_node* menu){
	oled_reset();
	if (menu->child_menu == NULL){
		return;
	}
	menu_node* current = menu->child_menu;
	oled_goto_page(0);
	oled_goto_column(0);
	oled_printf("->");
	oled_printf(current->name);
	//printf(current->name);
	uint8_t i = 1;
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
	//printf(current_menu->name);
	printf("current direction: %d \n", find_direction());
	if (find_direction() == UP){
		if (current_menu->sibling_left == NULL){
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
	
	else if (find_direction() == RIGHT || joystick_button()){
		if (current_menu->child_menu == NULL){
			return;
		}
		current_menu = current_menu->child_menu;
		menu_display(current_menu->parent_menu);
	}
	else if (find_direction() == LEFT){
		current_menu = current_menu->parent_menu;
		// hva skjer her? Skulle det ikke vært current_menu? (testet, men funket ikke).
		menu_display(main_menu);
	}
}
menu_node* menu_get_current_menu(){
	return current_menu;
}


