/************************************��Ȩ����(c)****************************************
* ��Ȩ����(c), 2012-2013, XXXXXXXX��˾, ��������Ȩ
* �ļ���      ��platform_ExtPin.c
* ����        ��XXXXXX
* ��������    ��XXXXXX
* �汾        ��V1.0.0.0
* ����        ��2017-11-19
* ��������    ��
* ��ע        ��
* ��Ҫ�����б�
* �޸ļ�¼    ��
  ��      ��          �汾          �޸���          �޸�����
  2017-11-19          1.0           XXXXXXXX        �����ļ�	
****************************************************************************************/

/* ���Բ��ֿ��� -----------------------------------------------------------------------*/

/* �����ļ� ---------------------------------------------------------------------------*/
#include "CoreInclude.h"
#include "NewType.h"
#include "app_cfg.h"

#include "STM32Fxxx_Misc.h"

/* �궨�� -----------------------------------------------------------------------------*/
#define FAN_ROTATION_DELAY_TIME				300							//����ת��������ʱʱ��

/* �ṹ���Ͷ��� -----------------------------------------------------------------------*/

/* ԭ���������� -----------------------------------------------------------------------*/

/* ȫ�ֱ������� -----------------------------------------------------------------------*/

/* ��̬�������� -----------------------------------------------------------------------*/
static BOOL     bFanRotation_RunState  = FALSE;
static uint32_t uiFanRotation_RunDelay = 0;

/****************************************************************************************
* �������ƣ�Cooker_ExtPinGPIOInit()
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-11-19 23:36
****************************************************************************************/
void Cooker_ExtPinGPIOInit(void)
{
	GPIO_InitTypeDef 		GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(FAN_ROTATION_PERIPH_CLK | GAS_SENSOR_PERIPH_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin     = FAN_ROTATION_PIN;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd    = GPIO_PuPd_UP;
	GPIO_Init(FAN_ROTATION_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin     = GAS_SENSOR_PIN;
	GPIO_Init(GAS_SENSOR_PORT, &GPIO_InitStructure);
}

/****************************************************************************************
* �������ƣ�FanRotation_Level()
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-11-19 23:36
****************************************************************************************/
static BOOL FanRotation_Level(void)
{
	return ((GPIO_ReadInputDataBit(FAN_ROTATION_PORT, FAN_ROTATION_PIN)) ? FALSE : TRUE);
}

/****************************************************************************************
* �������ƣ�GasStove_FanRotation()
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-11-19 23:36
****************************************************************************************/
BOOL GasStove_FanRotation(void)      //����Ƿ�ת��
{
	return bFanRotation_RunState;
}

/****************************************************************************************
* �������ƣ�
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2018-10-7 23:57
****************************************************************************************/
void FanRotation_Service(void)
{
	BOOL bState = FanRotation_Level();
    static BOOL bLastState = FALSE;

	if (bLastState != bState)
    {
        bLastState = bState;
		uiFanRotation_RunDelay = BSP_OS_TimeGet();
    }

	if (BSP_OS_Timeout(uiFanRotation_RunDelay, FAN_ROTATION_DELAY_TIME))
		bFanRotation_RunState = bLastState;
}

/****************************************************************************************
* �������ƣ�GasStove_GasSensor()
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-11-19 23:36
****************************************************************************************/
BOOL GasStove_GasSensor(void)        //�������
{
	return ((GPIO_ReadInputDataBit(GAS_SENSOR_PORT, GAS_SENSOR_PIN)) ? TRUE : FALSE);
}

/*******************************��Ȩ����(c)**************END OF FILE********************/
