/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXXXXXX, 保留所有权
* 文件名      ：platform_SX1212.c
* 作者        ：
* 电子邮箱    ：
* 版本        ：V1.0.0.0
* 日期        ：2017-03-22
* 功能描述    ：RFM64模块使用sx1212芯片底层驱动
* 备注        ：
* 主要函数列表：
* 修改记录    ：
  日      期          版本          修改人          修改内容
  2017-03-22          1.0           KingChou        创建文件	
****************************************************************************************/

/* 调试部分开关 -----------------------------------------------------------------------*/

/* 包含文件 ---------------------------------------------------------------------------*/
#include "app_cfg.h"
#include "CoreInclude.h"
//#include "STM32Fxxx_SPI.h"
#include "STM32Fxxx_Misc.h"

/* 宏定义 -----------------------------------------------------------------------------*/

/* 结构类型定义 -----------------------------------------------------------------------*/
//typedef enum _SX1212_GPIO_e
//{
//	eSX1212_GPIO_PLL_LOCK = 0,												//输入引脚
//	eSX1212_GPIO_MISO,
//	eSX1212_GPIO_IRQ1,
//	eSX1212_GPIO_IRQ0,
//	//eSX1212_GPIO_POR_CHK,
//
//	eSX1212_GPIO_NSS_DATA,													//输出引脚
//	eSX1212_GPIO_RF_DATA,
//	eSX1212_GPIO_NSS_CFG,
//	eSX1212_GPIO_SCK,
//	eSX1212_GPIO_MOSI,
//
//	eSX1212_GPIO_MAX,
//}SX1212_GPIO_e;

typedef struct _SX1212_GPIO_t
{
	GPIO_TypeDef*	port;
	uint16			pin;
	uint16			type;													//0-in;!0-out
}SX1212_GPIO_t;

/* 原型声明部分 -----------------------------------------------------------------------*/

/* 全局变量声明 -----------------------------------------------------------------------*/

/* 静态变量定义 -----------------------------------------------------------------------*/
const static SX1212_GPIO_t a_tSX1212_UseGPIO[] =
{
	//{SX1212_PLL_LOCK_PORT,	SX1212_PLL_LOCK_PIN,	0},
	{SX1212_MISO_PORT,		SX1212_MISO_PIN,		0},
	{SX1212_IRQ1_PORT,		SX1212_IRQ1_PIN,		0},
	{SX1212_IRQ0_PORT,		SX1212_IRQ0_PIN,		0},
	//{SX1212_POR_CHK_PORT,	SX1212_POR_CHK_PIN,		0},

	{SX1212_NSS_DATA_PORT,	SX1212_NSS_DATA_PIN,	1},
	//{SX1212_RF_DATA_PORT,	SX1212_RF_DATA_PIN,		1},
	{SX1212_NSS_CFG_PORT,	SX1212_NSS_CFG_PIN,		1},
	{SX1212_SCK_PORT,		SX1212_SCK_PIN,			1},
	{SX1212_MOSI_PORT,		SX1212_MOSI_PIN,		1},
};

/****************************************************************************************
* 函数名称：SX1212_GPIO_Cmd()
* 功能描述: SX1212的gpio操作
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-3-22 23:8
****************************************************************************************/
//Level_e SX1212_GPIO_Cmd(SX1212_GPIO_e gpio, Level_e level)
//{
//	if (gpio >= eSX1212_GPIO_MAX)
//		return level;
//
//	if (a_tSX1212_UseGPIO[gpio].type)
//	{
//		if (level)
//			GPIO_SetBits(a_tSX1212_UseGPIO[gpio].port, a_tSX1212_UseGPIO[gpio].pin); 
//		else
//			GPIO_ResetBits(a_tSX1212_UseGPIO[gpio].port, a_tSX1212_UseGPIO[gpio].pin);
//
//		return level;
//	}
//	else
//		return ((GPIO_ReadInputDataBit(a_tSX1212_UseGPIO[gpio].port, a_tSX1212_UseGPIO[gpio].pin)) ? HIGH_LEVEL : LOW_LEVEL);
//}

///****************************************************************************************
//* 函数名称：SX1212_GPIORFDataInput()
//* 功能描述: 
//* 入口参数：
//* 返回值  ：
//* 其它    ：
//* 生成时间：2017-3-23 20:23
//****************************************************************************************/
//void SX1212_GPIORFDataInput(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;

//	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Pin		= SX1212_RF_DATA_PIN;
//	GPIO_Init(SX1212_RF_DATA_PORT, &GPIO_InitStructure);
//}

///****************************************************************************************
//* 函数名称：SX1212_GPIORFDataOutput()
//* 功能描述: 
//* 入口参数：
//* 返回值  ：
//* 其它    ：
//* 生成时间：2017-3-23 20:23
//****************************************************************************************/
//void SX1212_GPIORFDataOutput(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;

//	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Pin		= SX1212_RF_DATA_PIN;
//	GPIO_Init(SX1212_RF_DATA_PORT, &GPIO_InitStructure);
//}

#if (defined(SX1212_SPI_MONI_ENABLE) && (SX1212_SPI_MONI_ENABLE))
/****************************************************************************************
* 函数名称：SX1212_DevInit()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-3-22 21:3
****************************************************************************************/
void SX1212_DevInit(void)
{
	uint16				count;
	GPIO_InitTypeDef	GPIO_InitStructure;

#if (defined(APP_OS))
	BSP_PeriphEn(M25PE80_CS_PERIPH_ID);
#else
	//RCC_AHBPeriphClockCmd(SX1212_NSS_DATA_PERIPH_CLK | SX1212_NSS_CFG_PERIPH_CLK | SX1212_PLL_LOCK_PERIPH_CLK |\
	//					  SX1212_RF_DATA_PERIPH_CLK | SX1212_IRQ1_PERIPH_CLK | SX1212_IRQ0_PERIPH_CLK |\
	//					  SX1212_MISO_PERIPH_CLK | SX1212_MOSI_PERIPH_CLK | SX1212_SCK_PERIPH_CLK, ENABLE);

	RCC_AHBPeriphClockCmd(SX1212_NSS_DATA_PERIPH_CLK
						| SX1212_NSS_CFG_PERIPH_CLK
						//| SX1212_PLL_LOCK_PERIPH_CLK 
						//| SX1212_RF_DATA_PERIPH_CLK
						| SX1212_IRQ1_PERIPH_CLK
						| SX1212_IRQ0_PERIPH_CLK
						| SX1212_MISO_PERIPH_CLK
						| SX1212_MOSI_PERIPH_CLK
						| SX1212_SCK_PERIPH_CLK, ENABLE);
#endif	//#if (defined(APP_OS))

	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;

	for (count = 0; count < sizeof(a_tSX1212_UseGPIO) / sizeof(SX1212_GPIO_t); count++)
	{
		GPIO_InitStructure.GPIO_Pin			= a_tSX1212_UseGPIO[count].pin;
		GPIO_Init(a_tSX1212_UseGPIO[count].port, &GPIO_InitStructure);

		if ((!a_tSX1212_UseGPIO[count].type) && (a_tSX1212_UseGPIO[count].type != a_tSX1212_UseGPIO[count + 1].type))
			GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	}
}

/****************************************************************************************
* 函数名称：RFM64_DataSPI8Bit()
* 功能描述: 传送数据8it
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-3-22 23:26
****************************************************************************************/
//static uint8 RFM64_DataSPI8Bit(uint8 byte)
//{
//	uint8 ucBitCnt;
//	uint8 ucRdData = 0;
//
//	//SX1212_GPIO_Cmd(eSX1212_GPIO_NSS_CFG, HIGH_LEVEL);
//	//SX1212_GPIO_Cmd(eSX1212_GPIO_NSS_DATA, LOW_LEVEL);
//	SX1212_GPIO_Cmd(eSX1212_GPIO_SCK, LOW_LEVEL);
//
//	for(ucBitCnt = 8; ucBitCnt > 0; ucBitCnt--)
//	{
//		SX1212_GPIO_Cmd(eSX1212_GPIO_SCK, LOW_LEVEL);
//		ucRdData <<= 1;
//		if(byte & 0x80)
//			SX1212_GPIO_Cmd(eSX1212_GPIO_MOSI, HIGH_LEVEL);
//		else
//			SX1212_GPIO_Cmd(eSX1212_GPIO_MOSI, LOW_LEVEL);
//
//		SX1212_GPIO_Cmd(eSX1212_GPIO_SCK, HIGH_LEVEL);
//		byte <<= 1;
//
//		if(SX1212_GPIO_Cmd(eSX1212_GPIO_MISO, 0))
//			ucRdData |= 0x01;
//	}
//
//	SX1212_GPIO_Cmd(eSX1212_GPIO_SCK, LOW_LEVEL);
//	//SX1212_GPIO_Cmd(eSX1212_GPIO_NSS_DATA, LOW_LEVEL);
//	SX1212_GPIO_Cmd(eSX1212_GPIO_MOSI, HIGH_LEVEL);
//
//	return ucRdData;
//}

/****************************************************************************************
* 函数名称：
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-3-22 23:36
****************************************************************************************/
//uint8 RFM64_CmdSPI16Bit(uint8 wr_rd, uint16 para)
//{
//	uint8	ucBitCnt;
//	uint16	usRdPara = 0;
//	uint16	usSendData;
//
//	SX1212_GPIO_Cmd(eSX1212_GPIO_NSS_DATA, HIGH_LEVEL);
//	SX1212_GPIO_Cmd(eSX1212_GPIO_NSS_CFG, LOW_LEVEL);
//	SX1212_GPIO_Cmd(eSX1212_GPIO_SCK, LOW_LEVEL);
//
//	usSendData = (para & 0xFF00) << 1;
//	if(wr_rd)
//		usSendData |= 0x4000;
//	usSendData |= (para & 0x00FF);
//
//	for(ucBitCnt = 16; ucBitCnt > 0; ucBitCnt--)
//	{
//		SX1212_GPIO_Cmd(eSX1212_GPIO_SCK, LOW_LEVEL);
//		usRdPara <<= 1;
//		if(usSendData & 0x8000)
//			SX1212_GPIO_Cmd(eSX1212_GPIO_MOSI, HIGH_LEVEL);
//		else
//			SX1212_GPIO_Cmd(eSX1212_GPIO_MOSI, LOW_LEVEL);
//
//		SX1212_GPIO_Cmd(eSX1212_GPIO_SCK, HIGH_LEVEL);
//		usSendData <<= 1;
//
//		if(SX1212_GPIO_Cmd(eSX1212_GPIO_MISO, 0))
//			usRdPara |= 0x01;
//	}
//
//	SX1212_GPIO_Cmd(eSX1212_GPIO_SCK, LOW_LEVEL);
//	SX1212_GPIO_Cmd(eSX1212_GPIO_MOSI, HIGH_LEVEL);
//	SX1212_GPIO_Cmd(eSX1212_GPIO_NSS_CFG, HIGH_LEVEL);
//
//	return((uint8)usRdPara);
//}
#else
/****************************************************************************************
* 函数名称：SX1212_SPIInit()
* 功能描述: RFM64模块spi初始化
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-03-22 20:53
****************************************************************************************/
void SX1212_SPIInit(void)
{
	STM32Fxxx_SPIDevInit(RFM64_SPI_DEV, SPI_CPOL_High, SPI_CPHA_2Edge, SPI_BaudRatePrescaler_128);
}

/****************************************************************************************
* 函数名称：SX1212_GPIOInit()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-3-22 21:3
****************************************************************************************/
static void SX1212_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

#if (defined(APP_OS))
	BSP_PeriphEn(SX1212_NSS_DATA_PERIPH_CLK);
	BSP_PeriphEn(SX1212_NSS_CFG_PERIPH_CLK);
#else
	RCC_AHBPeriphClockCmd(SX1212_NSS_DATA_PERIPH_CLK | SX1212_NSS_CFG_PERIPH_CLK, ENABLE);
#endif	//#if (defined(APP_OS))

	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;					
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin		= SX1212_NSS_DATA_PIN;
	GPIO_Init(SX1212_NSS_DATA_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin		= SX1212_NSS_CFG_PIN;
	GPIO_Init(SX1212_NSS_CFG_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin		= SX1212_RF_DATA_PIN;
	GPIO_Init(SX1212_RF_DATA_PORT, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin		= SX1212_PLL_LOCK_PIN;
	GPIO_Init(SX1212_PLL_LOCK_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin		= SX1212_IRQ1_PIN;
	GPIO_Init(SX1212_IRQ1_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin		= SX1212_IRQ0_PIN;
	GPIO_Init(SX1212_IRQ0_PORT, &GPIO_InitStructure);

	SX1212_DEASSERT_CS();
	SX1212_EnSpiCfg();
}

/****************************************************************************************
* 函数名称：SX1212_SPIGPIOInit()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-3-22 21:10
****************************************************************************************/
static void SX1212_SPIGPIOInit(void)
{
	STM32Fxxx_SPIInit(RFM64_SPI_DEV, STM32_SPI_ISR_DIS, NULL);
}

/****************************************************************************************
* 函数名称：SX1212_DevInit()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-3-22 21:3
****************************************************************************************/
void SX1212_DevInit(void)
{
	SX1212_GPIOInit();
	SX1212_SPIGPIOInit();
	SX1212_SPIInit();
}
#endif	//#if (defined(SX1212_SPI_MONI_ENABLE) && (SX1212_SPI_MONI_ENABLE))

///****************************************************************************************
//* 函数名称：SX1212_SPI_WriteByte()
//* 功能描述: 
//* 入口参数：
//* 返回值  ：
//* 其它    ：
//* 生成时间：2017-3-22 21:11
//****************************************************************************************/
//void SX1212_SPI_WriteByte(unsigned char byte)
//{
//#if (defined(SX1212_SPI_MONI_ENABLE) && (SX1212_SPI_MONI_ENABLE))
//	RFM64_DataSPI8Bit(byte);
//#else
//	STM32Fxxx_SPIWriteByte(RFM64_SPI_DEV, byte);
//#endif	//#if (defined(SX1212_SPI_MONI_ENABLE) && (SX1212_SPI_MONI_ENABLE))
//}
//
///****************************************************************************************
//* 函数名称：SX1212_SPI_ReadByte()
//* 功能描述: 
//* 入口参数：
//* 返回值  ：
//* 其它    ：
//* 生成时间：2017-3-22 21:11
//****************************************************************************************/
//unsigned char SX1212_SPI_ReadByte(void)
//{
//	uint8 byte;
//
//#if (defined(SX1212_SPI_MONI_ENABLE) && (SX1212_SPI_MONI_ENABLE))
//	byte = RFM64_DataSPI8Bit(0xFF);
//#else
//	STM32Fxxx_SPIReadByte(RFM64_SPI_DEV, (char *)&byte);
//#endif	//#if (defined(SX1212_SPI_MONI_ENABLE) && (SX1212_SPI_MONI_ENABLE))
//
//	return byte;
//}

/****************************************************************************************
* 函数名称: SX1212_DelayMs()
* 功能描述: 延时
* 入口参数:
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void SX1212_DelayMs(uint32 delay)
{
	BSP_OS_TimeDlyMs(delay);
}

/****************************************************************************************
* 函数名称: SX1212_Delayus()
* 功能描述: 延时
* 入口参数:
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void SX1212_Delayus(uint32 delay)
{
	STM32Fxxx_CountDelayus(delay);
}

/****************************************************************************************
* 函数名称：SX1212_MISOGet()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-3-24 22:25
****************************************************************************************/
unsigned char SX1212_MISOGet(void)
{
	return ((GPIO_ReadInputDataBit(SX1212_MISO_PORT, SX1212_MISO_PIN)) ? 1 : 0);
}

/****************************************************************************************
* 函数名称：SX1212_MOSISet()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-3-24 22:25
****************************************************************************************/
void SX1212_MOSISet(unsigned char i)
{
	if (i)
		GPIO_SetBits(SX1212_MOSI_PORT, SX1212_MOSI_PIN);
	else
		GPIO_ResetBits(SX1212_MOSI_PORT, SX1212_MOSI_PIN);
}

/****************************************************************************************
* 函数名称：SX1212_SCKSet()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-3-24 22:25
****************************************************************************************/
void SX1212_SCKSet(unsigned char i)
{
	if (i)
		GPIO_SetBits(SX1212_SCK_PORT, SX1212_SCK_PIN);
	else
		GPIO_ResetBits(SX1212_SCK_PORT, SX1212_SCK_PIN);
}

/****************************************************************************************
* 函数名称：SX1212_NSSCfgSet()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-3-24 22:25
****************************************************************************************/
void SX1212_NSSCfgSet(unsigned char i)
{
	if (i)
		GPIO_SetBits(SX1212_NSS_CFG_PORT, SX1212_NSS_CFG_PIN);
	else
		GPIO_ResetBits(SX1212_NSS_CFG_PORT, SX1212_NSS_CFG_PIN);
}

/****************************************************************************************
* 函数名称：SX1212_NSSDataSet()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-3-24 22:25
****************************************************************************************/
void SX1212_NSSDataSet(unsigned char i)
{
	if (i)
		GPIO_SetBits(SX1212_NSS_DATA_PORT, SX1212_NSS_DATA_PIN);
	else
		GPIO_ResetBits(SX1212_NSS_DATA_PORT, SX1212_NSS_DATA_PIN);
}

/****************************************************************************************
* 函数名称：SX1212_IRQ1Get()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-3-24 22:25
****************************************************************************************/
unsigned char SX1212_IRQ1Get(void)
{
	return ((GPIO_ReadInputDataBit(SX1212_IRQ1_PORT, SX1212_IRQ1_PIN)) ? 1 : 0);
}

/****************************************************************************************
* 函数名称：SX1212_IRQ0Get()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-3-24 22:25
****************************************************************************************/
unsigned char SX1212_IRQ0Get(void)
{
	return ((GPIO_ReadInputDataBit(SX1212_IRQ0_PORT, SX1212_IRQ0_PIN)) ? 1 : 0);
}

/*******************************版权所有(c)**************END OF FILE********************/
