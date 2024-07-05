#ifndef __L298N_H
#define __L298N_H

#include "main.h"

#define RR 30u    //������ٱ�

#define RELOADVALUE(motor) __HAL_TIM_GetAutoreload(&motor.encodingTIM_HandleTypeDef)    //��ȡ�Զ�װ��ֵ,������Ϊ20000
#define COUNTERNUM(motor) __HAL_TIM_GetCounter(&motor.encodingTIM_HandleTypeDef)       //��ȡ��������ʱ���еļ���ֵ


typedef struct _PID
{
	float kp,ki,kd;               //PID����������
	float error,lastError;        //��ǰ������һ�ε����
	float integral,maxIntegral;   //�������ͻ��ֵ��޷�
	float output,maxOutput;       //PID���������PID����������
}PID;

//����ṹ��
typedef struct _Motor
{
    TIM_HandleTypeDef encodingTIM_HandleTypeDef;
    TIM_HandleTypeDef pwmTIM_HandleTypeDef;    
    uint32_t pwmTIM_CHANNEL;
	int32_t lastAngle;        //��10msת���ĽǶ�
	int32_t totalAngle;       //�ܵĽǶ�
	int16_t loopNum;          //�����������ֵ
	float speed;              //��������Ŀǰת��,��λΪRPM
	float targetSpeed;       //����趨��Ŀ���ٶ�
	PID pid;                 //��ӵ����ӦPID
}Motor;

#include"main.h"

#define LIMIT(x,min,max) (x)=(((x)<=(min))?(min):(((x)>=(max))?(max):(x)))  //�޷�����

// typedef struct _CascadePID
// {
// 	PID inner;    // �ڻ��ٶȻ�PID
// 	PID outer;    // �⻷�ǶȻ�PID
// 	float output;
// };

// typedef struct _Motor
// {
// 	int32_t lastAngle;       //�ϴμ�������ʱת���ĽǶ�
// 	int32_t totalAngle;      //�ܹ�ת���ĽǶ�
// 	int16_t loopNum;         //��������������
// 	float speed;             //���������ٶ�
// 	float targetSpeed;       //����趨��Ŀ���ٶ�
//  CascadePID anglePID;     //����PID 
// };

// PID��ʼ������
void PID_Init(PID *pid, float p, float i, float d, float maxI, float maxOut);
// ����PID���㣬��Ҫ����Ĳ�����PID�ṹ�壬Ŀ��ֵ������ֵ
void PID_SingleCalc(PID *pid, float reference, float feedback);
// �������ָ��
void Motor_Send(Motor motor);

extern Motor motor[4];
void Motor_Init(Motor motor);
void L298N_SetMode(const char* Motorname, const char* Modename);
void L298N_SetSpeed(const char* Motorname, uint16_t Speed);
void GoStraight(uint16_t Speed);
void GoBack(uint16_t Speed);
void SpinTurnRight(uint16_t Speed);
void SpinTurnLeft(uint16_t Speed);
void GoRight(uint16_t Speed);
void GoLeft(uint16_t Speed);

#endif
