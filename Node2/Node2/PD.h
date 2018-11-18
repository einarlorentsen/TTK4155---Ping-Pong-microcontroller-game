/*
 * PD.h
 *
 * Created: 15.11.2018 16:44:08
 *  Author: einarlor
 */ 


#ifndef PD_H_
#define PD_H_
typedef enum{
	EASY,
	MEDIUM,
	INSANE,
}DIFFICULTY;


void PD_init();

int16_t PD_set_position_reference(int16_t ref);




#endif /* PD_H_ */