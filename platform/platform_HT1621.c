/************************************��Ȩ����(c)****************************************
* ��Ȩ����(c), 2012-2013, XXXXXXXX��˾, ��������Ȩ
* �ļ���      ��platform_HT1621.c
* ����        ��XXXXXX
* ��������    ��XXXXXX
* �汾        ��V1.0.0.0
* ����        ��2018-09-07
* ��������    ��
* ��ע        ��
* ��Ҫ�����б�
* �޸ļ�¼    ��
  ��      ��          �汾          �޸���          �޸�����
  2018-09-07          1.0           XXXXXXXX        �����ļ�	
****************************************************************************************/

/* ���Բ��ֿ��� -----------------------------------------------------------------------*/

/* �����ļ� ---------------------------------------------------------------------------*/
#include "app_cfg.h"
#include "CoreInclude.h"

#include "STM32Fxxx_Misc.h"
#include <stdint.h>
#include <string.h>

/* �궨�� -----------------------------------------------------------------------------*/
#define HT1621_CS_PERIPH_ID					RCC_AHBPeriph_GPIOA
#define HT1621_CS_PORT						GPIOA
#define HT1621_CS_PIN						GPIO_Pin_4

#define HT1621_WR_PERIPH_ID					RCC_AHBPeriph_GPIOA
#define HT1621_WR_PORT						GPIOA
#define HT1621_WR_PIN						GPIO_Pin_6

#define HT1621_DAT_PERIPH_ID				RCC_AHBPeriph_GPIOA
#define HT1621_DAT_PORT						GPIOA
#define HT1621_DAT_PIN						GPIO_Pin_5

#define HT1621_BLK_PERIPH_ID				RCC_AHBPeriph_GPIOF
#define HT1621_BLK_PORT						GPIOF
#define HT1621_BLK_PIN						GPIO_Pin_7


#define HT1621_CS_Set()						GPIO_SetBits(HT1621_CS_PORT, HT1621_CS_PIN)
#define HT1621_CS_Reset()					GPIO_ResetBits(HT1621_CS_PORT, HT1621_CS_PIN)

#define HT1621_WR_Set()						GPIO_SetBits(HT1621_WR_PORT, HT1621_WR_PIN)
#define HT1621_WR_Reset()					GPIO_ResetBits(HT1621_WR_PORT, HT1621_WR_PIN)

#define HT1621_DAT_Set()					GPIO_SetBits(HT1621_DAT_PORT, HT1621_DAT_PIN)
#define HT1621_DAT_Reset()					GPIO_ResetBits(HT1621_DAT_PORT, HT1621_DAT_PIN)


#define HT1621_WRIRE_MODE					0x05
#define HT1621_CMD_MODE						0x04
#define HT1621_READ_MODE					0x06

//#define BIAS								0x24
//#define SYSEN								0x01
//#define LCDOFF								0x02
//#define LCDON								0x03 

#define BIAS								0x52 //LCD 1/3bias
#define XTAL								0x30 //LCD ʱ��Դ32K
#define WDTDIS								0x0A //LCD ���Ź���ֹ
#define TMDIS								0x08 //LCD ��ֹʱ�����
#define SYSEN								0x02 //LCD ��ϵͳ��
#define LCDON								0x06 //��LCDƫ�÷�����
#define LCDOFF								0x04 //�ر�LCDƫ�÷�����
#define SYSDIS								0x00 //�ر�LCDϵͳ��

/* �ṹ���Ͷ��� -----------------------------------------------------------------------*/

/* ԭ���������� -----------------------------------------------------------------------*/

/* ȫ�ֱ������� -----------------------------------------------------------------------*/

/* ��̬�������� -----------------------------------------------------------------------*/

/****************************************************************************************
* ��������:
* ��������:
* ��ڲ���:
* ���ڲ���:
* ����ֵ  :
* ����    :
****************************************************************************************/
static void HT1621_Delay(void)
{
	uint16 delay = 500;
	while(delay--);
}

/****************************************************************************************
* �������ƣ�
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2018-9-7 23:25
****************************************************************************************/
void HT1621_WriteCmd(uint8 cmd)
{
	uint16 count, temp;

	HT1621_CS_Reset();

	for (count = 0, temp = 0x04; count < 3; count++, temp >>= 1)
	{
		if (HT1621_CMD_MODE & temp)
			HT1621_DAT_Set();
		else
			HT1621_DAT_Reset();

		HT1621_WR_Reset();
		HT1621_Delay();
		HT1621_WR_Set();
		HT1621_Delay();
	}

	for (count = 0, temp = 0x100; count < 9; count++, temp >>= 1)
	{
		if (cmd & temp)
			HT1621_DAT_Set();
		else
			HT1621_DAT_Reset();

		HT1621_WR_Reset();
		HT1621_Delay();
		HT1621_WR_Set();
		HT1621_Delay();
	}

	HT1621_CS_Set();
	HT1621_DAT_Set();
}

/****************************************************************************************
* �������ƣ�
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2018-9-7 23:25
****************************************************************************************/
void HT1621_WriteData(uint8 addr, const uint8 *const buffer, uint16 length)
{
	uint8  count, temp, value;
	uint16 index;

	HT1621_CS_Reset();

	for (count = 0, temp = 0x04; count < 3; count++, temp >>= 1)
	{
		if (HT1621_WRIRE_MODE & temp)
			HT1621_DAT_Set();
		else
			HT1621_DAT_Reset();

		HT1621_WR_Reset();
		HT1621_Delay();
		HT1621_WR_Set();
		HT1621_Delay();
	}
  //д����ʼ��ַ
	for (count = 0, temp = 0x20; count < 6; count++, temp >>= 1)
	{
		if (addr & temp)      //write addr
			HT1621_DAT_Set();     
		else
			HT1621_DAT_Reset();

		HT1621_WR_Reset();
		HT1621_Delay();
		HT1621_WR_Set();
		HT1621_Delay();
	}

	//����lcd��ʾ����
	for (index = 0; index < length; index++)
	{
		if (index & 0x01)
			value = buffer[index >> 1] >> 4;
		else
			value = buffer[index >> 1] & 0x0F;  //ȡ��λ

		for (count = 0, temp = 0x01; count < 4 ; count++, temp <<= 1)
		{
			(value & temp) ? HT1621_DAT_Set() : HT1621_DAT_Reset();   //write data

			HT1621_WR_Reset();
			HT1621_Delay();
			HT1621_WR_Set();
			HT1621_Delay();
		}
	}

	HT1621_CS_Set();
	HT1621_DAT_Set();
}

/****************************************************************************************
* �������ƣ�HT1621_GPIOInit()
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2018-09-07 22:49
****************************************************************************************/
static void HT1621_GPIOInit(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

#if (defined(APP_OS))
	BSP_PeriphEn(HT1621_CS_PERIPH_ID);
	BSP_PeriphEn(HT1621_WR_PERIPH_ID);
	BSP_PeriphEn(HT1621_DAT_PERIPH_ID);
	BSP_PeriphEn(HT1621_BLK_PERIPH_ID);
#else
	RCC_AHBPeriphClockCmd(HT1621_CS_PERIPH_ID | HT1621_WR_PERIPH_ID | HT1621_DAT_PERIPH_ID | HT1621_BLK_PERIPH_ID, ENABLE);
#endif	//#if (defined(APP_OS))

	GPIO_InitStructure.GPIO_Pin		= HT1621_CS_PIN;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(HT1621_CS_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin		= HT1621_WR_PIN;
	GPIO_Init(HT1621_WR_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin		= HT1621_DAT_PIN;
	GPIO_Init(HT1621_DAT_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin		= HT1621_BLK_PIN;
	GPIO_Init(HT1621_BLK_PORT, &GPIO_InitStructure);

	GPIO_SetBits(HT1621_BLK_PORT, HT1621_BLK_PIN);
}

/****************************************************************************************
* �������ƣ�
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2018-9-7 23:43
****************************************************************************************/
void HT1621_Init(void)
{
	HT1621_GPIOInit();

	HT1621_WriteCmd(BIAS);
	HT1621_WriteCmd(SYSEN);
	HT1621_WriteCmd(LCDON);

//	HT1621_WriteCmd(BIAS);
//	HT1621_WriteCmd(XTAL);
//	HT1621_WriteCmd(WDTDIS);
//	HT1621_WriteCmd(TMDIS);
//	HT1621_WriteCmd(SYSEN);
//	HT1621_WriteCmd(LCDON);
    
	{
		uint8 buffer[16], i;

		memset(buffer, 0, sizeof(buffer));

		HT1621_WriteData(0, buffer, sizeof(buffer) << 1);

		memset(buffer, 0xFF, sizeof(buffer));

		//HT1621_WriteData(0, buffer, sizeof(buffer) << 1);

		for (i = 0; i < 32; i += 2)
			HT1621_WriteData(i, &buffer[i >> 1], 2);
	}
}


/*******************************��Ȩ����(c)**************END OF FILE********************/
