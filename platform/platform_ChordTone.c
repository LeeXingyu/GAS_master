/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXX公司, 保留所有权
* 文件名      ：platform_ChordTone.c
* 作者        ：XXXXXX
* 电子邮箱    ：XXXXXX
* 版本        ：V1.0.0.0
* 日期        ：2017-10-06
* 功能描述    ：
* 备注        ：
* 主要函数列表：
* 修改记录    ：
  日      期          版本          修改人          修改内容
  2017-10-06          1.0           XXXXXXXX        创建文件	
****************************************************************************************/

/* 调试部分开关 -----------------------------------------------------------------------*/

/* 包含文件 ---------------------------------------------------------------------------*/
#include "CoreInclude.h"
#include "NewType.h"
#include <stdint.h>

#include "app_cfg.h"

/* 宏定义 -----------------------------------------------------------------------------*/

/* 结构类型定义 -----------------------------------------------------------------------*/

/* 原型声明部分 -----------------------------------------------------------------------*/

/* 全局变量声明 -----------------------------------------------------------------------*/

/* 静态变量定义 -----------------------------------------------------------------------*/
static uint32_t uiPWMTimerPeriod		= 0;
static BOOL		bChordTone_PWMDisable	= TRUE;

/****************************************************************************************
* 函数名称：ChordTone_FreqDevInit()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-10-06 22:56
****************************************************************************************/
void ChordTone_PWMDevInit(void)
{
//    GPIO_InitTypeDef 		GPIO_InitStructure;

//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

//	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);

//    GPIO_SetBits(GPIOB, GPIO_Pin_8);
    
    
	GPIO_InitTypeDef 		GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_2);
}

/****************************************************************************************
* 函数名称：ChordTone_PWMEnable()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-10-6 23:6
****************************************************************************************/
void ChordTone_PWMEnable(uint32 freq)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);

	uiPWMTimerPeriod = (uint32_t)(SystemCoreClock / (freq * 8)) - 1;

	TIM_TimeBaseStructure.TIM_Period			= uiPWMTimerPeriod;
	TIM_TimeBaseStructure.TIM_Prescaler			= 7;
	TIM_TimeBaseStructure.TIM_ClockDivision		= 0;
	TIM_TimeBaseStructure.TIM_CounterMode		= TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM16, ENABLE);

	if (bChordTone_PWMDisable)
	{
		uint16_t				TIM_Pulse;
		TIM_OCInitTypeDef		TIM_OCInitStructure;

		TIM_Pulse = (uint16_t)(((uint32_t)50 * (uiPWMTimerPeriod - 1)) / 100);

		TIM_OCInitStructure.TIM_OCMode			= TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState		= TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_OutputNState	= TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity		= TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCNPolarity		= TIM_OCNPolarity_Low;
		TIM_OCInitStructure.TIM_OCIdleState		= TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState	= TIM_OCIdleState_Reset;
		TIM_OCInitStructure.TIM_Pulse			= TIM_Pulse;

		TIM_OC1Init(TIM16, &TIM_OCInitStructure);
		TIM_CtrlPWMOutputs(TIM16, ENABLE);

		bChordTone_PWMDisable = FALSE;
	}
}

/****************************************************************************************
* 函数名称：ChordTone_PWMDisable()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-10-6 23:21
****************************************************************************************/
void ChordTone_PWMDisable(void)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;

	TIM_OCInitStructure.TIM_OCMode			= TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState		= TIM_OutputState_Disable;
	TIM_OCInitStructure.TIM_OutputNState	= TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity		= TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity		= TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState		= TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState	= TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_Pulse			= 0;

	TIM_OC1Init(TIM16, &TIM_OCInitStructure);
	TIM_CtrlPWMOutputs(TIM16, ENABLE);
    
	bChordTone_PWMDisable = TRUE;
}

/****************************************************************************************
* 函数名称：Buzzer_GPIOInit()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-10-6 23:21
****************************************************************************************/
void Buzzer_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(BUZZER_PWR_CTRL_PERIPH_ID, ENABLE);

	GPIO_InitStructure.GPIO_Pin     = BUZZER_PWR_CTRL_PIN;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
	GPIO_Init(BUZZER_PWR_CTRL_PORT, &GPIO_InitStructure);
}

/****************************************************************************************
* 函数名称：Buzzer_GPIOInit()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-10-6 23:21
****************************************************************************************/
void Buzzer_PwrOn(void)
{
	GPIO_ResetBits(BUZZER_PWR_CTRL_PORT, BUZZER_PWR_CTRL_PIN);
}

void Buzzer_PwrOff(void)
{
	GPIO_SetBits(BUZZER_PWR_CTRL_PORT, BUZZER_PWR_CTRL_PIN);
}

/*******************************版权所有(c)**************END OF FILE********************/
