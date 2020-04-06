/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "usart.h"
#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "mb.h"
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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId IWDGHandle;
uint32_t IWDGBuffer[ 128 ];
osStaticThreadDef_t IWDGControlBlock;
osThreadId myTask02Handle;
uint32_t myTask02Buffer[ 128 ];
osStaticThreadDef_t myTask02ControlBlock;
osThreadId myTask03Handle;
uint32_t myTask03Buffer[ 128 ];
osStaticThreadDef_t myTask03ControlBlock;
osSemaphoreId myBinarySem01Handle;
osStaticSemaphoreDef_t myBinarySem01ControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

extern void Set_Event_Port(void);   
extern void Slave_Serial_rx_ind(uint16_t size);
/* USER CODE END FunctionPrototypes */

void Start_IWDG(void const * argument);
void StartTask02(void const * argument);
void StartTask03(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of myBinarySem01 */
  osSemaphoreStaticDef(myBinarySem01, &myBinarySem01ControlBlock);
  myBinarySem01Handle = osSemaphoreCreate(osSemaphore(myBinarySem01), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of IWDG */
  osThreadStaticDef(IWDG, Start_IWDG, osPriorityHigh, 0, 128, IWDGBuffer, &IWDGControlBlock);
  IWDGHandle = osThreadCreate(osThread(IWDG), NULL);

  /* definition and creation of myTask02 */
  osThreadStaticDef(myTask02, StartTask02, osPriorityIdle, 0, 128, myTask02Buffer, &myTask02ControlBlock);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);

  /* definition and creation of myTask03 */
  osThreadStaticDef(myTask03, StartTask03, osPriorityIdle, 0, 128, myTask03Buffer, &myTask03ControlBlock);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Start_IWDG */
/**
  * @brief  Function implementing the IWDG thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_Start_IWDG */
void Start_IWDG(void const * argument)
{
  /* USER CODE BEGIN Start_IWDG */
  char McuId[40]={0};
  uint32_t UID[3]={0};
  UID[0] = HAL_GetUIDw0();   //读取芯片的96位唯一标识ID THE First word
  UID[1] = HAL_GetUIDw1();   //读取芯片的96位唯一标识ID
  UID[2] = HAL_GetUIDw2();   //读取芯片的96位唯一标识ID
  sprintf((char*) McuId, "%08lX", UID[0]);
  sprintf((char*) McuId + 8, "%08lX",  UID[1]);
  sprintf((char*) McuId + 16, "%08lX",  UID[2]);
//  HAL_UART_Transmit_DMA(&huart1,(uint8_t *)McuId,strlen(McuId));
  /* Infinite loop */
  for(;;)
  {
    //喂狗
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);//触发pb5红色
    osDelay(500);
  }
  /* USER CODE END Start_IWDG */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
/*  使用定时器接收不定长数据
    osSemaphoreWait(myBinarySem01Handle ,100);
    if( RxFlag == 1) // 如果一帧接收完成
    {
        HAL_UART_Transmit(&huart3 ,RxBuffer ,RxCounter ,0xFFFF);
        RxFlag = 0;
        RxCounter = 0;
    }
*/
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
* @brief Function implementing the myTask03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask03 */
void StartTask03(void const * argument)
{
  /* USER CODE BEGIN StartTask03 */
    eMBInit(MB_RTU, 1, 1, 115200, MB_PAR_NONE);
    eMBEnable();
    /* Infinite loop */
    for(;;)
    {
        eMBPoll();
        Set_Event_Port();
        if(CQ_getLength(cb))
        {
            Slave_Serial_rx_ind(1);
        }
        osDelay(1);
    }
  /* USER CODE END StartTask03 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/*
//强定义 ， 定时器实现不定长数据接收
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)//用户的回调函数：接收完成处理
{
    if(huart->Instance == USART3) // 串口3接收完成回调
    {
        __HAL_TIM_SET_COUNTER(&htim4 ,0);   //清除定时器计数值
        if(0 == RxCounter)//如果是第一个字符。，则开启定时器
        {   
            __HAL_TIM_CLEAR_FLAG(&htim4 ,TIM_FLAG_UPDATE);
            HAL_TIM_Base_Start_IT(&htim4);
        }
        RxBuffer[RxCounter] = RxTemp[0];    //接收缓存的数据，放入接收数组
        RxCounter++;
        HAL_UART_Receive_IT(&huart3, (uint8_t *)RxTemp, REC_LENGTH);//重新使能中断
    }
}
*/

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
