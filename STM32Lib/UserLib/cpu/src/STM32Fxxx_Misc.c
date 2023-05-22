/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXXXXXX, 保留所有权
* 文件名      ：STM32Fxxx_Misc.c
* 作者        ：XXXX
* 版本        ：V 1.0
* 日期        ：2015-03-17
* 功能描述    ：
* 备注        ：
* 主要函数列表：
* 修改记录    ：
  日      期          版本          修改人          修改内容
  2015-03-17          1.0           XXXX            创建文件
****************************************************************************************/

/* 调试部分开关 -----------------------------------------------------------------------*/

/* 包含文件 ---------------------------------------------------------------------------*/
#include "STM32Fxxx_Misc.h"
#include "CoreInclude.h"

/* 宏定义 -----------------------------------------------------------------------------*/

/* 结构类型定义 -----------------------------------------------------------------------*/

/* 原型声明部分 -----------------------------------------------------------------------*/

/* 全局变量声明 -----------------------------------------------------------------------*/

/* 静态变量定义 -----------------------------------------------------------------------*/
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
//    RCC_FLAG_HSIRDY,         //HSI晶振就绪
//    RCC_FLAG_HSERDY,         //HSE晶振就绪
//    RCC_FLAG_PLLRDY,         //PLL就绪
//    RCC_FLAG_LSERDY,         //LSI晶振就绪
//    RCC_FLAG_LSIRDY,         //LSE晶振就绪
    
//    RCC_FLAG_PINRST,         //管脚复位(NRST管脚上的低电平)
    RCC_FLAG_PORRST,         //POR/PDR复位(上电/掉电复位即电源复位)
    RCC_FLAG_SFTRST,         //软件复位
    RCC_FLAG_IWDGRST,        //IWDG看门狗复位(独立看门狗计数终止)
    RCC_FLAG_WWDGRST,        //WWDG看门狗复位(窗口看门狗计数终止)
    RCC_FLAG_LPWRRST,        //低功耗管理复位
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
* 函数名称: STM32Fxxx_GetEtag()
* 功能描述: 获取唯一ID，及存储空间
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void STM32Fxxx_GetEtag(ETAG_TypeDef *Info)
{
	uint32	id_1, id_2, id_3;

#ifdef STM32F1XX
	Info->flash_size = (*( uint16 *)0x1FFFF7E0);   //闪存容量寄存器
	Info->page_size  = (Info->flash_size<256)?1024:2048;

	id_1=(*( uint32 *)0x1FFFF7E8);   	//产品唯一身份标识寄存器(96位)
	id_2=(*( uint32 *)0x1FFFF7EC);
	id_3=(*( uint32 *)0x1FFFF7F0);
#endif //:~STM32F1XX

#ifdef STM32F0XX
	Info->flash_size = (*( uint16 *)0x1FFFF7CC);   //闪存容量寄存器
	Info->page_size  = 1024;

	id_1=(*( uint32 *)0x1FFFF7AC);   	//产品唯一身份标识寄存器(96位)
	id_2=(*( uint32 *)0x1FFFF7B0);
	id_3=(*( uint32 *)0x1FFFF7B4);
#endif //:~STM32F0XX

#ifdef STM32F4XX
	Info->flash_size = (*( uint16 *)0x1FFF7A22);   //闪存容量寄存器
    Info->page_size  = 0;                           //M4的Flash没有页这一说

	id_1=(*( uint32 *)0x1FFF7A10);   	//产品唯一身份标识寄存器(96位)
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
* 函数名称: STM32Fxxx_CountDelayus()
* 功能描述: 延时us
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
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
* 函数名称: SysTick_HandleInit()
* 功能描述: 
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void SysTick_HandleInit(void)
{
    SysTick_Config(SystemCoreClock / FACTOR_1_S);			//ticks = HCLK*ms
}

/****************************************************************************************
* 函数名称: SysTick_Handler()
* 功能描述: 
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void SysTick_FunReg(void (*pfun)(void))
{
	pfnSysTick_Fun = pfun;
}

/****************************************************************************************
* 函数名称: SysTick_Handler()
* 功能描述: 
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void SysTick_Handler(void)
{
	SysTick_Timer++;
	
	if (pfnSysTick_Fun)
		pfnSysTick_Fun();
}

/****************************************************************************************
* 函数名称: BSP_OS_TimeGet()
* 功能描述: 
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
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
* 函数名称: BSP_OS_Timeout()
* 功能描述: 
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
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
* 函数名称: BSP_OS_TimeDlyMs()
* 功能描述: 
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
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
* 函数名称: STM32Fxxx_JumpToAppCode()
* 功能描述: 跳向应用程序
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
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
* 函数名称: STM32Fxxx_SysReset()
* 功能描述: 复位stm32
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void STM32Fxxx_SysReset(void)
{
    __disable_irq();
    
    NVIC_SystemReset(); 
    while(1);
}

#ifdef STM32F0xx
/****************************************************************************************
* 函数名称: Application_VectorTableInit
* 功能描述: M0没有向量表偏移寄存器，在APP的开头main()里面加一段代码，把中断向量表复制到SRAM的起始地址0x20000000
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
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
* 函数名称: STM32Fxxx_AppVectorTabInit()
* 功能描述: 重新定义向量表
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
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
* 函数名称: STM32Fxxx_RstType()
* 功能描述: 复位类型
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
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
    
    RCC_ClearFlag();                        //清除RCC的复位标志
    
    return flag;
}

/*******************************版权所有(c)**************END OF FILE********************/
