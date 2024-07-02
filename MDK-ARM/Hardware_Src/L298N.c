#include "L298N.h"

void L298N_Pin_Init(void)
{
	// PWM通道初始化，逻辑功能IO口初始化。
}
void L298N_PWM_Enable(void)
{
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
}
void L298N_SetMode(char Motorname[], char Modename[])
{
	uint16_t PreAGpio1,PreAGpio2;
	if (Motorname == "A1" || Motorname == "a1")
	{
		PreAGpio1 = L298NA_A11_Pin;
		PreAGpio2 = L298NA_A12_Pin;
	}
	else if (Motorname == "A2" || Motorname == "a2")
	{
		PreAGpio1 = L298NA_A21_Pin;
		PreAGpio2 = L298NA_A22_Pin;
	}
	else if (Motorname == "B1" || Motorname == "b1")
	{
		PreAGpio1 = L298NA_B11_Pin;
		PreAGpio2 = L298NA_B12_Pin;
	}
	else if (Motorname == "B2" || Motorname == "b2")
	{
		PreAGpio1 = L298NA_B21_Pin;
		PreAGpio2 = L298NA_B22_Pin;
	}

	if (Modename == "stop")
	{
		HAL_GPIO_WritePin(GPIOD,PreAGpio1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,PreAGpio2,GPIO_PIN_RESET);
	}
	else if (Modename == "right")
	{
		HAL_GPIO_WritePin(GPIOD,PreAGpio1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD,PreAGpio2,GPIO_PIN_SET);
	}
	else if (Modename == "left")
	{
		HAL_GPIO_WritePin(GPIOD,PreAGpio1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD,PreAGpio2,GPIO_PIN_RESET);
	}
}
void L298N_SetSpeed(char Motorname[], uint16_t Speed)
{
	TIM_HandleTypeDef Prehtim;
	uint32_t PreChannel;
	if (Motorname == "A1" || Motorname == "a1")
	{
		Prehtim = htim2;
		PreChannel = TIM_CHANNEL_2;
	}
	else if (Motorname == "A2" || Motorname == "a2")
	{
		Prehtim = htim3;
		PreChannel = TIM_CHANNEL_2;
	}
	else if (Motorname == "B1" || Motorname == "b1")
	{
		Prehtim = htim4;
		PreChannel = TIM_CHANNEL_2;
	}
	else if (Motorname == "B2" || Motorname == "b2")
	{
		Prehtim = htim4;
		PreChannel = TIM_CHANNEL_4;
	}
	__HAL_TIM_SetCompare(&Prehtim, PreChannel, Speed);
}
void GoStraight(uint16_t Speed)
{
	L298N_SetMode("a1","left");
	L298N_SetMode("a2","right");
	L298N_SetMode("b1","right");
	L298N_SetMode("b2","left");

	L298N_SetSpeed("a1",Speed);
	L298N_SetSpeed("a2",Speed);
	L298N_SetSpeed("b1",Speed);
	L298N_SetSpeed("b2",Speed);
}
void GoBack(uint16_t Speed)
{
	L298N_SetMode("a1","right");
	L298N_SetMode("a2","left");
	L298N_SetMode("b1","left");
	L298N_SetMode("b2","right");

	L298N_SetSpeed("a1",Speed);
	L298N_SetSpeed("a2",Speed);
	L298N_SetSpeed("b1",Speed);
	L298N_SetSpeed("b2",Speed);
}
void SpinTurnRight(uint16_t Speed)
{
	L298N_SetMode("a1","left");
	L298N_SetMode("a2","left");
	L298N_SetMode("b1","left");
	L298N_SetMode("b2","left");

	L298N_SetSpeed("a1",Speed);
	L298N_SetSpeed("a2",Speed);
	L298N_SetSpeed("b1",Speed);
	L298N_SetSpeed("b2",Speed);
}
void SpinTurnLeft(uint16_t Speed)
{
	L298N_SetMode("a1","right");
	L298N_SetMode("a2","right");
	L298N_SetMode("b1","right");
	L298N_SetMode("b2","right");

	L298N_SetSpeed("a1",Speed);
	L298N_SetSpeed("a2",Speed);
	L298N_SetSpeed("b1",Speed);
	L298N_SetSpeed("b2",Speed);
}
void GoRight(uint16_t Speed)
{
	L298N_SetMode("a1","left");
	L298N_SetMode("a2","right");
	L298N_SetMode("b1","left");
	L298N_SetMode("b2","right");

	L298N_SetSpeed("a1",Speed);
	L298N_SetSpeed("a2",Speed);
	L298N_SetSpeed("b1",Speed);
	L298N_SetSpeed("b2",Speed);
}
void GoLeft(uint16_t Speed)
{
	L298N_SetMode("a1","right");
	L298N_SetMode("a2","left");
	L298N_SetMode("b1","right");
	L298N_SetMode("b2","left");

	L298N_SetSpeed("a1",Speed);
	L298N_SetSpeed("a2",Speed);
	L298N_SetSpeed("b1",Speed);
	L298N_SetSpeed("b2",Speed);
}
