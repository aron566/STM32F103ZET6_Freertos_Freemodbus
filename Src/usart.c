/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

/*创建环形缓冲区*/
CQ_handleTypeDef *cb;

uint8_t Uart1_RxBuff[UART_RXBUFF_MAX] = {0};
uint8_t Uart2_RxBuff[UART_RXBUFF_MAX] = {0};

/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart2_rx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 DMA Init */
    /* USART2_RX Init */
    hdma_usart2_rx.Instance = DMA1_Channel6;
    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart2_rx.Init.Mode = DMA_NORMAL;
    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart2_rx);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();
  
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

#if (USE_NEW_REDIRECT == 0)
/*************************************************
  * 函数功能: 重定向c库函数printf到HAL_UART_Transmit
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fputc(int ch, FILE *f)
{
#if (ENABLE_PRINTF == 1)
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 10);//原来使用阻塞式传输
#endif
  return ch;
}
/**
  * 函数功能: 重定向c库函数getchar,scanf
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fgetc(FILE * f)
{
  uint8_t ch = 0;
  while(HAL_UART_Receive(&huart1,&ch, 1, 0xffff)!=HAL_OK);
  return ch;
}

#else
/*新式重定向*/
#include "stdio.h"
int __io_putchar(int ch)
{
#if (ENABLE_PRINTF == 1)
    HAL_UART_Transmit(&huart1 ,()uint8_t)&ch ,1 ,0xFFFF);
#endif
    return ch;
}
int __write(int file, char *ptr, int len)
{
    int DataIdx;
    for(DataIdx = 0; DataIdx < len; DataIdx++)
    {
        __io_putchar(*ptr++);
    }
    return len;
}
#endif


/**
  ******************************************************************
  * @brief   空闲中断处理回调
  * @author  aron66
  * @version V1.0
  * @date    2020/3/15
  ******************************************************************
  */                                    
static void USAR_UART_IDLECallback(UART_HandleTypeDef *huart)
{
    uint8_t *Rx_buff = NULL;
    DMA_HandleTypeDef *Dma_Uartx = NULL;
    /*
    首先停止DMA传输，
    1.防止后面又有数据接收到，产生干扰，因为此时的数据还未处理。
    2.DMA需要重新配置。
    */
    HAL_UART_DMAStop(huart);                                                    //停止本次DMA传输
    /*判断来自的端口*/
    if(huart->Instance == USART1)
    {
        Rx_buff = Uart1_RxBuff;
        Dma_Uartx = &hdma_usart1_rx;
    }
    if(huart->Instance == USART2)
    {
        Rx_buff = Uart2_RxBuff;
        Dma_Uartx = &hdma_usart2_rx;
    }
    uint8_t data_length  = UART_RXBUFF_MAX - __HAL_DMA_GET_COUNTER(Dma_Uartx);  //计算接收到的数据长度
    
    CQ_putData(cb, Rx_buff, data_length);                                       //将接收的一包数据加入缓冲区
    
    memset(Rx_buff,0,data_length);                                              //清零接收缓冲区
    data_length = 0;
    HAL_UART_Receive_DMA(huart ,Rx_buff ,UART_RXBUFF_MAX);                      //重启开始DMA传输 每次128字节数据
}

/**
  ******************************************************************
  * @brief   自定义串口中断回调
  * @author  aron66
  * @version V1.0
  * @date    2020/3/15
  ******************************************************************
  */
void USER_UART_IRQHandler(UART_HandleTypeDef *huart)
{
    /*串口1中断回调处理*/
    if(USART1 == huart->Instance)                                   //判断是否是串口1
    {
        if(RESET != __HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))   //判断是否是空闲中断
        {
            __HAL_UART_CLEAR_IDLEFLAG(&huart1);                     //清楚空闲中断标志（否则会一直不断进入中断）
            USAR_UART_IDLECallback(huart);                          //调用中断处理函数
        }
    }
    /*串口2中断回调处理*/
    if(USART2 == huart->Instance)                                   //判断是否是串口2
    {
        if(RESET != __HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE))   //判断是否是空闲中断
        {
            __HAL_UART_CLEAR_IDLEFLAG(&huart2);                     //清楚空闲中断标志（否则会一直不断进入中断）
            USAR_UART_IDLECallback(huart);                          //调用中断处理函数
        }
    }
}
/**
  ******************************************************************
  * @brief   初始化建立环形缓冲区
  * @author  aron566
  * @version v1.0
  * @date    2020/4/6
  ******************************************************************
  */
void Circular_BUFF_init(void)
{
    cb = cb_create(256);
    
    /*启动串口接收*/
    HAL_UART_Receive_DMA(&huart1 ,Uart1_RxBuff ,UART_RXBUFF_MAX);
    HAL_UART_Receive_DMA(&huart1 ,Uart2_RxBuff ,UART_RXBUFF_MAX);
    
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
