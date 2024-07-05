#include "L298N.h"
#include "usart.h"
#include <string.h> // for strcmp

Motor motor[4];

void Motor_Init(Motor motor)
{
    HAL_TIM_Encoder_Start(&motor.encodingTIM_HandleTypeDef, TIM_CHANNEL_ALL); // ������������ʱ��
    __HAL_TIM_ENABLE_IT(&motor.encodingTIM_HandleTypeDef, TIM_IT_UPDATE);     // ������������ʱ�������ж�,���������
    HAL_TIM_Base_Start_IT(&htim6);                                            // ����100ms��ʱ���ж�
    HAL_TIM_PWM_Start(&motor.pwmTIM_HandleTypeDef, motor.pwmTIM_CHANNEL);     // ����PWM
    __HAL_TIM_SET_COUNTER(&motor.encodingTIM_HandleTypeDef, 30000);           // ��������ʱ����ʼֵ�趨Ϊ30000
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

// PID��ʼ������
void PID_Init(PID *pid, float p, float i, float d, float maxI, float maxOut)
{
    pid->kp = p;
    pid->ki = i;
    pid->kd = d;
    pid->maxIntegral = maxI;
    pid->maxOutput = maxOut;
}
// ����PID���㣬��Ҫ����Ĳ�����PID�ṹ�壬Ŀ��ֵ������ֵ
void PID_SingleCalc(PID *pid, float reference, float feedback)
{
    pid->lastError = pid->error;       // ������һ�ε����
    pid->error = reference - feedback; // ���µ�ǰ���
    // ����ֱ���P��I��D�ļ���
    pid->output = pid->error *pid->kp  ;                      // PΪ���ݵ�ǰ�����������
    pid->integral += pid->error * pid->ki;                     // IΪ�ۼ����������
    LIMIT(pid->integral, -pid->maxIntegral, pid->maxIntegral); // ����I����������Ƴ���
    pid->output += pid->integral;
    pid->output += (pid->error - pid->lastError) * pid->kd; // D�Ե�ǰ����ȥ�ϴ������Ϊ΢�ֻ���
    LIMIT(pid->output, -pid->maxOutput, pid->maxOutput);   // ����PID�����
}

// �������ָ��
void Motor_Send(Motor motor)
{
    PID_SingleCalc(&motor.pid, motor.targetSpeed, motor.speed);
    // ����PID���㣬���������趨��Ŀ��ֵtargetSpeed����������ƪ���´��������ֵ�õ���speedֵ
    float output = motor.pid.output;
    Serial_Printf("pwm%f\r\n", output+200);
    if (output > 0) // ��Ӧ��ת
    {
        L298N_SetSpeed("a1", (uint32_t)output+200);
        L298N_SetMode("a1", "right");
    }
    else // ��Ӧ��ת
    {
        L298N_SetSpeed("a1", (uint32_t)(-output-200));
        L298N_SetMode("a1", "left");
    }
}
