/************************************��Ȩ����(c)****************************************
* ��Ȩ����(c), 2012-2013, XXXXXXXX��˾, ��������Ȩ
* �ļ���      ��KeyBoard.c
* ����        ��XXXXXX
* ��������    ��XXXXXX
* �汾        ��V1.0.0.0
* ����        ��2017-10-14
* ��������    ��
* ��ע        ��
* ��Ҫ�����б�
* �޸ļ�¼    ��
  ��      ��          �汾          �޸���          �޸�����
  2017-10-14          1.0           XXXXXXXX        �����ļ�	
****************************************************************************************/

/* ���Բ��ֿ��� -----------------------------------------------------------------------*/

/* �����ļ� ---------------------------------------------------------------------------*/
#include "KeyBoard.h"
#include "ChordTone.h"

#include "CoreInclude.h"
#include "app_cfg.h"

#include "STM32Fxxx_Misc.h"

/* �궨�� -----------------------------------------------------------------------------*/
//#if (KEYBOARD_MAX_CNT < 1)
//#error "error key cnt."
//#endif  //KEYBOARD_MAX_CNT
//
#define KEYBOARD_CHK_PUSH					(0u)						//�����û�а�������
#define KEYBOARD_DEBOUNCE					(1u)						//��������
#define KEYBOARD_WAIT_RELEASE				(2u)						//�ȴ������ͷ�

#define KEYBOARD_STATE_NO					((uint8)0xFF)				//�ް�������

/* �ṹ���Ͷ��� -----------------------------------------------------------------------*/

/* ԭ���������� -----------------------------------------------------------------------*/

/* ȫ�ֱ������� -----------------------------------------------------------------------*/

/* ��̬�������� -----------------------------------------------------------------------*/
//static uint32 a_uiKeyBoard_DebounceDly[KEYBOARD_MAX_CNT];
//static uint8  a_ucKeyBoard_State[KEYBOARD_MAX_CNT];

static void		(*a_pfnKeyBoard_Fun[KEYBOARD_MAX_CNT])(void);

static uint8	ucKeyBoard_State = KEYBOARD_CHK_PUSH;
static uint8	ucKeyBoard_KeyValue;
static uint32	uiKeyBoard_DebounceDly;

static const uint8 a_ucKeyBoard_Code[] = 
{
	//0x00,			//����K1
	//0x01,			//����K2
	//0x02,			//����K3
	//0x03,			//����K4

//	0x04,			//����K5
//	0x05,			//����K6
//	0x06,			//����K7
//	0x07,			//����K8
//	
//	0x10,			//����K5��K8
    
    0x01,			//����K1
    0x02,			//����K2
    0x04,			//����K3
    0x08,			//����K4
    0x05,			//����K1,K4
	  0x0C,     //����K1,K2(TH2&TH1)
};

/****************************************************************************************
* �������ƣ�KeyBoard_InState()
* ����������
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-04-25 11:27
****************************************************************************************/
uint8 KeyBoard_InState(void)
{
	uint8 value = 0;

    if (Bit_RESET == GPIO_ReadInputDataBit(KEY_BOARD_1PIN_PORT, KEY_BOARD_1PIN))
		value |= 0x01;
    if (Bit_RESET == GPIO_ReadInputDataBit(KEY_BOARD_2PIN_PORT, KEY_BOARD_2PIN))
		value |= 0x02;
    if (Bit_RESET == GPIO_ReadInputDataBit(KEY_BOARD_3PIN_PORT, KEY_BOARD_3PIN))
		value |= 0x04;
    if (Bit_RESET == GPIO_ReadInputDataBit(KEY_BOARD_4PIN_PORT, KEY_BOARD_4PIN))
		value |= 0x08;
    
//	if (Bit_RESET == GPIO_ReadInputDataBit(KEY_BOARD_4PIN_PORT, KEY_BOARD_4PIN))
//	{
//		value = 0;

//		if (GPIO_ReadInputDataBit(KEY_BOARD_1PIN_PORT, KEY_BOARD_1PIN))
//			value |= 0x01;

//		if (GPIO_ReadInputDataBit(KEY_BOARD_2PIN_PORT, KEY_BOARD_2PIN))
//			value |= 0x02;

//		if (GPIO_ReadInputDataBit(KEY_BOARD_3PIN_PORT, KEY_BOARD_3PIN))
//			value |= 0x04;
//	}

	if (!value)
		return KEYBOARD_STATE_NO;
	else
		return value;
}

/****************************************************************************************
* �������ƣ�KeyBoard_Service()
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-10-14 23:35
****************************************************************************************/
void KeyBoard_Service(void)
{
	if (KEYBOARD_CHK_PUSH == ucKeyBoard_State)
	{
		ucKeyBoard_KeyValue	= KeyBoard_InState();

		if (KEYBOARD_STATE_NO != ucKeyBoard_KeyValue)
		{
			uiKeyBoard_DebounceDly	= BSP_OS_TimeGet();
			ucKeyBoard_State		= KEYBOARD_DEBOUNCE;
		}
	}
	//���� ��ʱ
	else if (KEYBOARD_DEBOUNCE == ucKeyBoard_State)
	{
        uint8 value = KeyBoard_InState();
        
		if (BSP_OS_Timeout(uiKeyBoard_DebounceDly, KEYBOARD_DEBOUNCE_DLY))
		{
			if (value == ucKeyBoard_KeyValue)
			{
				uiKeyBoard_DebounceDly	= BSP_OS_TimeGet();
				ucKeyBoard_State		= KEYBOARD_WAIT_RELEASE;
			}
			else
			{
				ucKeyBoard_State	= KEYBOARD_CHK_PUSH;
				ucKeyBoard_KeyValue	= KEYBOARD_STATE_NO;
			}
		}
        else
        {
            if (value != ucKeyBoard_KeyValue)
			{
				ucKeyBoard_State	= KEYBOARD_CHK_PUSH;
				ucKeyBoard_KeyValue	= KEYBOARD_STATE_NO;
			}
        }
	}
	else if (KEYBOARD_WAIT_RELEASE == ucKeyBoard_State)
	{
		BOOL  ret   = FALSE;
		uint8 value = KeyBoard_InState();

		if (ucKeyBoard_KeyValue != value)
			ret = TRUE;
		else
		{
			if (BSP_OS_Timeout(uiKeyBoard_DebounceDly, KEYBOARD_WAIT_RELEASE_TIME))
				ret = TRUE;
		}

		if (ret)
		{
			uint8 count = 0;

			while (count < KEYBOARD_MAX_CNT)
			{
				if (ucKeyBoard_KeyValue == a_ucKeyBoard_Code[count])
				{
					if (a_pfnKeyBoard_Fun[count])
						a_pfnKeyBoard_Fun[count]();

					{
						ChordTone_Music_e a_tMusic[] =
						{
							eCHORD_TONE_MUSIC_PWR_ON,
							eCHORD_TONE_MUSIC_PWR_OFF,
							eCHORD_TONE_MUSIC_SONG1,
							eCHORD_TONE_MUSIC_SONG2,
							eCHORD_TONE_MUSIC_SONG3,
							eCHORD_TONE_MUSIC_SONG4,
						};

						if (count < sizeof(a_tMusic) / sizeof(ChordTone_Music_e))
							ChordTone_Start(a_tMusic[count]);
					}
					
				}

				count++;
			}

			ucKeyBoard_State	= KEYBOARD_CHK_PUSH;
			ucKeyBoard_KeyValue	= KEYBOARD_STATE_NO;
		}
	}
}

/****************************************************************************************
* �������ƣ�KeyBoard_FunReg()
* ��������: �������ܺ���ע��
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-10-15 10:3
****************************************************************************************/
void KeyBoard_FunReg(uint8 key, void (*pfnKey)(void))
{
	const uint8 code[] = {0, 1, 3, 2, 4, 5};

	if (key >= KEYBOARD_MAX_CNT)
		return;

	a_pfnKeyBoard_Fun[code[key]] = pfnKey;
}

/****************************************************************************************
* �������ƣ�KeyBoard_Init()
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-10-14 23:25
****************************************************************************************/
void KeyBoard_Init(void)
{
	uint8			 count;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(KEY_BOARD_1PIN_PERIPH_CLK
						  | KEY_BOARD_2PIN_PERIPH_CLK
						  | KEY_BOARD_3PIN_PERIPH_CLK
						  | KEY_BOARD_4PIN_PERIPH_CLK , ENABLE);

	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin		= KEY_BOARD_1PIN;
	GPIO_Init(KEY_BOARD_1PIN_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin		= KEY_BOARD_2PIN;
	GPIO_Init(KEY_BOARD_2PIN_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin		= KEY_BOARD_3PIN;
	GPIO_Init(KEY_BOARD_3PIN_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin		= KEY_BOARD_4PIN;
	GPIO_Init(KEY_BOARD_4PIN_PORT, &GPIO_InitStructure);

	for (count = 0; count < KEYBOARD_MAX_CNT; count++)
		a_pfnKeyBoard_Fun[count] = NULL;
}

/*******************************��Ȩ����(c)**************END OF FILE********************/
