#ifndef __L298N_H
#define __L298N_H

#include "main.h"
#include "tim.h"

void L298N_Pin_Init(void);
void L298N_PWM_Enable(void);
void L298N_SetSpeed(char Motorname[], uint16_t Speed);
void L298N_SetMode(char Motorname[], char Modename[]);

#endif
