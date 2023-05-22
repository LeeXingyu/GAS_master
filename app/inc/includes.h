/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                        Evaluation Board
*
* Filename      : includes.h
* Version       : V1.00
* Programmer(s) : EHS
*********************************************************************************************************
*/

#ifndef  INCLUDES_PRESENT
#define  INCLUDES_PRESENT


/*
*********************************************************************************************************
*                                         STANDARD LIBRARIES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>


/*
*********************************************************************************************************
*                                              LIBRARIES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_ascii.h>
//#include  <lib_math.h>
#include  <lib_mem.h>
#include  <lib_str.h>


/*
*********************************************************************************************************
*                                              APP / BSP
*********************************************************************************************************
*/

#include  <bsp.h>
#include  <app_cfg.h>

#include  "CLI.h"


/*
*********************************************************************************************************
*                                                 OS
*********************************************************************************************************
*/

#include  <ucos_ii.h>


/*
*********************************************************************************************************
*                                                 ST
*********************************************************************************************************
*/

#include "STM32Fxxx_Misc.h"
#include "STM32Fxxx_WDG.h"

/*
*********************************************************************************************************
*                                            INCLUDES END
*********************************************************************************************************
*/

#include "eeprom.h"
//#include "RAM.h"
#include "PLCFun.h"
#include "rtc.h"
#include "PLC_HFP2014_Main.h"
#include "DownLoad.h"
#include "PLC_HFP2014.h"

#if (defined(PLC_JZQ_HARDWARE_PARK))
#include "ESP8266.h"
#endif  //#if (defined(PLC_JZQ_HARDWARE_PARK))

#if (defined(PLC_JZQ_HARDWARE_PARK_V3))
#include "wizplatform.h"
#endif	//#if (defined(PLC_JZQ_HARDWARE_PARK_V3))

#endif

