/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t rxdata;
uint8_t serial_rxdata;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_TIM15_Init();
  MX_TIM1_Init();
  MX_TIM5_Init();
  MX_I2C1_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */

  // OLED_Test();
  // uint8_t Buf[32] = {4, 0xAF, 0X00, 0X00, 0XFA};
  // NRF24L01_Init(); // 此初始化不了，则程序停止
  // NRF24L01_TX_Mode(TX_ADDRESSChen);
  // HAL_Delay(500);
  // NRF24L01_SendBuf(Buf);
  // Serial_Printf("Cisok\r\n");
  // Buf[2]++;
  // NRF24L01_TX_Mode(TX_ADDRESSJia);
  // HAL_Delay(500);
  // NRF24L01_SendBuf(Buf);
  // Serial_Printf("Jisok\r\n");
  // Buf[2]++;
  // NRF24L01_TX_Mode(TX_ADDRESSTang);
  // HAL_Delay(500);
  // NRF24L01_SendBuf(Buf);
  // Serial_Printf("Tisok\r\n");
  HAL_TIM_Base_Start_IT(&htim6);                // 开启10ms中断
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1); // 开启编码器定时器
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_2); // 开启编码器定时器
  __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);   // 开启编码器定时器更新中断,防溢出处理
  __HAL_TIM_SET_COUNTER(&htim2, 30000);         // 将编码器定时器初始值设定为30000

  motor[0].encodingTIM_HandleTypeDef = htim3;
  motor[0].pwmTIM_HandleTypeDef = htim4;
  motor[0].pwmTIM_CHANNEL = TIM_CHANNEL_1;
  motor[0].targetSpeed=2.5;
  motor[0].pid.output=0;
  Motor_Init(motor[0]);
  L298N_SetSpeed("a1", 200);

  PID_Init(&motor[0].pid, 10, 0.0, 0.0, 0, 250);
  L298N_SetMode("a1", "right");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    //  L298N_SetMode("a1", "right");
    // HAL_Delay(5000);
    // L298N_SetMode("a1", "left");
    // HAL_Delay(5000);
    // L298N_SetMode("a1", "stop");
    // HAL_Delay(5000);
    // Serial_Printf("I am serial\r\n");
    // Bluetooth_SendByte(0xaa);
    // GoStraight(300);
    // HAL_Delay(2000);
    // GoBack(300);
    // HAL_Delay(2000);
    // SpinTurnRight(300);
    // HAL_Delay(2000);
    // SpinTurnLeft(300);
    // HAL_Delay(2000);
    // Serial_Printf("%x",rxdata);
    // Serial_Printf("\r\n");
    // HAL_UART_Receive_IT(&huart6,(uint8_t *)&rxdata,1);
    // HAL_UART_Receive_IT(&huart1,(uint8_t *)&serial_rxdata,1);
    // Videographyhead_SendByte(rxdata);
    // Serial_SendByte(rxdata);
    // if(rxdata==1) GoStraight(350);
    // else if(rxdata==2) SpinTurnLeft(350);
    // else if(rxdata==3) SpinTurnRight(350);
    // else if(rxdata==4) GoBack(350);
    // else if(rxdata==5)
    // {
    //  L298N_SetMode("a1", "stop");
    //  L298N_SetMode("a2", "stop");
    //  L298N_SetMode("b1", "stop");
    //  L298N_SetMode("b2", "stop");
    // }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
   */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
   */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
  {
  }

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
  {
  }

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

// 中断处理函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == htim6.Instance) // 100ms中断
  {
    int16_t pluse = COUNTERNUM(motor[0]) - RELOADVALUE(motor[0]) / 2; // 从开始到现在当前10ms的总脉冲数
    motor[0].totalAngle = pluse;
    // 进行速度计算,根据前文所说的,4倍频,编码器13位,减速比30,再乘以100即为每秒钟输出轴多少转
    // motor.totalAngle - motor.lastAngle为当前100ms内的增量，即脉冲数
    motor[0].speed = (float)(motor[0].totalAngle - motor[0].lastAngle) / (4 * 16 * RR) * 10;
    motor[0].lastAngle = motor[0].totalAngle; // 更新转过的圈数
    Motor_Send(motor[0]);
    Serial_Printf("%f\r\n", motor[0].speed);
  }
  // 如果是编码器更新中断,即10ms内,脉冲数超过了计数范围,需要进行处理
  else if (htim->Instance == motor[0].encodingTIM_HandleTypeDef.Instance)
  {
    // if (COUNTERNUM(motor[0]) > 10000)
    //   motor[0].loopNum++; // 向上计数超过10000，正溢出+1
    // else if (COUNTERNUM(motor[0]) < 10000)
    //   motor[0].loopNum--;                                             // 向下计数小于0，负溢出+1
    // __HAL_TIM_SetCounter(&motor[0].encodingTIM_HandleTypeDef, 30000); // 重新设定初始值
  }
}
/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
   */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
