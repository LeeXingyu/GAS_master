/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXX公司, 保留所有权
* 文件名      ：HT1621.c
* 作者        ：XXXXXX
* 电子邮箱    ：XXXXXX
* 版本        ：V1.0.0.0
* 日期        ：2018-09-15
* 功能描述    ：
* 备注        ：
* 主要函数列表：
* 修改记录    ：
  日      期          版本          修改人          修改内容
  2018-09-15          1.0           XXXXXXXX        创建文件	
****************************************************************************************/

/* 调试部分开关 -----------------------------------------------------------------------*/
#define HT1621_LED_EN_BLINK

/* 包含文件 ---------------------------------------------------------------------------*/
#include "HT1621.h"

#include "utilities.h"
#include <string.h>

#include "STM32Fxxx_Misc.h"

/* 宏定义 -----------------------------------------------------------------------------*/
#define HT1621_USER_DEF_VALUE			0x01		//小数点或者其它图标

#define HT1621_SEG1_ADDR				0x16		//第一个数字，燃气泄漏图标（非字体）
#define HT1621_SEG2_ADDR				0x18		//第二个数字，燃气泄漏图标汉字
#define HT1621_SEG3_ADDR				0x1A		//第三个数字，中间冒号
#define HT1621_SEG4_ADDR				0x1D		//第四个数字

#define HT1621_RIGHT_SIGN_ADDR			0x1C		//右边三个图标地址
#define HT1621_TOP_SIGN_ADDR			0x1F		//顶部4个图标
#define HT1621_GASCHECK_SIGN_ADDR			0x1E  //气阀关闭地址		

#define HT1621_SEG_MAX_ADDR_CNT			6			//最大管理地址

/*****************************属于右边图标地址：HT1621_RIGHT_SIGN_ADDR*****************************/
#define SEG_SIGN_BAT_VALUE				0x02		//电池图标
#define SEG_SIGN_MAN_VALUE				0x04		//人体感应图标
#define SEG_SIGN_FIRE_VALUE				0x08		//火焰燃烧图标
#define SEG_SIGN_GASCHECK_VALUE   0x10    //气阀关闭图标 PIN9(MSB)
/*****************************属于右边图标地址：HT1621_RIGHT_SIGN_ADDR*****************************/

/******************************属于顶部图标地址：HT1621_TOP_SIGN_ADDR******************************/
#define SEG_SIGN_RTC_SET_VALUE			0x01		//时间设置图标
#define SEG_SIGN_START_TIME_VALUE		0x02		//启动时间图标
#define SEG_SIGN_FIRE_OFF_VALUE			0x04		//熄火时间图标
#define SEG_SIGN_COM_ID_VALUE			0x08		//通讯对码图标
/******************************属于顶部图标地址：HT1621_TOP_SIGN_ADDR******************************/

/* 结构类型定义 -----------------------------------------------------------------------*/

/* 原型声明部分 -----------------------------------------------------------------------*/

/* 全局变量声明 -----------------------------------------------------------------------*/

/* 静态变量定义 -----------------------------------------------------------------------*/
static const uint8 a_ucHT1621_SEG1Code[] =                         //时间表数据
{
	0xFA, 0x60, 0xD6, 0xF4, 0x6C, 0xBC, 0xBE, 0xE0, 0xFE, 0xFC,		//0-9
	0x04,															//-
	0x00,															//熄灭
};

static const uint8 a_ucHT1621_SEG2Code[] =
{
	0xFA, 0x60, 0xD6, 0xF4, 0x6C, 0xBC, 0xBE, 0xE0, 0xFE, 0xFC,		//0-9
	0x04,															//-
	0x00,															//熄灭
};
static const uint8 a_ucHT1621_SEG3Code[] =
{
	0xFA, 0x60, 0xD6, 0xF4, 0x6C, 0xBC, 0xBE, 0xE0, 0xFE, 0xFC,		//0-9
	0x04,															//-
	0x00,															//熄灭
};

static const uint8 a_ucHT1621_SEG4Code[] =
{
	0xAF, 0x06, 0x6D, 0x4F, 0xC6, 0xCB, 0xEB, 0x0E, 0xEF, 0xCF,		//0-9
	0x40,															//-
	0x00,															//熄灭
};

static uint8 aucSEG_PreServiceValue[HT1621_SEG_MAX_ADDR_CNT] =		//上一次写入该地址的值
{
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

static uint8 aucSEG_CurServiceValue[HT1621_SEG_MAX_ADDR_CNT] =		//当前应该写入改地址的数据
{
	0, 0, 0, 0, 0, 0,
};

//static const uint8 aucSEG_ValueAddr[HT1621_SEG_MAX_ADDR_CNT]=		//地址
//{
//	HT1621_SEG1_ADDR,
//	HT1621_SEG2_ADDR,
//	HT1621_SEG3_ADDR,
//	HT1621_SEG4_ADDR,

//	HT1621_RIGHT_SIGN_ADDR,
//	HT1621_TOP_SIGN_ADDR,
//};

//static const uint8 aucSEG_ValueCnt[HT1621_SEG_MAX_ADDR_CNT]=		//写入当前地址的数据长度
//{
//	2,
//	2,
//	2,
//	2,

//	1,
//	1,
//};

/***********************************LED参数*************************************/
static uint8  ucLed_DisValue = 0;

#if (defined(HT1621_LED_EN_BLINK))
static uint32 uiLed_DisQuickBlinkDly;
static uint32 uiLed_DisSlowBlinkDly;
static uint32 uiLed_DisBatSlowBlinkDly;
#endif  //#if (defined(HT1621_LED_EN_BLINK))
/***********************************LED参数*************************************/

/***********************************SEG参数*************************************/
static uint8    *p_ucLED_SEGValue = NULL;

static BOOL     bLED_SEGBlinkOnOff  = FALSE;
static uint8    ucLED_SEGBlinkState = 0;                               //以Bit位表示闪烁位
static uint32   uiLED_SEGBlinkDly   = 0;
/***********************************SEG参数*************************************/

/****************************************************************************************
* 函数名称：
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-10-15 11:5
****************************************************************************************/
static void Led_Display(uint16 value)
{
	//煤气泄露图标 aucSEG_CurServiceValue[0]/[1]还包括数码管的数字
	if (value & LED_DIS_GAS_LEAK)
	{
		aucSEG_CurServiceValue[0] |= HT1621_USER_DEF_VALUE;
		aucSEG_CurServiceValue[1] |= HT1621_USER_DEF_VALUE;
	}
	else
	{
		aucSEG_CurServiceValue[0] &= (~HT1621_USER_DEF_VALUE);
		aucSEG_CurServiceValue[1] &= (~HT1621_USER_DEF_VALUE);
	}
	
	//T9 气阀关闭状态
	if (value & LED_DIS_GASCHECK)
	{
		aucSEG_CurServiceValue[3] |= SEG_SIGN_GASCHECK_VALUE;
	}
	else
	{
		aucSEG_CurServiceValue[3] &= (~SEG_SIGN_GASCHECK_VALUE);
	}
	
	//右边三个图标
	//value为状态值   
	// T6亮 T9灭
	if (value & LED_DIS_GAS_FIRE)
	{
		aucSEG_CurServiceValue[4] |= SEG_SIGN_FIRE_VALUE;
		//aucSEG_CurServiceValue[3] &= (~SEG_SIGN_GASCHECK_VALUE);
	}
	else
	{
		aucSEG_CurServiceValue[4] &= (~SEG_SIGN_FIRE_VALUE);
		//aucSEG_CurServiceValue[3] |= SEG_SIGN_GASCHECK_VALUE;
	}

	if (value & LED_DIS_FAN_ROTATION)
		aucSEG_CurServiceValue[4] |= SEG_SIGN_MAN_VALUE;  //人感应标志
	else
		aucSEG_CurServiceValue[4] &= (~SEG_SIGN_MAN_VALUE);

	if (value & LED_DIS_BATTERY)
		aucSEG_CurServiceValue[4] |= SEG_SIGN_BAT_VALUE;
	else
		aucSEG_CurServiceValue[4] &= (~SEG_SIGN_BAT_VALUE);

  //顶部四个图标
	if (value & LED_DIS_SET_RTC)
		aucSEG_CurServiceValue[5] |= SEG_SIGN_RTC_SET_VALUE;
	else
		aucSEG_CurServiceValue[5] &= (~SEG_SIGN_RTC_SET_VALUE);

	if (value & LED_DIS_SET_START_TIME)
		aucSEG_CurServiceValue[5] |= SEG_SIGN_START_TIME_VALUE;
	else
		aucSEG_CurServiceValue[5] &= (~SEG_SIGN_START_TIME_VALUE);

	if (value & LED_DIS_SET_END_TIME)
		aucSEG_CurServiceValue[5] |= SEG_SIGN_FIRE_OFF_VALUE;
	else
		aucSEG_CurServiceValue[5] &= (~SEG_SIGN_FIRE_OFF_VALUE);

	if (value & LED_DIS_SET_SLAVE_CODE)
		aucSEG_CurServiceValue[5] |= SEG_SIGN_COM_ID_VALUE;
	else
		aucSEG_CurServiceValue[5] &= (~SEG_SIGN_COM_ID_VALUE);
}

/****************************************************************************************
* 函数名称：Led_DisplayOn()
* 功能描述: 点亮led
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-10-15 11:5
****************************************************************************************/
void Led_DisplayOn(uint16 value)
{
	ucLed_DisValue |= value;
}

/****************************************************************************************
* 函数名称：Led_DisplayOff()
* 功能描述: 熄灭led
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-10-15 11:5
****************************************************************************************/
void Led_DisplayOff(uint16 value)
{
	ucLed_DisValue &= (~value);
}

/****************************************************************************************
* 函数名称：
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-10-15 11:5
****************************************************************************************/
static void Led_DisplayService(void)
{
#if (defined(HT1621_LED_EN_BLINK))
	static uint8 	ucLastValue = 0;
	static uint8	ucPreValue  = 0xFF;
	static BOOL		bSlowBlinkState = FALSE;
	static BOOL		Bat_bSlowBlinkState = FALSE;
  //T5 燃气泄露状态标识 
	if (ucLed_DisValue & LED_DIS_GAS_LEAK)
	{
		if (BSP_OS_Timeout(uiLed_DisQuickBlinkDly, 1000))
		{
			uiLed_DisQuickBlinkDly = BSP_OS_TimeGet();

			if (ucLastValue & LED_DIS_GAS_LEAK)
				ucLastValue &= (~LED_DIS_GAS_LEAK);
			else
				ucLastValue |= LED_DIS_GAS_LEAK;
		}
	}
	else
		ucLastValue &= ~LED_DIS_GAS_LEAK;
	
//	//T9 气阀关闭标识
//		if (ucLed_DisValue & LED_DIS_GASCHECK)
//	{
//		if (BSP_OS_Timeout(uiLed_DisQuickBlinkDly, 1000))
//		{
//			uiLed_DisQuickBlinkDly = BSP_OS_TimeGet();

//			if (ucLastValue & LED_DIS_GASCHECK)
//				ucLastValue &= (~LED_DIS_GASCHECK);
//			else
//				ucLastValue |= LED_DIS_GASCHECK;
//		}
//	}
//	else
//		ucLastValue &= ~LED_DIS_GASCHECK;
	
	
	//T6 火焰标志状态标识   无火焰时 T9亮
	if (ucLed_DisValue & LED_DIS_GAS_FIRE)
	{
		if (bSlowBlinkState)
		{
			if (BSP_OS_Timeout(uiLed_DisSlowBlinkDly, 2000))
			{
				bSlowBlinkState = FALSE;

				uiLed_DisSlowBlinkDly = BSP_OS_TimeGet();

				if (ucLed_DisValue & LED_DIS_GAS_FIRE)
					ucLastValue &= (~LED_DIS_GAS_FIRE);
			}
		}
		else
		{
			if (BSP_OS_Timeout(uiLed_DisSlowBlinkDly, 1000))
			{
				bSlowBlinkState = TRUE;

				uiLed_DisSlowBlinkDly = BSP_OS_TimeGet();

				if (ucLed_DisValue & LED_DIS_GAS_FIRE)
					ucLastValue |= LED_DIS_GAS_FIRE;
			}
		}
	}

	if (!(ucLed_DisValue & LED_DIS_GAS_FIRE))
		ucLastValue &= ~LED_DIS_GAS_FIRE;

//	if (!(ucLed_DisValue & LED_DIS_FAN_ROTATION))
//		ucLastValue &= ~LED_DIS_FAN_ROTATION;
//    else
//        ucLastValue |= LED_DIS_FAN_ROTATION;
    
    {
		static BOOL		bCurLEDState = FALSE;
		static uint32	uiFanDly;
		//T7 人体检测标识
		if (!(ucLed_DisValue & LED_DIS_FAN_ROTATION))
		{
			//未检测到人
			if ((!bCurLEDState)
				|| (BSP_OS_Timeout(uiFanDly, 10000)))
			{
				bCurLEDState = FALSE;
				ucLastValue &= ~LED_DIS_FAN_ROTATION;
			}
			else
				ucLastValue |= LED_DIS_FAN_ROTATION;
		}
		else
		{
			bCurLEDState = TRUE;
			uiFanDly	 = BSP_OS_TimeGet();

			ucLastValue |= LED_DIS_FAN_ROTATION;
		}
	}
   // T8 电池标识
//	if (ucLed_DisValue & LED_DIS_BATTERY)
//		ucLastValue |= LED_DIS_BATTERY;
//	else
//		ucLastValue &= ~LED_DIS_BATTERY;
//	
	if (ucLed_DisValue & LED_DIS_BATTERY)
	{
		if (Bat_bSlowBlinkState)
		{
			if (BSP_OS_Timeout(uiLed_DisBatSlowBlinkDly, 2000))
			{
				Bat_bSlowBlinkState = FALSE;

				uiLed_DisBatSlowBlinkDly = BSP_OS_TimeGet();

				if (ucLed_DisValue & LED_DIS_BATTERY)
					ucLastValue &= (~LED_DIS_BATTERY);
			}
		}
		else
		{
			if (BSP_OS_Timeout(uiLed_DisBatSlowBlinkDly, 1000))
			{
				Bat_bSlowBlinkState = TRUE;

				uiLed_DisBatSlowBlinkDly = BSP_OS_TimeGet();

				if (ucLed_DisValue & LED_DIS_BATTERY)
					ucLastValue |= LED_DIS_BATTERY;
			}
		}
	}
	if (!(ucLed_DisValue & LED_DIS_BATTERY))
		ucLastValue &= ~LED_DIS_BATTERY;
	
	
   // T1
	if (ucLed_DisValue & LED_DIS_SET_RTC)
		ucLastValue |= LED_DIS_SET_RTC;
	else
		ucLastValue &= ~LED_DIS_SET_RTC;
   //T2
	if (ucLed_DisValue & LED_DIS_SET_START_TIME)
		ucLastValue |= LED_DIS_SET_START_TIME;
	else
		ucLastValue &= ~LED_DIS_SET_START_TIME;
  //T3
	if (ucLed_DisValue & LED_DIS_SET_END_TIME)
		ucLastValue |= LED_DIS_SET_END_TIME;
	else
		ucLastValue &= ~LED_DIS_SET_END_TIME;
   //T4
	if (ucLed_DisValue & LED_DIS_SET_SLAVE_CODE)
		ucLastValue |= LED_DIS_SET_SLAVE_CODE;
	else
		ucLastValue &= ~LED_DIS_SET_SLAVE_CODE;

	if (ucLastValue != ucPreValue)
	{
		ucPreValue = ucLastValue;

		Led_Display(ucLastValue);
	}
#else
    Led_Display(ucLed_DisValue);
#endif  //#if (defined(HT1621_LED_EN_BLINK))
}


/****************************************************************************************
* 函数名称：
* 功能描述: 更新SEG值
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-10-10 22:39
****************************************************************************************/
void LED_SEG_Update(uint8 *pVaulue)
{
	if (pVaulue)
		p_ucLED_SEGValue = pVaulue;
}

/****************************************************************************************
* 函数名称：LED_SEGBlinkSet()
* 功能描述: SEG闪烁设置
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-10-10 22:39
****************************************************************************************/
void LED_SEGBlinkSet(uint8 value)
{
	ucLED_SEGBlinkState = value;

	uiLED_SEGBlinkDly   = BSP_OS_TimeGet();
	bLED_SEGBlinkOnOff  = FALSE;
}

/****************************************************************************************
* 函数名称：LED_SEG_Service()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-10-10 22:39
****************************************************************************************/
static void LED_SEG_Service(void)
{
	BOOL  ret = TRUE;
	uint8 count;
	uint8 value[4];
	static BOOL  bLED_DotBlinkState = FALSE;
	static uint8 a_ucLED_Value[4] = {0xFF, 0xFF, 0xFF, 0xFF};
	static const uint8 *a_pHT1621_CodeTab[] =
	{
		a_ucHT1621_SEG1Code,
		a_ucHT1621_SEG2Code,
		a_ucHT1621_SEG3Code,
		a_ucHT1621_SEG4Code,
	};

	if (!p_ucLED_SEGValue)
		return;

	if (ucLED_SEGBlinkState)
	{
		if (BSP_OS_Timeout(uiLED_SEGBlinkDly, LED_SEG_BLINK_TIME))
		{
			uiLED_SEGBlinkDly  = BSP_OS_TimeGet();
			bLED_SEGBlinkOnOff = bLED_SEGBlinkOnOff ? FALSE : TRUE;
		}
	}

	memcpy(value, p_ucLED_SEGValue, 4);

	if (ucLED_SEGBlinkState && (!bLED_SEGBlinkOnOff))
	{
		for (count = 0; count < 4; count++)
		{
			if (ucLED_SEGBlinkState & (0x01 << count))
			{
				value[count] = 11;
			}
		}

		if (ucLED_SEGBlinkState & 0x10)
			ret = FALSE;
	}

	for (count = 0; count < 4; count++)
	{
		if ((a_ucLED_Value[count] != value[count]) || ((2 == count) && (bLED_DotBlinkState != ret)))
		{
			a_ucLED_Value[count] = value[count];

			if (2 == count)
			{
				bLED_DotBlinkState = ret;
				//数码管数据进行操作
				if (ret)
					aucSEG_CurServiceValue[count] = a_pHT1621_CodeTab[count][value[count]] | HT1621_USER_DEF_VALUE;
				else
					aucSEG_CurServiceValue[count] = a_pHT1621_CodeTab[count][value[count]] & (~HT1621_USER_DEF_VALUE);
			}
			else
				aucSEG_CurServiceValue[count] = a_pHT1621_CodeTab[count][value[count]];
		}
	}
}


/****************************************************************************************
* 函数名称：HT1621_SignService()
* 功能描述: 图标服务函数
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2018-09-15 11:36
****************************************************************************************/
void HT1621_SignService(void)
{
	LED_SEG_Service();

	Led_DisplayService();

	//uint8 count;

	//count = 0;
	//while (count < HT1621_SEG_MAX_ADDR_CNT)
	//{
	//	if (aucSEG_CurServiceValue[0] != aucSEG_PreServiceValue[0])
	//	{
	//		aucSEG_PreServiceValue[0] = aucSEG_CurServiceValue[0];

	//		HT1621_WriteData(aucSEG_ValueAddr[count], &aucSEG_CurServiceValue[count], aucSEG_ValueCnt[count]);
	//	}

	//	count++;
	//}
	//
  //当前值与之前值对比，相同为真  不变
	if (!cmp_buf((char *)aucSEG_CurServiceValue, (char *)aucSEG_PreServiceValue, HT1621_SEG_MAX_ADDR_CNT))
	{
		uint8 buffer[HT1621_SEG_MAX_ADDR_CNT];

		memcpy(aucSEG_PreServiceValue, aucSEG_CurServiceValue, HT1621_SEG_MAX_ADDR_CNT);
		//aucSEG_CurServiceValue[0]表示T5和数码管1        
		//aucSEG_CurServiceValue[1]表示数码管2---PIN3&PIN4(MSB)
		//aucSEG_CurServiceValue[2]表示数码管3---PIN5&PIN6(MSB)
		//aucSEG_CurServiceValue[3]表示数码管4---PIN8&PIN9(MSB)
		//aucSEG_CurServiceValue[4]表示右边三图标
		//aucSEG_CurServiceValue[5]表示顶部图标
		buffer[0] = aucSEG_CurServiceValue[0];
		buffer[1] = aucSEG_CurServiceValue[1];		
		buffer[2] = aucSEG_CurServiceValue[2];
		//buffer[0]的高位代表PIN2数据，低位为PIN1数据
		//buffer[1]的高位代表PIN4数据，低位为PIN3数据
		//buffer[2]的高位代表PIN6数据，低位为PIN5数据
		//buffer[3]的高位代表PIN8数据，低位为PIN7数据
		//buffer[4]的高位代表PIN10数据，低位为PIN9数据
		//按照地址顺序写入数据 PIN1至PIN10
		buffer[3] = ((aucSEG_CurServiceValue[3] & 0x0F) << 4) | (aucSEG_CurServiceValue[4] & 0x0F);		
		buffer[4] = ((aucSEG_CurServiceValue[3] & 0xF0) >> 4) | ((aucSEG_CurServiceValue[5] & 0x0F) << 4);
		//一次性将所有数据写入到HT1261中
		//写连续地址模式 
		//一个地址写入4bit数据，一个buffer存有8bit数据
		HT1621_WriteData(HT1621_SEG1_ADDR, buffer, 10);
	}
}


/*******************************版权所有(c)**************END OF FILE********************/
