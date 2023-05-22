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

/* �궨�� -----------------------------------------------------------------------------*/
#define GSA_STOVE_OFF_FIRE_TIME             31000

/* �ṹ���Ͷ��� -----------------------------------------------------------------------*/

/* ԭ���������� -----------------------------------------------------------------------*/

/* ȫ�ֱ������� -----------------------------------------------------------------------*/
static BOOL	bGasStove_FireState = FALSE;
static BOOL bCooker_SysIdMatchState = FALSE;

static uint32 uiFireState_ChkDly = 0;

/* ��̬�������� -----------------------------------------------------------------------*/

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
	Cooker_Parse_t entity;

	entity.cmd			= eCOOKER_CTRL_FIRE;
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
* ��������: 
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
		case eCOOKER_CTRL_FIRE:
		{

		}
		break;
		//case eCOOKER_RSP:
		//{

		//}
		//break;
		default: break;
	}
}


/*******************************��Ȩ����(c)**************END OF FILE********************/
