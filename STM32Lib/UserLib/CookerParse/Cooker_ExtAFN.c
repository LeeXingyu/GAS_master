/************************************��Ȩ����(c)****************************************
* ��Ȩ����(c), 2012-2013, XXXXXXXX��˾, ��������Ȩ
* �ļ���      ��Cooker_ExtAFN.c
* ����        ��XXXXXX
* ��������    ��XXXXXX
* �汾        ��V1.0.0.0
* ����        ��2017-11-15
* ��������    ��
* ��ע        ��
* ��Ҫ�����б�
* �޸ļ�¼    ��
  ��      ��          �汾          �޸���          �޸�����
  2017-11-15          1.0           XXXXXXXX        �����ļ�	
****************************************************************************************/

/* ���Բ��ֿ��� -----------------------------------------------------------------------*/

/* �����ļ� ---------------------------------------------------------------------------*/
#include "Cooker_Parse.h"

//#include "Led.h"
#include "HT1621.h"
#include "STM32Fxxx_Misc.h"
#include "string.h"
/* �궨�� -----------------------------------------------------------------------------*/
#define GSA_STOVE_OFF_FIRE_TIME             31000

/* �ṹ���Ͷ��� -----------------------------------------------------------------------*/

/* ԭ���������� -----------------------------------------------------------------------*/
extern void Cooker_WirelessSendLoad(char *load, unsigned int len);
extern void SendRfFrame (unsigned char *buffer, unsigned char len);
extern void Updata_Awaken_Config(void);
extern void Updata_Normal_Config(void);
/* ȫ�ֱ������� -----------------------------------------------------------------------*/
static BOOL	bGasStove_FireState = FALSE;
static BOOL bCooker_SysIdMatchState = FALSE;
static BOOL bGasStove_BatState = FALSE;
static BOOL bGasStove_GasCtrlState = FALSE;
static uint32 uiFireState_ChkDly = 0;
static uint32 uiBatState_ChkDly = 0;
static uint32 uiGasCtrlState_ChkDly = 0;
/* ��̬�������� -----------------------------------------------------------------------*/
static char Awaken_data[30] = {0xAA};
/****************************************************************************************
* �������ƣ�GasStove_FireStateChkService()
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-11-19 22:41
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
* �������ƣ�GasStove_FireState()
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-11-19 22:41
****************************************************************************************/
BOOL GasStove_FireState(void)
{
	return bGasStove_FireState;
}

/****************************************************************************************
* �������ƣ�GasStove_Flameout()
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-11-19 22:41
****************************************************************************************/
void GasStove_Flameout(void)
{
	char i = 0;
	Cooker_Parse_t entity;
	
	Updata_Awaken_Config();
	memset(Awaken_data,0xAA,30);
	//19200bitrate 52ms�ӻ�����һ��
	//ѭ������ ����
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
* �������ƣ�Cooker_SysIdMatch()
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-11-19 22:41
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
* �������ƣ�
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-11-19 23:22
****************************************************************************************/
BOOL Cooker_SysIdMatchResult(void)
{
	return bCooker_SysIdMatchState;
}

/****************************************************************************************
* �������ƣ�Cooker_AFNChk()
* ��������: �Դӻ��źŵ�cmd���з���
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-11-15 20:22
****************************************************************************************/
void Cooker_AFNChk(Cooker_Parse_t *entity)
{
	switch ((Cooker_Cmd_e)entity->cmd)
	{
		case eCOOKER_SET_SYS_ID:
		{
			//����ɹ�
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
		case eCOOKER_CTRL_Gas:    //��������
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
		case eCOOKER_STATE_Gas:        //��ѹ״̬
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


/*******************************��Ȩ����(c)**************END OF FILE********************/
