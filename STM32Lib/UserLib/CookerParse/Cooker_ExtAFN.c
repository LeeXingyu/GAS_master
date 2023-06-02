/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXX公司, 保留所有权
* 文件名      ：Cooker_ExtAFN.c
* 作者        ：XXXXXX
* 电子邮箱    ：XXXXXX
* 版本        ：V1.0.0.0
* 日期        ：2017-11-15
* 功能描述    ：
* 备注        ：
* 主要函数列表：
* 修改记录    ：
  日      期          版本          修改人          修改内容
  2017-11-15          1.0           XXXXXXXX        创建文件	
****************************************************************************************/

/* 调试部分开关 -----------------------------------------------------------------------*/

/* 包含文件 ---------------------------------------------------------------------------*/
#include "Cooker_Parse.h"

//#include "Led.h"
#include "HT1621.h"
#include "STM32Fxxx_Misc.h"
#include "string.h"
/* 宏定义 -----------------------------------------------------------------------------*/
#define GSA_STOVE_OFF_FIRE_TIME             31000

/* 结构类型定义 -----------------------------------------------------------------------*/

/* 原型声明部分 -----------------------------------------------------------------------*/
extern void Cooker_WirelessSendLoad(char *load, unsigned int len);
extern void SendRfFrame (unsigned char *buffer, unsigned char len);
extern void Updata_Awaken_Config(void);
extern void Updata_Normal_Config(void);
/* 全局变量声明 -----------------------------------------------------------------------*/
static BOOL	bGasStove_FireState = FALSE;
static BOOL bCooker_SysIdMatchState = FALSE;
static BOOL bGasStove_BatState = FALSE;
static BOOL bGasStove_GasCtrlState = FALSE;
static uint32 uiFireState_ChkDly = 0;
static uint32 uiBatState_ChkDly = 0;
static uint32 uiGasCtrlState_ChkDly = 0;
/* 静态变量定义 -----------------------------------------------------------------------*/
static char Awaken_data[30] = {0xAA};
/****************************************************************************************
* 函数名称：GasStove_FireStateChkService()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-11-19 22:41
****************************************************************************************/
void GasStove_FireStateChkService(void)
{
	if (bGasStove_FireState && (BSP_OS_Timeout(uiFireState_ChkDly, GSA_STOVE_OFF_FIRE_TIME)))
	{
		bGasStove_FireState = FALSE;
		Led_DisplayOff(LED_DIS_GAS_FIRE);
	}
}

/****************************************************************************************
* 函数名称：GasStove_FireState()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-11-19 22:41
****************************************************************************************/
BOOL GasStove_FireState(void)
{
	return bGasStove_FireState;
}

/****************************************************************************************
* 函数名称：GasStove_Flameout()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-11-19 22:41
****************************************************************************************/
void GasStove_Flameout(void)
{
	char i = 0;
	Cooker_Parse_t entity;
	
	Updata_Awaken_Config();
	memset(Awaken_data,0xAA,30);
	//19200bitrate 52ms从机接收一次
	//循环发送 唤醒
	for(i = 0;i<55;i++)
	{	
			Cooker_WirelessSendLoad(Awaken_data,30);
	}
	Updata_Normal_Config();

	
	entity.cmd			= eCOOKER_CTRL_Gas;
	entity.payload[0]	= COOKER_PARSE_FALSE;
	entity.length		= 1;

	Cooker_Load(&entity);
}

/****************************************************************************************
* 函数名称：Cooker_SysIdMatch()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-11-19 22:41
****************************************************************************************/
void Cooker_SysIdMatch(void)
{
	Cooker_Parse_t entity;

	entity.cmd		= eCOOKER_SET_SYS_ID;
	entity.length	= 0;

	bCooker_SysIdMatchState = FALSE;

	Cooker_Load(&entity);
}

/****************************************************************************************
* 函数名称：
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-11-19 23:22
****************************************************************************************/
BOOL Cooker_SysIdMatchResult(void)
{
	return bCooker_SysIdMatchState;
}

/****************************************************************************************
* 函数名称：Cooker_AFNChk()
* 功能描述: 对从机信号的cmd进行分析
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-11-15 20:22
****************************************************************************************/
void Cooker_AFNChk(Cooker_Parse_t *entity)
{
	switch ((Cooker_Cmd_e)entity->cmd)
	{
		case eCOOKER_SET_SYS_ID:
		{
			//对码成功
			bCooker_SysIdMatchState = TRUE;
		}
		break;
		case eCOOKER_FIRE_STATE:
		{
			if (1 == entity->length)
			{
				if (COOKER_PARSE_TRUE == entity->payload[0])
				{
					Led_DisplayOn(LED_DIS_GAS_FIRE);
					bGasStove_FireState = TRUE;
				}
				else
				{
					Led_DisplayOff(LED_DIS_GAS_FIRE);
					bGasStove_FireState = FALSE;
				}

				uiFireState_ChkDly = BSP_OS_TimeGet();

				entity->payload[0] = COOKER_PARSE_TRUE;
				entity->length = 1;
				Cooker_Load(entity);
			}
		}
		break;
		case eCOOKER_CTRL_Gas:    //气阀控制
		{
			if (1 == entity->length)
			{
				if (COOKER_PARSE_FALSE == entity->payload[0])
				{
					Led_DisplayOn(LED_DIS_GASCHECK);
					bGasStove_GasCtrlState = TRUE;
				}
				else
				{
					Led_DisplayOff(LED_DIS_GASCHECK);
					bGasStove_GasCtrlState = FALSE;
				}
				uiGasCtrlState_ChkDly = BSP_OS_TimeGet();

			}
		}
		break;
		case eCOOKER_STATE_Gas:        //气压状态
		{
			if (1 == entity->length)
			{
				if (COOKER_PARSE_TRUE == entity->payload[0])
				{
					Led_DisplayOn(LED_DIS_GAS_FIRE);
					bGasStove_FireState = TRUE;
				}
				else
				{
					Led_DisplayOff(LED_DIS_GAS_FIRE);
					bGasStove_FireState = FALSE;
				}
				uiFireState_ChkDly = BSP_OS_TimeGet();

			}
		}
		break;
		case eCOOKER_STATE_Bat:
		{
			if (1 == entity->length)
			{
				if (COOKER_PARSE_FALSE == entity->payload[0])				
				{
					Led_DisplayOn(LED_DIS_BATTERY);
					bGasStove_BatState = TRUE;
				}
				else
				{
					Led_DisplayOff(LED_DIS_BATTERY);
					bGasStove_BatState = FALSE;
				}
				uiBatState_ChkDly = BSP_OS_TimeGet();
			}	
		}
		break;
		default: break;
	}
}

void GasStove_GasBatStateChkService(void)
{
	if((!bGasStove_BatState) && (BSP_OS_Timeout(uiBatState_ChkDly, GSA_STOVE_OFF_FIRE_TIME)))
	{
		Led_DisplayOff(LED_DIS_BATTERY);
		bGasStove_BatState = FALSE;
	} 
	if((!bGasStove_GasCtrlState) && (BSP_OS_Timeout(uiGasCtrlState_ChkDly, GSA_STOVE_OFF_FIRE_TIME)))
	{
		Led_DisplayOff(LED_DIS_GASCHECK);
		bGasStove_BatState = FALSE;
	} 	
}


/*******************************版权所有(c)**************END OF FILE********************/
