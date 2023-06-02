/************************************��Ȩ����(c)****************************************
* ��Ȩ����(c), 2012-2013, XXXXXXXXXXXXX, ��������Ȩ
* �ļ���      ��app.c
* ����        ��
* ��������    ��
* �汾        ��V1.0.0.0
* ����        ��2017-03-19
* ��������    ��
* ��ע        ��
* ��Ҫ�����б�
* �޸ļ�¼    ��
  ��      ��          �汾          �޸���          �޸�����
  2017-03-19          1.0           XXXXXXXX        �����ļ�	
****************************************************************************************/

/* ���Բ��ֿ��� -----------------------------------------------------------------------*/

/* �����ļ� ---------------------------------------------------------------------------*/
#include "app_cfg.h"
#include "NewType.h"
#include "CoreInclude.h"
#include "STM32Fxxx_Serial.h"
#include "STM32Fxxx_Misc.h"

#if (defined(CODE_DEBUG))
#include "CLI.h"
#endif  //#if (defined(CODE_DEBUG))

#include "SX1212.h"
#include "rtc.h"

#include "ChordTone.h"
#include "KeyBoard.h"
#include "Cooker_Parse.h"
#include "HT1621.h"
#include <string.h>
//#include "Package_SX1212.h"

#include "HT1621.h"

/* �궨�� -----------------------------------------------------------------------------*/

/* �ṹ���Ͷ��� -----------------------------------------------------------------------*/

/* ԭ���������� -----------------------------------------------------------------------*/
extern void BSP_Description(void);

extern void SX1212_DevInit(void);

extern void GasStove_DisplayService(void);
extern void GasStove_Init(void);

extern void Cooker_ExtPinGPIOInit(void);
extern void FanRotation_Service(void);
extern void GasStove_GasBatStateChkService(void);
extern void GasStove_FireStateChkService(void);
extern void Cooker_WirelessSendLoad(char *load, unsigned int len);
extern void LED_SEG_Service(void);
extern void bat_dis(void);
/* ȫ�ֱ������� -----------------------------------------------------------------------*/
char i = 0;
/* ��̬�������� -----------------------------------------------------------------------*/
extern unsigned int Cooker_Load(Cooker_Parse_t *entity);
extern void TM1650_GPIOInit(void);
extern void GasStove_Flameout(void);
Cooker_Parse_t entity;

/****************************************************************************************
* �������ƣ�main()
* ��������: 
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-03-19 21:15
****************************************************************************************/
int main(void)
{
#ifdef APP_UPDATE
    Application_VectorTableInit(LOAD_APPlicationAddr);
#endif  //APP_UPDATE
    
    SysTick_HandleInit();
    
#if (defined(CODE_DEBUG))
    CLI_Init(SERIAL_BAUDRATE_115200);
    BSP_Description();
#endif  //#if (defined(CODE_DEBUG))

	HT1621_Init();

	rtc_init();
	ChordTone_Init();
	KeyBoard_Init();
//	Led_Init();

	Cooker_ExtPinGPIOInit();
	GasStove_Init();
    
	SX1212_DevInit();
	InitSX1212();

#if 1       // to test SPI
	SpiWriteCfg(REG_SYNCBYTE4, 0x55);
	if(SpiReadCfg(REG_SYNCBYTE4) != 0x55) {
		APP_TRACE("\n1.something wrong with SPI\n");		// something wrong with SPI
	}

	SpiWriteCfg(REG_SYNCBYTE4, 0xAA);
	if(SpiReadCfg(REG_SYNCBYTE4) != 0xAA) {
		APP_TRACE("\n2.something wrong with SPI\n");		// something wrong with SPI
	}

	SpiWriteCfg(REG_SYNCBYTE4, SYNC_WORD4);
	if(SpiReadCfg(REG_SYNCBYTE4) != SYNC_WORD4) {
		APP_TRACE("\n3.something wrong with SPI\n");		// something wrong with SPI
	}
#endif

//	Package_SX1212_Init();
    while (1)
    {
			
#if (defined(CODE_DEBUG))
        CLI_SerialAppTask();
#endif  //#if (defined(CODE_DEBUG))

		FanRotation_Service();
		HT1621_SignService();
		for(i = 0; i<50; i++)	
		{
			unsigned char rc = ERROR;

			ReceiveRfFrame((unsigned char *)(&RF_Pkt), sizeof(RF_Pkt), &rc);
			if(rc == OK)
			{
                uint16 count;
                
				APP_TRACE("\n1.���յ�����, KEY: %u->", (unsigned int)RF_Pkt.key);

                count = 0;
				while (count < RF_Pkt.key)
				{
						APP_TRACE("%02X ", RF_Pkt.fill[count]);
						count++;
				}
				APP_TRACE("\r\n");
                
				count = 0;
				while (count < RF_Pkt.key)
				{
					Cooker_Parse(RF_Pkt.fill[count++]);  
				}
				break;
			}

		} 

		ChordTone_CtrlService();

		KeyBoard_Service();
		
		GasStove_DisplayService();

		GasStove_FireStateChkService();
		GasStove_GasBatStateChkService();	
    }
}

/****************************************************************************************
* �������ƣ�SX1212_SendKey()
* ��������: ���嵱ǰ���͵�key
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-3-26 15:14
****************************************************************************************/
void SX1212_SendKey(uint8 key)
{
//	uiSendDly = key;
}

/****************************************************************************************
* �������ƣ�BSP_Description()
* ��������: ������Ϣ���
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2017-3-19 21:17
****************************************************************************************/
void BSP_Description(void)
{
#if (defined(CODE_DEBUG))
	int 				count = 0;
	RCC_ClocksTypeDef 	clocks;
	ETAG_TypeDef 		MCU_Tag;
#endif  //#if (defined(CODE_DEBUG))

#if (defined(CODE_DEBUG))
	extern char	__heap_base[];
	extern char __heap_limit[];
	extern char	__initial_sp[];
//	extern char STACK_SIZE[];
//	extern char HEAP_SIZE[];
#endif  //#if (defined(CODE_DEBUG))

#if (defined(CODE_DEBUG))
	RCC_GetClocksFreq(&clocks);
	STM32Fxxx_GetEtag(&MCU_Tag);
#endif  //#if (defined(CODE_DEBUG))

#if (defined(CODE_DEBUG))
	APP_TRACE("���԰汾.\r\n");
	APP_TRACE("����ʱ�䣺"__DATE__" - "__TIME__"\r\n\r\n");
#else
	APP_TRACE("����ʱ�䣺"__DATE__"\r\n\r\n");
#endif	//#if (defined(CODE_DEBUG))

#if (defined(CODE_DEBUG))
#ifdef STM32F1XX
	APP_TRACE("�����̼��汾: V%u.%u.%u.%u\r\n\r\n",__STM32F10X_STDPERIPH_VERSION_MAIN, __STM32F10X_STDPERIPH_VERSION_SUB1,\
		__STM32F10X_STDPERIPH_VERSION_SUB2, __STM32F10X_STDPERIPH_VERSION_RC);
#endif  //#ifdef STM32F1XX

#ifdef STM32F0XX
	APP_TRACE("�����̼��汾: V%u.%u.%u.%u\r\n\r\n",__STM32F0XX_STDPERIPH_VERSION_MAIN, __STM32F0XX_STDPERIPH_VERSION_SUB1,\
		__STM32F0XX_STDPERIPH_VERSION_SUB2, __STM32F0XX_STDPERIPH_VERSION_RC);
#endif  //#ifdef STM32F0XX  

	APP_TRACE("�ն�MCUΨһID: ");
	while(count < 12)
	{
		APP_TRACE("%02X ", (uint16)MCU_Tag.id[count]);
		count++;
	}
	APP_TRACE("\r\n\r\n");

#ifdef STM32F1XX
	APP_TRACE("SYSCLK    Ƶ��: %uHz\r\n", clocks.SYSCLK_Frequency);     /*!< returns SYSCLK clock frequency expressed in Hz */
	APP_TRACE("HCLK      Ƶ��: %uHz\r\n", clocks.HCLK_Frequency);       /*!< returns HCLK clock frequency expressed in Hz */
	APP_TRACE("PCLK1     Ƶ��: %uHz\r\n", clocks.PCLK1_Frequency);      /*!< returns PCLK1 clock frequency expressed in Hz */
	APP_TRACE("PCLK2     Ƶ��: %uHz\r\n", clocks.PCLK2_Frequency);      /*!< returns PCLK2 clock frequency expressed in Hz */
	APP_TRACE("ADCCLK    Ƶ��: %uHz\r\n\r\n", clocks.ADCCLK_Frequency); /*!< returns ADCCLK clock frequency expressed in Hz */
#endif  //STM32F1XX

#ifdef STM32F0XX
	APP_TRACE("SYSCLK    Ƶ��: %uHz\r\n", clocks.SYSCLK_Frequency);
	APP_TRACE("HCLK      Ƶ��: %uHz\r\n", clocks.HCLK_Frequency);
	APP_TRACE("PCLK      Ƶ��: %uHz\r\n", clocks.PCLK_Frequency);
	APP_TRACE("ADCCLK    Ƶ��: %uHz\r\n", clocks.ADCCLK_Frequency);
	APP_TRACE("CECCLK    Ƶ��: %uHz\r\n", clocks.CECCLK_Frequency);
	APP_TRACE("I2C1CLK   Ƶ��: %uHz\r\n", clocks.I2C1CLK_Frequency);
	APP_TRACE("USART1CLK Ƶ��: %uHz\r\n", clocks.USART1CLK_Frequency);
	APP_TRACE("USART2CLK Ƶ��: %uHz\r\n", clocks.USART2CLK_Frequency);
	APP_TRACE("USBCLK    Ƶ��: %uHz\r\n\r\n", clocks.USBCLK_Frequency);
#endif  //STM32F0XX

	APP_TRACE("__heap_base  : 0x%X\n", (uint32)&__heap_base);
	APP_TRACE("__heap_limit : 0x%X\n", (uint32)&__heap_limit);
	APP_TRACE("__initial_sp : 0x%X\n", (uint32)&__initial_sp);
//	APP_TRACE("STACK_SIZE   : 0x%X\n", (uint32)&STACK_SIZE);
//	APP_TRACE("HEAP_SIZE    : 0x%X\n", (uint32)&HEAP_SIZE);
#endif  //#if (defined(CODE_DEBUG))
}

/*******************************��Ȩ����(c)**************END OF FILE********************/

