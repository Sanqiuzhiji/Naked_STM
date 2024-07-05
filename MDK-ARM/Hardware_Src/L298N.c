#include "L298N.h"
#include "usart.h"
#include <string.h> // for strcmp

Motor motor[4];

void Motor_Init(Motor motor)
{
    HAL_TIM_Encoder_Start(&motor.encodingTIM_HandleTypeDef, TIM_CHANNEL_ALL); // 开启编码器定时器
    __HAL_TIM_ENABLE_IT(&motor.encodingTIM_HandleTypeDef, TIM_IT_UPDATE);     // 开启编码器定时器更新中断,防溢出处理
    HAL_TIM_Base_Start_IT(&htim6);                                            // 开启100ms定时器中断
    HAL_TIM_PWM_Start(&motor.pwmTIM_HandleTypeDef, motor.pwmTIM_CHANNEL);     // 开启PWM
    __HAL_TIM_SET_COUNTER(&motor.encodingTIM_HandleTypeDef, 30000);           // 编码器定时器初始值设定为30000
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
    L298N_SetSpeed("b1", Speed * 75 / 100);
    L298N_SetSpeed("b2", Speed * 75 / 100);
}

void SpinTurnLeft(uint16_t Speed)
{
    L298N_SetMode("a1", "right");
    L298N_SetMode("a2", "left");
    L298N_SetMode("b1", "left");
    L298N_SetMode("b2", "right");

    L298N_SetSpeed("a1", Speed * 75 / 100);
    L298N_SetSpeed("a2", Speed * 75 / 100);
    L298N_SetSpeed("b1", Speed);
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

// PID初始化函数
void PID_Init(PID *pid, float p, float i, float d, float maxI, float maxOut)
{
    pid->kp = p;
    pid->ki = i;
    pid->kd = d;
    pid->maxIntegral = maxI;
    pid->maxOutput = maxOut;
}
// 单级PID计算，需要传入的参数，PID结构体，目标值，反馈值
void PID_SingleCalc(PID *pid, float reference, float feedback)
{
    pid->lastError = pid->error;       // 更新上一次的误差
    pid->error = reference - feedback; // 更新当前误差
    // 下面分别是P，I，D的计算
    pid->output = pid->error *pid->kp  ;                      // P为根据当前误差计算输出量
    pid->integral += pid->error * pid->ki;                     // I为累计误差的输出量
    LIMIT(pid->integral, -pid->maxIntegral, pid->maxIntegral); // 限制I的输出，抑制超调
    pid->output += pid->integral;
    pid->output += (pid->error - pid->lastError) * pid->kd; // D以当前误差减去上次误差作为微分环节
    LIMIT(pid->output, -pid->maxOutput, pid->maxOutput);   // 限制PID总输出
}

// 电机发送指令
void Motor_Send(Motor motor)
{
    PID_SingleCalc(&motor.pid, motor.targetSpeed, motor.speed);
    // 进行PID计算，传入我们设定的目标值targetSpeed，和我们上篇文章处理编码器值得到的speed值
    float output = motor.pid.output;
    Serial_Printf("pwm%f\r\n", output+200);
    if (output > 0) // 对应正转
    {
        L298N_SetSpeed("a1", (uint32_t)output+200);
        L298N_SetMode("a1", "right");
    }
    else // 对应反转
    {
        L298N_SetSpeed("a1", (uint32_t)(-output-200));
        L298N_SetMode("a1", "left");
    }
}
