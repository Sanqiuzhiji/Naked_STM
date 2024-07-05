#ifndef __L298N_H
#define __L298N_H

#include "main.h"

#define RR 30u    //电机减速比

#define RELOADVALUE(motor) __HAL_TIM_GetAutoreload(&motor.encodingTIM_HandleTypeDef)    //获取自动装载值,本例中为20000
#define COUNTERNUM(motor) __HAL_TIM_GetCounter(&motor.encodingTIM_HandleTypeDef)       //获取编码器定时器中的计数值


typedef struct _PID
{
	float kp,ki,kd;               //PID的三个参数
	float error,lastError;        //当前误差和上一次的误差
	float integral,maxIntegral;   //积分量和积分的限幅
	float output,maxOutput;       //PID的输出量和PID的最大输出量
}PID;

//电机结构体
typedef struct _Motor
{
    TIM_HandleTypeDef encodingTIM_HandleTypeDef;
    TIM_HandleTypeDef pwmTIM_HandleTypeDef;    
    uint32_t pwmTIM_CHANNEL;
	int32_t lastAngle;        //上10ms转过的角度
	int32_t totalAngle;       //总的角度
	int16_t loopNum;          //溢出次数计数值
	float speed;              //电机输出轴目前转速,单位为RPM
	float targetSpeed;       //添加设定的目标速度
	PID pid;                 //添加电机对应PID
}Motor;

#include"main.h"

#define LIMIT(x,min,max) (x)=(((x)<=(min))?(min):(((x)>=(max))?(max):(x)))  //限幅定义

// typedef struct _CascadePID
// {
// 	PID inner;    // 内环速度环PID
// 	PID outer;    // 外环角度环PID
// 	float output;
// };

// typedef struct _Motor
// {
// 	int32_t lastAngle;       //上次计数结束时转过的角度
// 	int32_t totalAngle;      //总共转过的角度
// 	int16_t loopNum;         //电机计数过零计数
// 	float speed;             //电机输出轴速度
// 	float targetSpeed;       //添加设定的目标速度
//  CascadePID anglePID;     //串级PID 
// };

// PID初始化函数
void PID_Init(PID *pid, float p, float i, float d, float maxI, float maxOut);
// 单级PID计算，需要传入的参数，PID结构体，目标值，反馈值
void PID_SingleCalc(PID *pid, float reference, float feedback);
// 电机发送指令
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
