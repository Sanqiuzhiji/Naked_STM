#include "L298N.h"
#include "usart.h"
#include <string.h> // for strcmp
void L298N_Pin_Init(void)
{
    // PWM通道初始化，逻辑功能IO口初始化。
}
void L298N_PWM_Enable(void)
{
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
}

void L298N_SetMode(const char *Motorname, const char *Modename)
{
    uint16_t PreAGpio1 = 0, PreAGpio2 = 0;

    if (strcmp(Motorname, "A1") == 0 || strcmp(Motorname, "a1") == 0)
    {
        PreAGpio1 = INA11_Pin;
        PreAGpio2 = INA12_Pin;
    }
    else if (strcmp(Motorname, "A2") == 0 || strcmp(Motorname, "a2") == 0)
    {

        PreAGpio1 = INA21_Pin;
        PreAGpio2 = INA22_Pin;
    }
    else if (strcmp(Motorname, "B1") == 0 || strcmp(Motorname, "b1") == 0)
    {
        PreAGpio1 = INB11_Pin;
        PreAGpio2 = INB12_Pin;
    }
    else if (strcmp(Motorname, "B2") == 0 || strcmp(Motorname, "b2") == 0)
    {
        PreAGpio1 = INB21_Pin;
        PreAGpio2 = INB22_Pin;
    }

    if (strcmp(Modename, "stop") == 0)
    {
        HAL_GPIO_WritePin(GPIOD, PreAGpio1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, PreAGpio2, GPIO_PIN_RESET);
    }
    else if (strcmp(Modename, "right") == 0)
    {
        HAL_GPIO_WritePin(GPIOD, PreAGpio1, GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, PreAGpio2, GPIO_PIN_RESET);
    }
    else if (strcmp(Modename, "left") == 0)
    {
        HAL_GPIO_WritePin(GPIOD, PreAGpio1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, PreAGpio2, GPIO_PIN_SET);
    }
}

void L298N_SetSpeed(const char *Motorname, uint16_t Speed)
{
    TIM_HandleTypeDef Prehtim = {0};
    uint32_t PreChannel = 0;

    if (strcmp(Motorname, "A1") == 0 || strcmp(Motorname, "a1") == 0)
    {
        Prehtim = htim4;
        PreChannel = TIM_CHANNEL_1;
    }
    else if (strcmp(Motorname, "A2") == 0 || strcmp(Motorname, "a2") == 0)
    {
        Prehtim = htim15;
        PreChannel = TIM_CHANNEL_1;
    }
    else if (strcmp(Motorname, "B1") == 0 || strcmp(Motorname, "b1") == 0)
    {
        Prehtim = htim15;
        PreChannel = TIM_CHANNEL_2;
    }
    else if (strcmp(Motorname, "B2") == 0 || strcmp(Motorname, "b2") == 0)
    {
        Prehtim = htim4;
        PreChannel = TIM_CHANNEL_2;
    }

    __HAL_TIM_SetCompare(&Prehtim, PreChannel, Speed);
}

void GoStraight(uint16_t Speed)
{
    L298N_SetMode("a1", "right");
    L298N_SetMode("a2", "left");
    L298N_SetMode("b1", "left");
    L298N_SetMode("b2", "right");

    L298N_SetSpeed("a1", Speed);
    L298N_SetSpeed("a2", Speed);
    L298N_SetSpeed("b1", Speed);
    L298N_SetSpeed("b2", Speed);
}

void GoBack(uint16_t Speed)
{
    L298N_SetMode("a1", "left");
    L298N_SetMode("a2", "right");
    L298N_SetMode("b1", "right");
    L298N_SetMode("b2", "left");

    L298N_SetSpeed("a1", Speed);
    L298N_SetSpeed("a2", Speed);
    L298N_SetSpeed("b1", Speed);
    L298N_SetSpeed("b2", Speed);
}

void SpinTurnRight(uint16_t Speed)
{
    L298N_SetMode("a1", "right");
    L298N_SetMode("a2", "left");
    L298N_SetMode("b1", "left");
    L298N_SetMode("b2", "right");

    L298N_SetSpeed("a1", Speed);
    L298N_SetSpeed("a2", Speed);
    L298N_SetSpeed("b1", Speed* 75 / 100);
    L298N_SetSpeed("b2", Speed * 75 / 100);
}

void SpinTurnLeft(uint16_t Speed)
{
    L298N_SetMode("a1", "right");
    L298N_SetMode("a2", "left");
    L298N_SetMode("b1", "left");
    L298N_SetMode("b2", "right");

    L298N_SetSpeed("a1", Speed* 75 / 100);
    L298N_SetSpeed("a2", Speed* 75 / 100 );
    L298N_SetSpeed("b1", Speed );
    L298N_SetSpeed("b2", Speed);
}

void GoRight(uint16_t Speed)
{
    L298N_SetMode("a1", "left");
    L298N_SetMode("a2", "right");
    L298N_SetMode("b1", "left");
    L298N_SetMode("b2", "right");

    L298N_SetSpeed("a1", Speed);
    L298N_SetSpeed("a2", Speed);
    L298N_SetSpeed("b1", Speed);
    L298N_SetSpeed("b2", Speed);
}

void GoLeft(uint16_t Speed)
{
    L298N_SetMode("a1", "right");
    L298N_SetMode("a2", "left");
    L298N_SetMode("b1", "right");
    L298N_SetMode("b2", "left");

    L298N_SetSpeed("a1", Speed);
    L298N_SetSpeed("a2", Speed);
    L298N_SetSpeed("b1", Speed);
    L298N_SetSpeed("b2", Speed);
}
