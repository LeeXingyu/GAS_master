/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXX公司, 保留所有权
* 文件名      ：platform_ExtPin.c
* 作者        ：XXXXXX
* 电子邮箱    ：XXXXXX
* 版本        ：V1.0.0.0
* 日期        ：2017-11-19
* 功能描述    ：
* 备注        ：
* 主要函数列表：
* 修改记录    ：
  日      期          版本          修改人          修改内容
  2017-11-19          1.0           XXXXXXXX        创建文件	
****************************************************************************************/

/* 调试部分开关 -----------------------------------------------------------------------*/

/* 包含文件 ---------------------------------------------------------------------------*/
#include "CoreInclude.h"
#include "NewType.h"
#include "app_cfg.h"

#include "STM32Fxxx_Misc.h"

/* 宏定义 -----------------------------------------------------------------------------*/
#define FAN_ROTATION_DELAY_TIME				300							//风扇转动抖动延时时间

/* 结构类型定义 -----------------------------------------------------------------------*/

/* 原型声明部分 -----------------------------------------------------------------------*/

/* 全局变量声明 -----------------------------------------------------------------------*/

/* 静态变量定义 -----------------------------------------------------------------------*/
static BOOL     bFanRotation_RunState  = FALSE;
static uint32_t uiFanRotation_RunDelay = 0;

/****************************************************************************************
* 函数名称：Cooker_ExtPinGPIOInit()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-11-19 23:36
****************************************************************************************/
void Cooker_ExtPinGPIOInit(void)
{
	GPIO_InitTypeDef 		GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(FAN_ROTATION_PERIPH_CLK | GAS_SENSOR_PERIPH_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin     = FAN_ROTATION_PIN;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
	GPIO_Init(FAN_ROTATION_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin     = GAS_SENSOR_PIN;
	GPIO_Init(GAS_SENSOR_PORT, &GPIO_InitStructure);
}

/****************************************************************************************
* 函数名称：FanRotation_Level()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-11-19 23:36
****************************************************************************************/
static BOOL FanRotation_Level(void)
{
	return ((GPIO_ReadInputDataBit(FAN_ROTATION_PORT, FAN_ROTATION_PIN)) ? FALSE : TRUE);
}

/****************************************************************************************
* 函数名称：GasStove_FanRotation()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-11-19 23:36
****************************************************************************************/
BOOL GasStove_FanRotation(void)      //风机是否转动
{
	return bFanRotation_RunState;
}

/****************************************************************************************
* 函数名称：
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2018-10-7 23:57
****************************************************************************************/
void FanRotation_Service(void)
{
	BOOL bState = FanRotation_Level();
    static BOOL bLastState = FALSE;

	if (bLastState != bState)
    {
        bLastState = bState;
		uiFanRotation_RunDelay = BSP_OS_TimeGet();
    }

	if (BSP_OS_Timeout(uiFanRotation_RunDelay, FAN_ROTATION_DELAY_TIME))
		bFanRotation_RunState = bLastState;
}

/****************************************************************************************
* 函数名称：GasStove_GasSensor()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-11-19 23:36
****************************************************************************************/
BOOL GasStove_GasSensor(void)        //气敏检查
{
	return ((GPIO_ReadInputDataBit(GAS_SENSOR_PORT, GAS_SENSOR_PIN)) ? TRUE : FALSE);
}

/*******************************版权所有(c)**************END OF FILE********************/
