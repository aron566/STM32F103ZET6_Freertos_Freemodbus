/*
 * FreeModbus Libary: RT-Thread Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.60 2013/08/13 15:07:05 Armink $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#include "bsp.h"

/* ----------------------- Static variables ---------------------------------*/
/* software simulation serial transmit IRQ handler thread stack */
static uint32_t serial_soft_trans_irq_stack[128];
/* software simulation serial transmit IRQ handler thread */
static osThreadId SlaveSerial_soft_trans_irq_Handle;
static osStaticThreadDef_t slave_transControlBlock;


/* serial event */
static StaticEventGroup_t   Slaveevent_serial;           /*事件存储，事件组*/
static EventGroupHandle_t   Slaveevent_serial_Handle;    /*事件标志组句柄*/
/* modbus slave serial device */
static UART_HandleTypeDef *huart_x;
/* ----------------------- Defines ------------------------------------------*/
/* serial transmit event */
#define EVENT_SERIAL_TRANS_START    (1<<0)

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR(void);

static void prvvUARTRxISR(void);
static BOOL serial_rx_ind(UART_HandleTypeDef *dev, uint16_t size);
static void serial_soft_trans_irq(void const *parameter);
void Slave_Serial_rx_ind(uint16_t size);
/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits,
        eMBParity eParity)
{
    /**
     * set 485 mode receive and transmit control IO
     * @note MODBUS_SLAVE_RT_CONTROL_PIN_INDEX need be defined by user
     */

    /* set serial name */
    switch (ucPORT)
    {
      case 1:
        huart_x = &huart1;
        break;
      case 2:
        huart_x = &huart2;
        break;
      case 3:
        
        break;
      default:
        return FALSE;
        break;
    }
    /*创建事件组，成功返回句柄*/
    Slaveevent_serial_Handle = xEventGroupCreateStatic(&Slaveevent_serial);
    if(Slaveevent_serial_Handle == NULL)
    {
        printf("创建事件组 失败！\n");
    }
    printf("创建串口事件组 Slaveevent_serial_Handle\n");
    osThreadStaticDef(slave_trans, serial_soft_trans_irq, osPriorityNormal, 0, 128, serial_soft_trans_irq_stack, &slave_transControlBlock);
    SlaveSerial_soft_trans_irq_Handle = osThreadCreate(osThread(slave_trans), NULL);
    return TRUE;
}

void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
//    uint32_t recvedEvent;
    if (xRxEnable)
    {
        /* enable RX interrupt */
//        __HAL_UART_ENABLE_IT(huart_x, UART_FLAG_IDLE);
        printf("开启串口接收中断 UART_IT_RXNE\n");
        /* switch 485 to receive mode */
        /*拉低RS485_SEL脚，RS485为接收状态*/
        HAL_GPIO_WritePin(RS485_SEL_GPIO_Port, RS485_SEL_Pin, GPIO_PIN_RESET);		
    }
    else
    {
        /* switch 485 to transmit mode */
        /*拉高RS485_SEL脚，RS485为发送状态*/
        HAL_GPIO_WritePin(RS485_SEL_GPIO_Port, RS485_SEL_Pin, GPIO_PIN_SET);		
        /* disable RX interrupt */
        printf("关闭串口接收中断 UART_FLAG_IDLE\n");
//        HAL_UART_AbortReceive_IT(huart_x);
//        __HAL_UART_DISABLE_IT(huart_x ,UART_FLAG_IDLE);

    }
    if (xTxEnable)
    {
        /* start serial transmit */
//        __HAL_UART_ENABLE_IT(huart_x ,UART_IT_TXE);
        printf("开启串口发送中断 UART_IT_TXE\n");
        /*设置事件标志组 EVENT_SERIAL_TRANS_START 置1*/
        xEventGroupSetBits(Slaveevent_serial_Handle, EVENT_SERIAL_TRANS_START);    
    }
    else
    {
        /* stop serial transmit */
//        recvedEvent = xEventGroupGetBits(Slaveevent_serial_Handle);
        printf("关闭串口发送中断 UART_IT_TXE\n");
//        HAL_UART_AbortTransmit_IT(huart_x);
//        __HAL_UART_DISABLE_IT(huart_x ,UART_IT_TXE);
    }
}

void vMBPortClose(void)
{
    HAL_UART_Abort(huart_x);
}

BOOL xMBPortSerialPutByte(CHAR ucByte)
{
    HAL_UART_Transmit_IT(huart_x,(uint8_t *)&ucByte, 1);
    return TRUE;
}

BOOL xMBPortSerialGetByte(CHAR * pucByte)
{
//    HAL_UART_Receive_IT(huart_x, (uint8_t*)pucByte, 1);
    CQ_getData(cb, (uint8_t*)pucByte, 1);
    return TRUE;
}

/* 
 * Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
void prvvUARTTxReadyISR(void)
{
    pxMBFrameCBTransmitterEmpty();
}

/* 
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUARTRxISR(void)
{
    pxMBFrameCBByteReceived();
}

/**
 * Software simulation serial transmit IRQ handler.
 *
 * @param parameter parameter
 */
static void serial_soft_trans_irq(void const *parameter) 
{
    uint32_t recved_event;
    
    while (1)
    {
        /* waiting for serial transmit start */
        recved_event = xEventGroupGetBits(Slaveevent_serial_Handle);
        /* execute modbus callback */
        if(recved_event == EVENT_SERIAL_TRANS_START)
        {
            prvvUARTTxReadyISR();
//            xEventGroupClearBits(Slaveevent_serial_Handle ,EVENT_SERIAL_TRANS_START);
        }
        osDelay(1);
    }
}

/**
 * This function is serial receive callback function
 *
 * @param dev the device of serial
 * @param size the data size that receive
 *
 * @return return RT_EOK
 */
static BOOL serial_rx_ind(UART_HandleTypeDef *dev, uint16_t size) {
    prvvUARTRxISR();
    return TRUE;
}

void Slave_Serial_rx_ind(uint16_t size)
{
    serial_rx_ind(huart_x ,size);
}