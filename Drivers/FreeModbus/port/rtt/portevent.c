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
 * File: $Id: portevent.c,v 1.60 2013/08/13 15:07:05 Armink $
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
volatile uint32_t Modbus_Event_ALL = 0;
void Set_Event_Port(void);   
/* ----------------------- Variables ----------------------------------------*/
static StaticEventGroup_t   xSlaveOsEvent;      /*  事件存储，事件组  */
static EventGroupHandle_t   slave_event_Handle; /*  事件标志组句柄    */
/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortEventInit( void )
{
    /*创建事件组，成功返回句柄*/
    printf("创建事件组，成功返回句柄\n");
    slave_event_Handle = xEventGroupCreateStatic(&xSlaveOsEvent);
    if(slave_event_Handle == NULL)
    {
        printf("创建事件组 失败！\n");
    }
    return TRUE;
}

BOOL
xMBPortEventPost( eMBEventType eEvent )
{
    /*设置事件标志组 eEvent 置1*/
    printf("设置事件标志组 eEvent:0x%04X\n",eEvent);
    Modbus_Event_ALL |= eEvent;
        
    return TRUE;
}

BOOL
xMBPortEventGet( eMBEventType * eEvent )
{
    /* waiting forever OS event */
    uint32_t recvedEvent;
    recvedEvent = xEventGroupGetBits(slave_event_Handle);
    switch (recvedEvent)
    {
    case EV_READY:
        *eEvent = EV_READY;
        printf("读取事件标志组 EV_READY\n");
        break;
    case EV_FRAME_RECEIVED:
        *eEvent = EV_FRAME_RECEIVED;
        printf("读取事件标志组 EV_FRAME_RECEIVED\n");
        break;
    case EV_EXECUTE:
        *eEvent = EV_EXECUTE;
        printf("读取事件标志组 EV_EXECUTE\n");
        break;
    case EV_FRAME_SENT:
        *eEvent = EV_FRAME_SENT;
        printf("读取事件标志组 EV_FRAME_SENT\n");
        break;
    }
    xEventGroupClearBits(slave_event_Handle ,recvedEvent);
    return TRUE;
}

/**
  ******************************************************************
  * @brief   循环设置事件接口，原中断中设置事件存在问题
  * @author  aron566
  * @version v1.0
  * @date    2020/4/5
  ******************************************************************
  */
void Set_Event_Port(void)
{
    uint32_t ret = 0;
    if(Modbus_Event_ALL)
    {
        ret = xEventGroupSetBits(slave_event_Handle, Modbus_Event_ALL);
        if(ret != Modbus_Event_ALL)
        {
            printf("设置事件失败\n");
        }
        else
        {
            Modbus_Event_ALL = 0;
        }
    }
}
