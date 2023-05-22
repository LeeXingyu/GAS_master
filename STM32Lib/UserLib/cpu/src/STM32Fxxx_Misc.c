/************************************��Ȩ����(c)****************************************
* ��Ȩ����(c), 2012-2013, XXXXXXXXXXXX, ��������Ȩ
* �ļ���      ��STM32Fxxx_Misc.c
* ����        ��XXXX
* �汾        ��V 1.0
* ����        ��2015-03-17
* ��������    ��
* ��ע        ��
* ��Ҫ�����б�
* �޸ļ�¼    ��
  ��      ��          �汾          �޸���          �޸�����
  2015-03-17          1.0           XXXX            �����ļ�
****************************************************************************************/

/* ���Բ��ֿ��� -----------------------------------------------------------------------*/

/* �����ļ� ---------------------------------------------------------------------------*/
#include "STM32Fxxx_Misc.h"
#include "CoreInclude.h"

/* �궨�� -----------------------------------------------------------------------------*/

/* �ṹ���Ͷ��� -----------------------------------------------------------------------*/

/* ԭ���������� -----------------------------------------------------------------------*/

/* ȫ�ֱ������� -----------------------------------------------------------------------*/

/* ��̬�������� -----------------------------------------------------------------------*/
#if (!defined(APP_OS))
static volatile uint32	SysTick_Timer;
#endif  //(!defined(APP_OS))

#if defined(STM32F0XX)
static const char STM32Fxxx_DelayCount[] = {0, 1, 2, 3, 4, 4};
#endif  //defined(STM32F0XX)

#if defined(STM32F1XX)
static const char STM32Fxxx_DelayCount[] = {0, 1, 4, 5, 6, 6};
#endif  //defined(STM32F1XX)

#if defined(STM32F4XX)
static const char STM32Fxxx_DelayCount[] = {0, 30, 35, 40, 39, 39};
#endif  //defined(STM32F4XX)


#ifdef STM32F0xx
#if (defined ( __CC_ARM ))
    __IO uint32_t VectorTable[48] __attribute__((at(0x20000000)));
#elif (defined (__ICCARM__))
#pragma location = 0x20000000
    __no_init __IO uint32_t VectorTable[48];
#elif defined   (  __GNUC__  )
    __IO uint32_t VectorTable[48] __attribute__((section(".RAMVectorTable")));
#elif defined ( __TASKING__ )
    __IO uint32_t VectorTable[48] __at(0x20000000);
#endif
#endif              //STM32F0xx

const static uint8_t a_McuRstFlag[] =
{
//    RCC_FLAG_HSIRDY,         //HSI�������
//    RCC_FLAG_HSERDY,         //HSE�������
//    RCC_FLAG_PLLRDY,         //PLL����
//    RCC_FLAG_LSERDY,         //LSI�������
//    RCC_FLAG_LSIRDY,         //LSE�������
    
//    RCC_FLAG_PINRST,         //�ܽŸ�λ(NRST�ܽ��ϵĵ͵�ƽ)
    RCC_FLAG_PORRST,         //POR/PDR��λ(�ϵ�/���縴λ����Դ��λ)
    RCC_FLAG_SFTRST,         //�����λ
    RCC_FLAG_IWDGRST,        //IWDG���Ź���λ(�������Ź�������ֹ)
    RCC_FLAG_WWDGRST,        //WWDG���Ź���λ(���ڿ��Ź�������ֹ)
    RCC_FLAG_LPWRRST,        //�͹��Ĺ���λ
};

const static MCU_RstType_e a_eMCUResetType[] =
{
//    eMCU_PIN_RST,
    eMCU_POR_RST,
    eMCU_SFT_RST,
    eMCU_IWDG_RST,
    eMCU_WWDG_RST,
    eMCU_LPWR_RST,
};

/****************************************************************************************
* ��������: STM32Fxxx_GetEtag()
* ��������: ��ȡΨһID�����洢�ռ�
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
****************************************************************************************/
void STM32Fxxx_GetEtag(ETAG_TypeDef *Info)
{
	uint32	id_1, id_2, id_3;

#ifdef STM32F1XX
	Info->flash_size = (*( uint16 *)0x1FFFF7E0);   //���������Ĵ���
	Info->page_size  = (Info->flash_size<256)?1024:2048;

	id_1=(*( uint32 *)0x1FFFF7E8);   	//��ƷΨһ��ݱ�ʶ�Ĵ���(96λ)
	id_2=(*( uint32 *)0x1FFFF7EC);
	id_3=(*( uint32 *)0x1FFFF7F0);
#endif //:~STM32F1XX

#ifdef STM32F0XX
	Info->flash_size = (*( uint16 *)0x1FFFF7CC);   //���������Ĵ���
	Info->page_size  = 1024;

	id_1=(*( uint32 *)0x1FFFF7AC);   	//��ƷΨһ��ݱ�ʶ�Ĵ���(96λ)
	id_2=(*( uint32 *)0x1FFFF7B0);
	id_3=(*( uint32 *)0x1FFFF7B4);
#endif //:~STM32F0XX

#ifdef STM32F4XX
	Info->flash_size = (*( uint16 *)0x1FFF7A22);   //���������Ĵ���
    Info->page_size  = 0;                           //M4��Flashû��ҳ��һ˵

	id_1=(*( uint32 *)0x1FFF7A10);   	//��ƷΨһ��ݱ�ʶ�Ĵ���(96λ)
	id_2=(*( uint32 *)0x1FFF7A14);
	id_3=(*( uint32 *)0x1FFF7A18);
#endif //:~STM32F4XX


	Info->id[0] = (uint8)id_1;
	Info->id[1] = (uint8)(id_1>>8);
	Info->id[2] = (uint8)(id_1>>16);
	Info->id[3] = (uint8)(id_1>>24);

	Info->id[4] = (uint8)id_2;
	Info->id[5] = (uint8)(id_2>>8);
	Info->id[6] = (uint8)(id_2>>16);
	Info->id[7] = (uint8)(id_2>>24);

	Info->id[8] = (uint8)id_3;
	Info->id[9] = (uint8)(id_3>>8);
	Info->id[10] = (uint8)(id_3>>16);
	Info->id[11] = (uint8)(id_3>>24);
}

/****************************************************************************************
* ��������: STM32Fxxx_CountDelayus()
* ��������: ��ʱus
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
****************************************************************************************/
void STM32Fxxx_CountDelayus(uint16 us)
{
#if defined(STM32F0XX)
	int i, count;
	uint32 delay;

	switch(us)
	{
	case 1:
		delay = STM32Fxxx_DelayCount[us];
		break;

	case 2:
		delay = STM32Fxxx_DelayCount[us];
		break;

	case 3:
		delay = STM32Fxxx_DelayCount[us];
		break;

	case 4:
		delay = STM32Fxxx_DelayCount[us];
		break;

	case 5:
		delay = STM32Fxxx_DelayCount[us];
		break;

	default:
		delay = 5;
		break;
	}

	for(count=0; count<us; count++)
	{
		i = 0;
		while(i < delay)
			i++;
	}
#endif  //defined(STM32F0XX)

#if defined(STM32F1XX)
	int i, count;
	uint32 delay;

	switch(us)
	{
	case 1:
		delay = STM32Fxxx_DelayCount[us];
		break;

	case 2:
		delay = STM32Fxxx_DelayCount[us];
		break;

	case 3:
		delay = STM32Fxxx_DelayCount[us];
		break;

	case 4:
		delay = STM32Fxxx_DelayCount[us];
		break;

	case 5:
		delay = STM32Fxxx_DelayCount[us];
		break;
    
	default:
		delay = 7;
		break;
	}

	for(count=0; count<us; count++)
	{
		i = 0;
		while(i < delay)
			i++;
	}
#endif  //defined(STM32F1XX)
    
#if defined(STM32F4XX)
    int i, count;
	uint32 delay;

	switch(us)
	{
	case 1:
		delay = STM32Fxxx_DelayCount[us];
		break;

	case 2:
		delay = STM32Fxxx_DelayCount[us];
		break;

	case 3:
		delay = STM32Fxxx_DelayCount[us];
		break;

	default:
		delay = 39;
		break;
	}

	for(count=0; count<us; count++)
	{
		i = 0;
		while(i < delay)
			i++;
	}
#endif  //defined(STM32F4XX)
}

#if (!defined(APP_OS))
static void (*pfnSysTick_Fun)(void) = NULL;
/****************************************************************************************
* ��������: SysTick_HandleInit()
* ��������: 
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
****************************************************************************************/
void SysTick_HandleInit(void)
{
    SysTick_Config(SystemCoreClock / FACTOR_1_S);			//ticks = HCLK*ms
}

/****************************************************************************************
* ��������: SysTick_Handler()
* ��������: 
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
****************************************************************************************/
void SysTick_FunReg(void (*pfun)(void))
{
	pfnSysTick_Fun = pfun;
}

/****************************************************************************************
* ��������: SysTick_Handler()
* ��������: 
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
****************************************************************************************/
void SysTick_Handler(void)
{
	SysTick_Timer++;
	
	if (pfnSysTick_Fun)
		pfnSysTick_Fun();
}

/****************************************************************************************
* ��������: BSP_OS_TimeGet()
* ��������: 
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
****************************************************************************************/
uint32 BSP_OS_TimeGet(void)
{
	uint32 Timer;

	__disable_irq();
	Timer = SysTick_Timer;
	__enable_irq();

	return Timer;
}

/****************************************************************************************
* ��������: BSP_OS_Timeout()
* ��������: 
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
****************************************************************************************/
BOOL BSP_OS_Timeout(uint32 Start, uint32 Delay)
{
	uint32 Timer;

	__disable_irq();
	Timer = SysTick_Timer;
	__enable_irq();

	if((uint32)(Timer - Start) >= Delay)
    	return(TRUE);
  	else
    	return(FALSE);
}

/****************************************************************************************
* ��������: BSP_OS_TimeDlyMs()
* ��������: 
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
****************************************************************************************/
void BSP_OS_TimeDlyMs(uint32 dly_ms)
{
	uint32 timer;

	timer = SysTick_Timer;
	while((uint32)(SysTick_Timer-timer) < dly_ms)
		;
}
#endif //APP_OS


/****************************************************************************************
* ��������: STM32Fxxx_JumpToAppCode()
* ��������: ����Ӧ�ó���
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
****************************************************************************************/
void STM32Fxxx_JumpToAppCode(uint32 app_addr)
{
    uint32  JumpAddress;
    void    (*Jump_To_Application)(void);

    if (((*(__IO uint32*)app_addr) & 0x2FFE0000 ) == 0x20000000)
    { 
        JumpAddress = *(__IO uint32_t*)(app_addr + 4);             //Jump to user application
        Jump_To_Application = (void (*)(void))JumpAddress;
        __set_MSP(*(__IO uint32_t*)app_addr);                      //Initialize user application's Stack Pointer
        Jump_To_Application();
    }
}

/****************************************************************************************
* ��������: STM32Fxxx_SysReset()
* ��������: ��λstm32
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
****************************************************************************************/
void STM32Fxxx_SysReset(void)
{
    __disable_irq();
    
    NVIC_SystemReset(); 
    while(1);
}

#ifdef STM32F0xx
/****************************************************************************************
* ��������: Application_VectorTableInit
* ��������: M0û��������ƫ�ƼĴ�������APP�Ŀ�ͷmain()�����һ�δ��룬���ж��������Ƶ�SRAM����ʼ��ַ0x20000000
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
****************************************************************************************/
void STM32Fxxx_AppVectorTabInit(uint32 app_addr)
{
    uint32 i;
    
    /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file*/
    
    /* Relocate by software the vector table to the internal SRAM at 0x20000000 ***/  

    /* Copy the vector table from the Flash (mapped at the base of the application
        load address 0x08006000) to the base address of the SRAM at 0x20000000. */
    if ((app_addr >> 10) & 0x01)
    {
        while(1);
    }
        
    for(i = 0; i < 48; i++)
    {
        VectorTable[i] = *(__IO uint32_t*)(app_addr + (i << 2));
    }

    /* Enable the SYSCFG peripheral clock*/
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SYSCFG, ENABLE); 
    /* Remap SRAM at 0x00000000 */
    SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);

    __disable_irq();

    __enable_irq();
}
#endif          //STM32F0xx

#ifdef STM32F1XX
/****************************************************************************************
* ��������: STM32Fxxx_AppVectorTabInit()
* ��������: ���¶���������
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
****************************************************************************************/
void STM32Fxxx_AppVectorTabInit(uint32 offest)
{
    if ((offest >> 10) & 0x01)
    {
        while(1);
    }
    
#ifdef  VECT_TAB_RAM
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, offest);
#endif          //VECT_TAB_RAM
}
#endif  //STM32F1XX

/****************************************************************************************
* ��������: STM32Fxxx_RstType()
* ��������: ��λ����
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
****************************************************************************************/
MCU_RstType_e STM32Fxxx_RstType(void)
{
    MCU_RstType_e   flag = eMCU_NO_RST;
    uint32          count = 0;
    uint32          type_cnt = sizeof(a_eMCUResetType) / sizeof(MCU_RstType_e);
    
    while ((count < type_cnt) && (eMCU_NO_RST == flag))
    {
        if (SET == RCC_GetFlagStatus(a_McuRstFlag[count]))
            flag = a_eMCUResetType[count];
        
        count++;
    }
    
    RCC_ClearFlag();                        //���RCC�ĸ�λ��־
    
    return flag;
}

/*******************************��Ȩ����(c)**************END OF FILE********************/
