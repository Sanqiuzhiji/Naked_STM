#include "main.h"

void L298NM_Pin_Init(void)
{
	
}
void Servo_SetAngle(float Angle)
{
	TIM_SetCompare1(TIM2,Angle / 180 * 2000 + 500);
}

