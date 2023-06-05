/************************************��Ȩ����(c)****************************************
* ��Ȩ����(c), 2012-2013, XXXXXXXX��˾, ��������Ȩ
* �ļ���      ��HT1621.c
* ����        ��XXXXXX
* ��������    ��XXXXXX
* �汾        ��V1.0.0.0
* ����        ��2018-09-15
* ��������    ��
* ��ע        ��
* ��Ҫ�����б�
* �޸ļ�¼    ��
  ��      ��          �汾          �޸���          �޸�����
  2018-09-15          1.0           XXXXXXXX        �����ļ�	
****************************************************************************************/

/* ���Բ��ֿ��� -----------------------------------------------------------------------*/
#define HT1621_LED_EN_BLINK

/* �����ļ� ---------------------------------------------------------------------------*/
#include "HT1621.h"

#include "utilities.h"
#include <string.h>

#include "STM32Fxxx_Misc.h"

/* �궨�� -----------------------------------------------------------------------------*/
#define HT1621_USER_DEF_VALUE			0x01		//С�����������ͼ��

#define HT1621_SEG1_ADDR				0x16		//��һ�����֣�ȼ��й©ͼ�꣨�����壩
#define HT1621_SEG2_ADDR				0x18		//�ڶ������֣�ȼ��й©ͼ�꺺��
#define HT1621_SEG3_ADDR				0x1A		//���������֣��м�ð��
#define HT1621_SEG4_ADDR				0x1D		//���ĸ�����

#define HT1621_RIGHT_SIGN_ADDR			0x1C		//�ұ�����ͼ���ַ
#define HT1621_TOP_SIGN_ADDR			0x1F		//����4��ͼ��
#define HT1621_GASCHECK_SIGN_ADDR			0x1E  //�����رյ�ַ		

#define HT1621_SEG_MAX_ADDR_CNT			6			//�������ַ

/*****************************�����ұ�ͼ���ַ��HT1621_RIGHT_SIGN_ADDR*****************************/
#define SEG_SIGN_BAT_VALUE				0x02		//���ͼ��
#define SEG_SIGN_MAN_VALUE				0x04		//�����Ӧͼ��
#define SEG_SIGN_FIRE_VALUE				0x08		//����ȼ��ͼ��
#define SEG_SIGN_GASCHECK_VALUE   0x10    //�����ر�ͼ�� PIN9(MSB)
/*****************************�����ұ�ͼ���ַ��HT1621_RIGHT_SIGN_ADDR*****************************/

/******************************���ڶ���ͼ���ַ��HT1621_TOP_SIGN_ADDR******************************/
#define SEG_SIGN_RTC_SET_VALUE			0x01		//ʱ������ͼ��
#define SEG_SIGN_START_TIME_VALUE		0x02		//����ʱ��ͼ��
#define SEG_SIGN_FIRE_OFF_VALUE			0x04		//Ϩ��ʱ��ͼ��
#define SEG_SIGN_COM_ID_VALUE			0x08		//ͨѶ����ͼ��
/******************************���ڶ���ͼ���ַ��HT1621_TOP_SIGN_ADDR******************************/

/* �ṹ���Ͷ��� -----------------------------------------------------------------------*/

/* ԭ���������� -----------------------------------------------------------------------*/

/* ȫ�ֱ������� -----------------------------------------------------------------------*/

/* ��̬�������� -----------------------------------------------------------------------*/
static const uint8 a_ucHT1621_SEG1Code[] =                         //ʱ�������
{
	0xFA, 0x60, 0xD6, 0xF4, 0x6C, 0xBC, 0xBE, 0xE0, 0xFE, 0xFC,		//0-9
	0x04,															//-
	0x00,															//Ϩ��
};

static const uint8 a_ucHT1621_SEG2Code[] =
{
	0xFA, 0x60, 0xD6, 0xF4, 0x6C, 0xBC, 0xBE, 0xE0, 0xFE, 0xFC,		//0-9
	0x04,															//-
	0x00,															//Ϩ��
};
static const uint8 a_ucHT1621_SEG3Code[] =
{
	0xFA, 0x60, 0xD6, 0xF4, 0x6C, 0xBC, 0xBE, 0xE0, 0xFE, 0xFC,		//0-9
	0x04,															//-
	0x00,															//Ϩ��
};

static const uint8 a_ucHT1621_SEG4Code[] =
{
	0xAF, 0x06, 0x6D, 0x4F, 0xC6, 0xCB, 0xEB, 0x0E, 0xEF, 0xCF,		//0-9
	0x40,															//-
	0x00,															//Ϩ��
};

static uint8 aucSEG_PreServiceValue[HT1621_SEG_MAX_ADDR_CNT] =		//��һ��д��õ�ַ��ֵ
{
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

static uint8 aucSEG_CurServiceValue[HT1621_SEG_MAX_ADDR_CNT] =		//��ǰӦ��д��ĵ�ַ������
{
	0, 0, 0, 0, 0, 0,
};

//static const uint8 aucSEG_ValueAddr[HT1621_SEG_MAX_ADDR_CNT]=		//��ַ
//{
//	HT1621_SEG1_ADDR,
//	HT1621_SEG2_ADDR,
//	HT1621_SEG3_ADDR,
//	HT1621_SEG4_ADDR,

//	HT1621_RIGHT_SIGN_ADDR,
//	HT1621_TOP_SIGN_ADDR,
//};

//static const uint8 aucSEG_ValueCnt[HT1621_SEG_MAX_ADDR_CNT]=		//д�뵱ǰ��ַ�����ݳ���
//{
//	2,
//	2,
//	2,
//	2,

//	1,
//	1,
//};

/***********************************LED����*************************************/
static uint8  ucLed_DisValue = 0;

#if (defined(HT1621_LED_EN_BLINK))
static uint32 uiLed_DisQuickBlinkDly;
static uint32 uiLed_DisSlowBlinkDly;
static uint32 uiLed_DisBatSlowBlinkDly;
#endif  //#if (defined(HT1621_LED_EN_BLINK))
/***********************************LED����*************************************/

/***********************************SEG����*************************************/
static uint8    *p_ucLED_SEGValue = NULL;

static BOOL     bLED_SEGBlinkOnOff  = FALSE;
static uint8    ucLED_SEGBlinkState = 0;                               //��Bitλ��ʾ��˸λ
static uint32   uiLED_SEGBlinkDly   = 0;
/***********************************SEG����*************************************/

/****************************************************************************************
* �������ƣ�
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-10-15 11:5
****************************************************************************************/
static void Led_Display(uint16 value)
{
	//ú��й¶ͼ�� aucSEG_CurServiceValue[0]/[1]����������ܵ�����
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
	
	//T9 �����ر�״̬
	if (value & LED_DIS_GASCHECK)
	{
		aucSEG_CurServiceValue[3] |= SEG_SIGN_GASCHECK_VALUE;
	}
	else
	{
		aucSEG_CurServiceValue[3] &= (~SEG_SIGN_GASCHECK_VALUE);
	}
	
	//�ұ�����ͼ��
	//valueΪ״ֵ̬   
	// T6�� T9��
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
		aucSEG_CurServiceValue[4] |= SEG_SIGN_MAN_VALUE;  //�˸�Ӧ��־
	else
		aucSEG_CurServiceValue[4] &= (~SEG_SIGN_MAN_VALUE);

	if (value & LED_DIS_BATTERY)
		aucSEG_CurServiceValue[4] |= SEG_SIGN_BAT_VALUE;
	else
		aucSEG_CurServiceValue[4] &= (~SEG_SIGN_BAT_VALUE);

  //�����ĸ�ͼ��
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
* �������ƣ�Led_DisplayOn()
* ��������: ����led
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-10-15 11:5
****************************************************************************************/
void Led_DisplayOn(uint16 value)
{
	ucLed_DisValue |= value;
}

/****************************************************************************************
* �������ƣ�Led_DisplayOff()
* ��������: Ϩ��led
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-10-15 11:5
****************************************************************************************/
void Led_DisplayOff(uint16 value)
{
	ucLed_DisValue &= (~value);
}

/****************************************************************************************
* �������ƣ�
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-10-15 11:5
****************************************************************************************/
static void Led_DisplayService(void)
{
#if (defined(HT1621_LED_EN_BLINK))
	static uint8 	ucLastValue = 0;
	static uint8	ucPreValue  = 0xFF;
	static BOOL		bSlowBlinkState = FALSE;
	static BOOL		Bat_bSlowBlinkState = FALSE;
  //T5 ȼ��й¶״̬��ʶ 
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
	
//	//T9 �����رձ�ʶ
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
	
	
	//T6 �����־״̬��ʶ   �޻���ʱ T9��
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
		//T7 �������ʶ
		if (!(ucLed_DisValue & LED_DIS_FAN_ROTATION))
		{
			//δ��⵽��
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
   // T8 ��ر�ʶ
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
* �������ƣ�
* ��������: ����SEGֵ
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-10-10 22:39
****************************************************************************************/
void LED_SEG_Update(uint8 *pVaulue)
{
	if (pVaulue)
		p_ucLED_SEGValue = pVaulue;
}

/****************************************************************************************
* �������ƣ�LED_SEGBlinkSet()
* ��������: SEG��˸����
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-10-10 22:39
****************************************************************************************/
void LED_SEGBlinkSet(uint8 value)
{
	ucLED_SEGBlinkState = value;

	uiLED_SEGBlinkDly   = BSP_OS_TimeGet();
	bLED_SEGBlinkOnOff  = FALSE;
}

/****************************************************************************************
* �������ƣ�LED_SEG_Service()
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-10-10 22:39
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
				//��������ݽ��в���
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
* �������ƣ�HT1621_SignService()
* ��������: ͼ�������
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2018-09-15 11:36
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
  //��ǰֵ��֮ǰֵ�Աȣ���ͬΪ��  ����
	if (!cmp_buf((char *)aucSEG_CurServiceValue, (char *)aucSEG_PreServiceValue, HT1621_SEG_MAX_ADDR_CNT))
	{
		uint8 buffer[HT1621_SEG_MAX_ADDR_CNT];

		memcpy(aucSEG_PreServiceValue, aucSEG_CurServiceValue, HT1621_SEG_MAX_ADDR_CNT);
		//aucSEG_CurServiceValue[0]��ʾT5�������1        
		//aucSEG_CurServiceValue[1]��ʾ�����2---PIN3&PIN4(MSB)
		//aucSEG_CurServiceValue[2]��ʾ�����3---PIN5&PIN6(MSB)
		//aucSEG_CurServiceValue[3]��ʾ�����4---PIN8&PIN9(MSB)
		//aucSEG_CurServiceValue[4]��ʾ�ұ���ͼ��
		//aucSEG_CurServiceValue[5]��ʾ����ͼ��
		buffer[0] = aucSEG_CurServiceValue[0];
		buffer[1] = aucSEG_CurServiceValue[1];		
		buffer[2] = aucSEG_CurServiceValue[2];
		//buffer[0]�ĸ�λ����PIN2���ݣ���λΪPIN1����
		//buffer[1]�ĸ�λ����PIN4���ݣ���λΪPIN3����
		//buffer[2]�ĸ�λ����PIN6���ݣ���λΪPIN5����
		//buffer[3]�ĸ�λ����PIN8���ݣ���λΪPIN7����
		//buffer[4]�ĸ�λ����PIN10���ݣ���λΪPIN9����
		//���յ�ַ˳��д������ PIN1��PIN10
		buffer[3] = ((aucSEG_CurServiceValue[3] & 0x0F) << 4) | (aucSEG_CurServiceValue[4] & 0x0F);		
		buffer[4] = ((aucSEG_CurServiceValue[3] & 0xF0) >> 4) | ((aucSEG_CurServiceValue[5] & 0x0F) << 4);
		//һ���Խ���������д�뵽HT1261��
		//д������ַģʽ 
		//һ����ַд��4bit���ݣ�һ��buffer����8bit����
		HT1621_WriteData(HT1621_SEG1_ADDR, buffer, 10);
	}
}


/*******************************��Ȩ����(c)**************END OF FILE********************/
