/************************************��Ȩ����(c)****************************************
* ��Ȩ����(c), 2012-2013, XXXXXXXX��˾, ��������Ȩ
* �ļ���      ��STM32Fxxx_RTC.c
* ����        ��XXXXXX
* ��������    ��XXXXXX
* �汾        ��V1.0.0.0
* ����        ��2017-10-06
* ��������    ��
* ��ע        ��
* ��Ҫ�����б�
* �޸ļ�¼    ��
  ��      ��          �汾          �޸���          �޸�����
  2017-10-06          1.0           XXXXXXXX        �����ļ�	
****************************************************************************************/

/* ���Բ��ֿ��� -----------------------------------------------------------------------*/

/* �����ļ� ---------------------------------------------------------------------------*/
#include "STM32Fxxx_RTC.h"
#include "CoreInclude.h"


/* �궨�� -----------------------------------------------------------------------------*/
#define BKP_VALUE						0x32F0

/* �ṹ���Ͷ��� -----------------------------------------------------------------------*/

/* ԭ���������� -----------------------------------------------------------------------*/

/* ȫ�ֱ������� -----------------------------------------------------------------------*/

/* ��̬�������� -----------------------------------------------------------------------*/

/****************************************************************************************
* �������ƣ�STM32Fxxx_RTCInit()
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-10-06 15:07
****************************************************************************************/
BOOL STM32Fxxx_RTCInit(void)
{
	RTC_InitTypeDef RTC_InitStructure;

	if(RTC_ReadBackupRegister(RTC_BKP_DR0) != BKP_VALUE)
	{
		/* Enable the PWR clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

		/* Allow access to RTC */
		PWR_BackupAccessCmd(ENABLE);

		/* Enable the LSE OSC */
		RCC_LSEConfig(RCC_LSE_ON);

		/* Wait till LSE is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{
		}

		/* Select the RTC Clock Source */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

		/* Enable the RTC Clock */
		RCC_RTCCLKCmd(ENABLE);

		/* Wait for RTC APB registers synchronisation */
		RTC_WaitForSynchro();

		/* Configure the RTC data register and RTC prescaler */
		RTC_InitStructure.RTC_AsynchPrediv	= 0x7F;
		RTC_InitStructure.RTC_SynchPrediv	= 0xFF;
		RTC_InitStructure.RTC_HourFormat	= RTC_HourFormat_24;

		if (RTC_Init(&RTC_InitStructure) == ERROR)
			return FALSE;

		RTC_WriteBackupRegister(RTC_BKP_DR0, BKP_VALUE);
	}
	else
	{
		/* Enable the PWR clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

		/* Allow access to RTC */
		PWR_BackupAccessCmd(ENABLE);

		/* Wait for RTC APB registers synchronisation */
		RTC_WaitForSynchro();

		/* Clear the RTC Alarm Flag */
		RTC_ClearFlag(RTC_FLAG_ALRAF);

		/* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
		//	    EXTI_ClearITPendingBit(EXTI_Line17);
	}

	return TRUE;
}

/****************************************************************************************
* �������ƣ�BSP_RTCSet()
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-10-6 15:9
****************************************************************************************/
BOOL BSP_RTCSet(RTCTime *rtc)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;

	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);

	RTC_DateStructure.RTC_Year		= rtc->year;
	RTC_DateStructure.RTC_Month		= rtc->month;
	RTC_DateStructure.RTC_Date		= rtc->day;
	RTC_TimeStructure.RTC_Hours		= rtc->hour;
	RTC_TimeStructure.RTC_Minutes	= rtc->minute;
	RTC_TimeStructure.RTC_Seconds	= rtc->second;
	RTC_TimeStructure.RTC_H12		= RTC_H12_AM;

	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);

	return TRUE;
}

/****************************************************************************************
* �������ƣ�BSP_RTCGet()
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-10-6 15:10
****************************************************************************************/
BOOL BSP_RTCGet(RTCTime *rtc)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;

	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);

	rtc->year	= RTC_DateStructure.RTC_Year;
	rtc->month	= RTC_DateStructure.RTC_Month;
	rtc->day	= RTC_DateStructure.RTC_Date;
	rtc->hour	= RTC_TimeStructure.RTC_Hours;
	rtc->minute = RTC_TimeStructure.RTC_Minutes;
	rtc->second = RTC_TimeStructure.RTC_Seconds;

	return TRUE;
}

/*******************************��Ȩ����(c)**************END OF FILE********************/
