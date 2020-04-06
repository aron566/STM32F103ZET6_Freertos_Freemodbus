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
 * File: $Id: porttimer_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbport.h"

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
/* ----------------------- Variables ----------------------------------------*/
static USHORT usT35TimeOut50us;
static USHORT RT_TICK_PER_SECOND = 50;
static TIM_HandleTypeDef timer;
static void prvvTIMERExpiredISR(void);
void Master_timer_timeout_ind(void* parameter);
uint32_t master_timer_tick;
extern uint32_t TIM3CurrentTicks;
/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR(void);

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBMasterPortTimersInit(USHORT usTimeOut50us)
{
    /* backup T35 ticks */
    usT35TimeOut50us = usTimeOut50us;
    timer = htim3;
    master_timer_tick = (50 * usT35TimeOut50us) / RT_TICK_PER_SECOND;
    /*启动定时器中断*/
//    HAL_TIM_Base_Start(&timer);
    return TRUE;
}

void vMBMasterPortTimersT35Enable()
{
    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_T35);
    master_timer_tick = (50 * usT35TimeOut50us) / RT_TICK_PER_SECOND;
    
    /*启动定时器*/
    TIM3CurrentTicks = 0;
    __HAL_TIM_SET_COUNTER(&timer, 0);
    HAL_TIM_Base_Start(&timer);
}

void vMBMasterPortTimersConvertDelayEnable()
{
    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_CONVERT_DELAY);
    master_timer_tick = MB_MASTER_DELAY_MS_CONVERT * 1000 / RT_TICK_PER_SECOND;
    
    /*启动定时器*/
    TIM3CurrentTicks = 0;
    __HAL_TIM_SET_COUNTER(&timer, 0);
    HAL_TIM_Base_Start(&timer);
}

void vMBMasterPortTimersRespondTimeoutEnable()
{
    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_RESPOND_TIMEOUT);
    /*超时时间转换为所需次数*/
    master_timer_tick = MB_MASTER_TIMEOUT_MS_RESPOND * 1000 / RT_TICK_PER_SECOND;
    
    /*启动定时器*/
    TIM3CurrentTicks = 0;
    __HAL_TIM_SET_COUNTER(&timer, 0);
    HAL_TIM_Base_Start(&timer);
}

void vMBMasterPortTimersDisable()
{
    /*停止定时器*/
    HAL_TIM_Base_Stop(&timer);
}

void prvvTIMERExpiredISR(void)
{
    (void) pxMBMasterPortCBTimerExpired();
}

void Master_timer_timeout_ind(void* parameter)
{
    prvvTIMERExpiredISR();
}

#endif
