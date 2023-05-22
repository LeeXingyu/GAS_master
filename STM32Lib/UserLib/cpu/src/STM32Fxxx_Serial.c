/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXXXXXX, 保留所有权
* 文件名      ：STM32Fxxx_Serial.c
* 作者        ：XXXX
* 版本        ：V 1.0
* 日期        ：2015-06-17
* 功能描述    ：
* 备注        ：
* 主要函数列表：
* 修改记录    ：
  日      期          版本          修改人          修改内容
  2015-06-17          1.0           XXXX            创建文件
****************************************************************************************/

/* 调试部分开关 -----------------------------------------------------------------------*/

/* 包含文件 ---------------------------------------------------------------------------*/
#include "STM32Fxxx_Serial.h"
#include "CoreInclude.h"

#if (defined(APP_OS))
#include "bsp.h"
#else
#include "app_cfg.h"
#include "STM32Fxxx_Misc.h"
#endif  //(defined(APP_OS))

/* 宏定义 -----------------------------------------------------------------------------*/
#if (!defined(APP_OS))
#define NVIC_UART1_PREPRIORITY		0
#define NVIC_UART1_SUBPRIORITY		1

#define NVIC_UART2_PREPRIORITY		0
#define NVIC_UART2_SUBPRIORITY		2

#define NVIC_UART3_PREPRIORITY		0
#define NVIC_UART3_SUBPRIORITY		2

#define NVIC_UART4_PREPRIORITY		0
#define NVIC_UART4_SUBPRIORITY		0

#define NVIC_UART5_PREPRIORITY		0
#define NVIC_UART5_SUBPRIORITY		3

#define NVIC_UART6_PREPRIORITY		0
#define NVIC_UART6_SUBPRIORITY		3
#endif  //#if (!defined(APP_OS))

/* 结构类型定义 -----------------------------------------------------------------------*/
typedef void (*STM32_Ser_Fun)(void);

#if (!defined(APP_OS))
typedef struct _STM32Fxxx_Serial_ISR_t
{
    IRQn_Type irq_type;
    uint32_t  pre_prio;
    uint32_t  sub_prio;
}STM32Fxxx_Serial_ISR_t;
#endif  //APP_OS

/* 原型声明部分 -----------------------------------------------------------------------*/
static void STM32Fxxx_Serail_EventISR_Handler(SERIAL_Port port);
#if (defined(APP_OS))
static void STM32Fxxx_Serail1_EventISR_Handler(void);
static void STM32Fxxx_Serail2_EventISR_Handler(void);
static void STM32Fxxx_Serail3_EventISR_Handler(void);
static void STM32Fxxx_Serail4_EventISR_Handler(void);
#if defined(STM32F1XX)
static void STM32Fxxx_Serail5_EventISR_Handler(void);
#elif (defined(STM32F4XX))
static void STM32Fxxx_Serail5_EventISR_Handler(void);
static void STM32Fxxx_Serail6_EventISR_Handler(void);
#endif //:~STM32F4XX
#endif  //#if (defined(APP_OS))

static void STM32Fxxx_Ser1GPIOInit(void);
static void STM32Fxxx_Ser2GPIOInit(void);
static void STM32Fxxx_Ser3GPIOInit(void);
static void STM32Fxxx_Ser4GPIOInit(void);
#if defined(STM32F1XX)
static void STM32Fxxx_Ser5GPIOInit(void);
#elif (defined(STM32F4XX))
static void STM32Fxxx_Ser5GPIOInit(void);
static void STM32Fxxx_Ser6GPIOInit(void);
#endif //:~STM32F4XX

/* 全局变量声明 -----------------------------------------------------------------------*/

/* 静态变量定义 -----------------------------------------------------------------------*/
const uint32 Serial_BaudrateDict[] 		= {1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200};
const uint16 Serial_ParityDict[] 		= {USART_Parity_No, USART_Parity_Odd, USART_Parity_Even};
const uint16 Serial_ByteBitsDict[] 		= {USART_WordLength_8b, USART_WordLength_9b};

#ifdef STM32F0XX
const uint16 Serial_StopBitsDict[]		= {USART_StopBits_1, USART_StopBits_2, USART_StopBits_1_5};
#else
const uint16 Serial_StopBitsDict[]		= {USART_StopBits_1, USART_StopBits_0_5, USART_StopBits_2, USART_StopBits_1_5};
#endif  //#ifdef STM32F0XX

#if (defined(APP_OS))
#if (defined(OS_USE_uCOSIII))
//static BSP_OS_SEM	Serial_RXSem[SERIAL_PORT_MAX];
static BSP_OS_SEM	Serial_TXLockSem[SERIAL_PORT_MAX];
#else
//static BSP_OS_SEM	Serial_RXSem[SERIAL_PORT_MAX] =
//{
//	NULL,
//	NULL,
//	NULL,
//	NULL,
//#if defined(STM32F1XX)
//	NULL
//#elif defined(STM32F4XX)
//	NULL,
//	NULL
//#endif //:~STM32F1XX
//};

static BSP_OS_SEM	Serial_TXLockSem[SERIAL_PORT_MAX] =
{
	NULL,
	NULL,
	NULL,
	NULL,
#if defined(STM32F1XX)
	NULL
#elif defined(STM32F4XX)
	NULL,
	NULL
#endif //:~STM32F1XX
};
#endif  //#if (defined(OS_USE_uCOSIII))

static BOOL abSerial_Used[SERIAL_PORT_MAX] =
{
	FALSE,
	FALSE,
	FALSE,
	FALSE,
#if defined(STM32F1XX)
	FALSE
#elif defined(STM32F4XX)
	FALSE,
	FALSE
#endif //:~STM32F1XX
};

const static CPU_INT08U Serial_INT_ID[SERIAL_PORT_MAX] =
{
	BSP_INT_ID_USART1,
	BSP_INT_ID_USART2,
	BSP_INT_ID_USART3,
#if defined(STM32F1XX)
	BSP_INT_ID_UART4,
	BSP_INT_ID_UART5
#elif defined(STM32F4XX)
	BSP_INT_ID_USART4,
	BSP_INT_ID_USART5,
	BSP_INT_ID_USART6
#endif //:~STM32F1XX
};

const static STM32_Ser_Fun	Serial_EventISR_Handler[SERIAL_PORT_MAX] =
{
	STM32Fxxx_Serail1_EventISR_Handler,
	STM32Fxxx_Serail2_EventISR_Handler,
	STM32Fxxx_Serail3_EventISR_Handler,
	STM32Fxxx_Serail4_EventISR_Handler,
#if defined(STM32F1XX)
	STM32Fxxx_Serail5_EventISR_Handler
#elif defined(STM32F4XX)
	STM32Fxxx_Serail5_EventISR_Handler,
	STM32Fxxx_Serail6_EventISR_Handler
#endif //:~STM32F1XX
};
#else
const static STM32Fxxx_Serial_ISR_t a_SerialISRTypePrio[SERIAL_PORT_MAX] =
{
    {USART1_IRQn, NVIC_UART1_PREPRIORITY, NVIC_UART1_SUBPRIORITY},
    {USART2_IRQn, NVIC_UART2_PREPRIORITY, NVIC_UART2_SUBPRIORITY},
#ifdef STM32F0XX
//    {USART3_4_IRQn, NVIC_UART3_PREPRIORITY, NVIC_UART3_SUBPRIORITY},
#else
    {USART3_IRQn, NVIC_UART3_PREPRIORITY, NVIC_UART3_SUBPRIORITY},
#endif  //#ifdef STM32F0XX
#if defined(STM32F1XX)
    {UART4_IRQn, NVIC_UART4_PREPRIORITY, NVIC_UART4_SUBPRIORITY},
    {UART5_IRQn, NVIC_UART5_PREPRIORITY, NVIC_UART5_SUBPRIORITY},
#elif defined(STM32F4XX)
    {USART4_IRQn, NVIC_UART4_PREPRIORITY, NVIC_UART4_SUBPRIORITY},
    {USART5_IRQn, NVIC_UART5_PREPRIORITY, NVIC_UART5_SUBPRIORITY},
    {USART6_IRQn, NVIC_UART6_PREPRIORITY, NVIC_UART6_SUBPRIORITY},
#elif defined(STM32F0XX)
//    {USART3_4_IRQn, NVIC_UART3_PREPRIORITY, NVIC_UART3_SUBPRIORITY},
#endif  //#if defined(STM32F4XX)
};
#endif  //APP_OS

static STM32Fxxx_SerRxPtr Serial_RxHandler[SERIAL_PORT_MAX] =
{
	NULL,
	NULL,
	NULL,
	NULL,
#if defined(STM32F1XX)
	NULL
#elif defined(STM32F4XX)
	NULL,
	NULL
#endif //:~STM32F1XX
};

const static STM32_Ser_Fun	Serial_GPIOInit[SERIAL_PORT_MAX] =
{
	STM32Fxxx_Ser1GPIOInit,
	STM32Fxxx_Ser2GPIOInit,
	STM32Fxxx_Ser3GPIOInit,
	STM32Fxxx_Ser4GPIOInit,
#if defined(STM32F1XX)
	STM32Fxxx_Ser5GPIOInit
#elif defined(STM32F4XX)
	STM32Fxxx_Ser5GPIOInit,
	STM32Fxxx_Ser6GPIOInit
#endif //:~STM32F1XX
};

const static USART_TypeDef *Serial_PERIPH_ID[SERIAL_PORT_MAX] =
{
    USART1,
    USART2,
    USART3,
#ifdef STM32F0XX
    USART4,
#elif defined(STM32F1XX)
    UART4,
#endif  //#ifdef STM32F0XX
    
#if defined(STM32F1XX)
    UART5
#elif defined(STM32F4XX)
    UART5,
    USART6
#endif //:~STM32F1XX
};


/****************************************************************************************
* 函数名称: STM32Fxxx_SerialInit()
* 功能描述: 初始化串口
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
BOOL STM32Fxxx_SerialInit(SERIAL_Port port, SERIAL_BaudRate baud, SERIAL_Parity parity,
		SERIAL_ByteBits bits, SERIAL_StopBits stop, STM32Fxxx_SerRxPtr handler)
{
    FlagStatus              tc_status;

    USART_InitTypeDef       usart_init;
    USART_ClockInitTypeDef  usart_clk_init;
    
#if (defined(APP_OS))
	CPU_INT08U  			os_err;
	CPU_CHAR				name[30];
#endif  //#if (defined(APP_OS))

	if((port>=SERIAL_PORT_MAX) ||(baud>=SERIAL_BAUDRATE_MAX)|| (parity>=SERIAL_PARITY_MAX) ||
		(bits>=SERIAL_BYTE_BITS_MAX) || (stop>=SERIAL_STOP_MAX))
		return FALSE;

	if(!handler)
		return FALSE;

#if (defined(APP_OS))
	if(abSerial_Used[port] == FALSE)
	{
		sprintf((char*)name, "Ser%u Lock Sem", (port+1));
		os_err = BSP_OS_SemCreate(&Serial_TXLockSem[port],     1, name);
		if (os_err == DEF_FAIL) return FALSE;
	}
#endif  //#if (defined(APP_OS))

    usart_init.USART_BaudRate            = Serial_BaudrateDict[baud];
    usart_init.USART_WordLength          = (parity!=SERIAL_PARITY_NONE)?USART_WordLength_9b:Serial_ByteBitsDict[bits];;
    usart_init.USART_StopBits            = Serial_StopBitsDict[stop];
    usart_init.USART_Parity              = Serial_ParityDict[parity] ;
    usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_init.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

    usart_clk_init.USART_Clock           = USART_Clock_Disable;
    usart_clk_init.USART_CPOL            = USART_CPOL_Low;
    usart_clk_init.USART_CPHA            = USART_CPHA_2Edge;
    usart_clk_init.USART_LastBit         = USART_LastBit_Disable;

    Serial_GPIOInit[port]();

    USART_Init((USART_TypeDef *)Serial_PERIPH_ID[port], &usart_init);
    USART_ClockInit((USART_TypeDef *)Serial_PERIPH_ID[port], &usart_clk_init);
    USART_Cmd((USART_TypeDef *)Serial_PERIPH_ID[port], ENABLE);

    USART_ITConfig((USART_TypeDef *)Serial_PERIPH_ID[port], USART_IT_TC, DISABLE);
    USART_ITConfig((USART_TypeDef *)Serial_PERIPH_ID[port], USART_IT_TXE, DISABLE);
    tc_status  = USART_GetFlagStatus((USART_TypeDef *)Serial_PERIPH_ID[port], USART_FLAG_TC);

    while (tc_status == SET)
    {
        USART_ClearITPendingBit((USART_TypeDef *)Serial_PERIPH_ID[port], USART_IT_TC);
        USART_ClearFlag((USART_TypeDef *)Serial_PERIPH_ID[port], USART_IT_TC);
        BSP_OS_TimeDlyMs(10);
        tc_status = USART_GetFlagStatus((USART_TypeDef *)Serial_PERIPH_ID[port], USART_FLAG_TC);
    }

    Serial_RxHandler[port] = handler;
	USART_ITConfig((USART_TypeDef *)Serial_PERIPH_ID[port], USART_IT_RXNE, ENABLE);
#if (defined(APP_OS))
    BSP_IntVectSet(Serial_INT_ID[port], Serial_EventISR_Handler[port]);
    BSP_IntEn(Serial_INT_ID[port]);
#else
#if (defined(STM32F1XX))
    NVIC_SetPriority(a_SerialISRTypePrio[port].irq_type, NVIC_EncodePriority(NVIC_PRIORITY_GROUP, a_SerialISRTypePrio[port].pre_prio, a_SerialISRTypePrio[port].sub_prio));
#elif (defined(STM32F4XX))
    
#elif (defined(STM32F0XX))
    NVIC_SetPriority(a_SerialISRTypePrio[port].irq_type, a_SerialISRTypePrio[port].pre_prio);
#endif  //#if (defined(STM32F1XX))
    NVIC_EnableIRQ(a_SerialISRTypePrio[port].irq_type);
#endif  //#if (defined(APP_OS))

#if (defined(APP_OS))
    abSerial_Used[port] = TRUE;
#endif  //#if (defined(APP_OS))

    return TRUE;
}

/****************************************************************************************
* 函数名称: STM32Fxxx_ExtiISREnable()
* 功能描述: 串口中断允许
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void STM32Fxxx_SerialISREnable(SERIAL_Port port)
{
    if(port >= SERIAL_PORT_MAX)
		return;
    
#if defined(APP_OS)
    BSP_IntEn(Serial_INT_ID[port]);
#else
    NVIC_EnableIRQ(a_SerialISRTypePrio[port].irq_type);
#endif  //#if defined(APP_OS)
}

/****************************************************************************************
* 函数名称: STM32Fxxx_SerialISRDisable()
* 功能描述: 串口中断禁止
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void STM32Fxxx_SerialISRDisable(SERIAL_Port port)
{
    if(port >= SERIAL_PORT_MAX)
		return;
    
#if defined(APP_OS)
    BSP_IntDis(Serial_INT_ID[port]);
#else
    NVIC_DisableIRQ(a_SerialISRTypePrio[port].irq_type);
#endif  //#if defined(APP_OS)
}

#if defined(APP_OS)
/****************************************************************************************
* 函数名称: STM32Fxxx_SerialLock()
* 功能描述: 锁定串口
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
BOOL STM32Fxxx_SerialLock(SERIAL_Port port, CPU_INT32U dly_ms)
{
    if(port>=SERIAL_PORT_MAX)
		return FALSE;
    
//    if(Serial_TXSem[port]==NULL)
//        return FALSE;
    
	return (BSP_OS_SemWaitFlag(&Serial_TXLockSem[port], dly_ms)==DEF_TRUE)?TRUE:FALSE;
}

/****************************************************************************************
* 函数名称: STM32Fxxx_SerialUnlock()
* 功能描述: 解锁串口
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
BOOL STM32Fxxx_SerialUnlock(SERIAL_Port port)
{
    if(port>=SERIAL_PORT_MAX)
		return FALSE;

//    if(Serial_TXSem[port]==NULL)
//        return FALSE;

	BSP_OS_SemPostFlag(&Serial_TXLockSem[port]);

	return TRUE;
}
#endif  //#if defined(APP_OS)

/****************************************************************************************
* 函数名称: STM32Fxxx_SerialWriteByte()
* 功能描述: 发送数据
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
BOOL STM32Fxxx_SerialWriteByte(SERIAL_Port port, char byte)
{
	if(port >= SERIAL_PORT_MAX)
		return FALSE;

    USART_SendData((USART_TypeDef *)Serial_PERIPH_ID[port], byte);
    while(USART_GetFlagStatus((USART_TypeDef *)Serial_PERIPH_ID[port], USART_FLAG_TXE) == RESET)	  // Loop until USARTy DR register is empty
    {
    }

	return TRUE;
}

/****************************************************************************************
* 函数名称: STM32Fxxx_SerialWriteBlock()
* 功能描述: 发送数据
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
BOOL STM32Fxxx_SerialWriteBlock(SERIAL_Port port, char *payload, int length)
{
	if(port>=SERIAL_PORT_MAX)
		return FALSE;

	return TRUE;
}

/****************************************************************************************
* 函数名称: STM32Fxxx_Serail_EventISR_Handler()
* 功能描述: 中断函数
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
static void STM32Fxxx_Serail_EventISR_Handler(SERIAL_Port port)
{
	USART_TypeDef 	*uart = (USART_TypeDef 	*)Serial_PERIPH_ID[port];
//    FlagStatus 		rxne_status;
    int		  		c;
    
//    __disable_irq();
    
    if (SET == USART_GetFlagStatus(uart, USART_FLAG_RXNE)) 
    {
        c = USART_ReceiveData(uart) & 0xFF;
        USART_ClearITPendingBit(uart, USART_IT_RXNE);
        if(Serial_RxHandler[port])
        	Serial_RxHandler[port](c);
    }
    
//    __enable_irq();
}

/****************************************************************************************
* 函数名称: STM32Fxxx_Ser1GPIOInit()
* 功能描述: IO初始化
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
static void STM32Fxxx_Ser1GPIOInit(void)
{
#if (defined(STM32FXXX_SERIAL1_ENABLE)&&STM32FXXX_SERIAL1_ENABLE)

	GPIO_InitTypeDef        gpio_init;

#if (defined(APP_OS))
    BSP_PeriphEn(UART1_PERIPH_ID);
    BSP_PeriphEn(UART1_TX_PERIPH_ID);
    BSP_PeriphEn(UART1_RX_PERIPH_ID);
#else
#if defined(STM32F1XX)
    RCC_APB2PeriphClockCmd(UART1_PERIPH_ID, ENABLE);
    RCC_APB2PeriphClockCmd(UART1_TX_PERIPH_ID | UART1_RX_PERIPH_ID, ENABLE);
#elif (defined(STM32F4XX))
    
#elif (defined(STM32F0XX))
    RCC_APB2PeriphClockCmd(UART1_PERIPH_ID, ENABLE);
    RCC_AHBPeriphClockCmd(UART1_TX_PERIPH_ID | UART1_RX_PERIPH_ID, ENABLE);
#endif  //#if defined(STM32F1XX)
#endif  //#if (defined(APP_OS))

#if defined(STM32F1XX)
#if (defined(APP_OS))
    BSP_PeriphEn(BSP_PERIPH_ID_AFIO);
#else
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
#endif  //#if (defined(APP_OS))

#if UART1_REMAP_ENABLE
    GPIO_PinRemapConfig(UART1_REMAP_ID, ENABLE);
#endif                                                         
                                                               
    gpio_init.GPIO_Pin   = UART1_TX_PIN;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(UART1_TX_PORT, &gpio_init);

                                                            
    gpio_init.GPIO_Pin   = UART1_RX_PIN;
    gpio_init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(UART1_RX_PORT, &gpio_init);

#endif //:~STM32F1XX

#if defined(STM32F4XX)
    gpio_init.GPIO_OType 	= GPIO_OType_PP;					
    gpio_init.GPIO_PuPd 	= GPIO_PuPd_UP;
    gpio_init.GPIO_Mode 	= GPIO_Mode_AF;
    gpio_init.GPIO_Speed 	= GPIO_Speed_50MHz;
    gpio_init.GPIO_Pin 		= UART1_TX_PIN;
	GPIO_Init(UART1_TX_PORT, &gpio_init);

	gpio_init.GPIO_Pin 		= UART1_RX_PIN;				
	GPIO_Init(UART1_RX_PORT, &gpio_init);

	GPIO_PinAFConfig(UART1_TX_PORT, UART1_TX_PIN_SOURCE, UART1_TX_AF);	
	GPIO_PinAFConfig(UART1_RX_PORT, UART1_RX_PIN_SOURCE, UART1_RX_AF);	
#endif //:~STM32F4XX

#if defined(STM32F0XX)
    gpio_init.GPIO_OType 	= GPIO_OType_PP;
    gpio_init.GPIO_PuPd 	= GPIO_PuPd_UP;
    gpio_init.GPIO_Mode 	= GPIO_Mode_AF;
    gpio_init.GPIO_Speed 	= GPIO_Speed_50MHz;
    gpio_init.GPIO_Pin 		= UART1_TX_PIN;
	GPIO_Init(UART1_TX_PORT, &gpio_init);

	gpio_init.GPIO_Pin 		= UART1_RX_PIN;				
	GPIO_Init(UART1_RX_PORT, &gpio_init);

	GPIO_PinAFConfig(UART1_TX_PORT, UART1_TX_PIN_SOURCE, UART1_TX_PIN_AF);	
	GPIO_PinAFConfig(UART1_RX_PORT, UART1_RX_PIN_SOURCE, UART1_RX_PIN_AF);	
#endif  //#if defined(STM32F0XX)
#endif
}

/****************************************************************************************
* 函数名称: STM32Fxxx_Ser1GPIOInit()
* 功能描述: IO初始化
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
static void STM32Fxxx_Ser2GPIOInit(void)
{
#if STM32FXXX_SERIAL2_ENABLE
	GPIO_InitTypeDef        gpio_init;

#if (defined(APP_OS))
    BSP_PeriphEn(UART2_PERIPH_ID);
    BSP_PeriphEn(UART2_TX_PERIPH_ID);
    BSP_PeriphEn(UART2_RX_PERIPH_ID);
#else
    RCC_APB1PeriphClockCmd(UART2_PERIPH_ID, ENABLE);
    RCC_APB2PeriphClockCmd(UART2_TX_PERIPH_ID | UART2_RX_PERIPH_ID, ENABLE);
#endif  //#if (defined(APP_OS))

#if defined(STM32F1XX)
#if (defined(APP_OS))
    BSP_PeriphEn(BSP_PERIPH_ID_AFIO);
#else
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
#endif  //#if (defined(APP_OS))

#if UART2_REMAP_ENABLE
    GPIO_PinRemapConfig(UART2_REMAP_ID, ENABLE);
#endif

    gpio_init.GPIO_Pin   = UART2_TX_PIN;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(UART2_TX_PORT, &gpio_init);

    gpio_init.GPIO_Pin   = UART2_RX_PIN;
    gpio_init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(UART2_RX_PORT, &gpio_init);

#endif //:~STM32F1XX

#if defined(STM32F4XX)
    gpio_init.GPIO_OType 	= GPIO_OType_PP;
    gpio_init.GPIO_PuPd 	= GPIO_PuPd_UP;
    gpio_init.GPIO_Mode 	= GPIO_Mode_AF;
    gpio_init.GPIO_Speed 	= GPIO_Speed_50MHz;
    gpio_init.GPIO_Pin 		= UART2_TX_PIN;
	GPIO_Init(UART2_TX_PORT, &gpio_init);

	gpio_init.GPIO_Pin 		= UART2_RX_PIN;
	GPIO_Init(UART2_RX_PORT, &gpio_init);

	GPIO_PinAFConfig(UART2_TX_PORT, UART2_TX_PIN_SOURCE, UART2_TX_AF);
	GPIO_PinAFConfig(UART2_RX_PORT, UART2_RX_PIN_SOURCE, UART2_RX_AF);
#endif //:~STM32F4XX

#endif //:~STM32FXXX_SERIAL2_ENABLE
}

/****************************************************************************************
* 函数名称: STM32Fxxx_Ser1GPIOInit()
* 功能描述: IO初始化
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
static void STM32Fxxx_Ser3GPIOInit(void)
{
#if STM32FXXX_SERIAL3_ENABLE
	GPIO_InitTypeDef        gpio_init;

#if (defined(APP_OS))
    BSP_PeriphEn(UART3_PERIPH_ID);
    BSP_PeriphEn(UART3_TX_PERIPH_ID);
    BSP_PeriphEn(UART3_RX_PERIPH_ID);
#else
    RCC_APB1PeriphClockCmd(UART3_PERIPH_ID, ENABLE);
    RCC_APB2PeriphClockCmd(UART3_TX_PERIPH_ID | UART3_RX_PERIPH_ID, ENABLE);
#endif  //#if (defined(APP_OS))

#if defined(STM32F1XX)
#if (defined(APP_OS))
    BSP_PeriphEn(BSP_PERIPH_ID_AFIO);
#else
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
#endif  //#if (defined(APP_OS))

#if UART3_REMAP_ENABLE
    GPIO_PinRemapConfig(UART3_REMAP_ID, ENABLE);
#endif                                                               
                                                          
    gpio_init.GPIO_Pin   = UART3_TX_PIN;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(UART3_TX_PORT, &gpio_init);

                                                               
    gpio_init.GPIO_Pin   = UART3_RX_PIN;
    gpio_init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(UART3_RX_PORT, &gpio_init);

#endif //:~STM32F1XX

#if defined(STM32F4XX)
    gpio_init.GPIO_OType 	= GPIO_OType_PP;					
    gpio_init.GPIO_PuPd 	= GPIO_PuPd_UP;
    gpio_init.GPIO_Mode 	= GPIO_Mode_AF;
    gpio_init.GPIO_Speed 	= GPIO_Speed_50MHz;
    gpio_init.GPIO_Pin 		= UART3_TX_PIN;
	GPIO_Init(UART3_TX_PORT, &gpio_init);

	gpio_init.GPIO_Pin 		= UART3_RX_PIN;				
	GPIO_Init(UART3_RX_PORT, &gpio_init);

	GPIO_PinAFConfig(UART3_TX_PORT, UART3_TX_PIN_SOURCE, UART3_TX_AF);	
	GPIO_PinAFConfig(UART3_RX_PORT, UART3_RX_PIN_SOURCE, UART3_RX_AF);	
#endif //:~STM32F4XX
#endif
}

/****************************************************************************************
* 函数名称: STM32Fxxx_Ser1GPIOInit()
* 功能描述: IO初始化
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
static void STM32Fxxx_Ser4GPIOInit(void)
{
#if STM32FXXX_SERIAL4_ENABLE
	GPIO_InitTypeDef        gpio_init;

#if (defined(APP_OS))
    BSP_PeriphEn(UART4_PERIPH_ID);
    BSP_PeriphEn(UART4_TX_PERIPH_ID);
    BSP_PeriphEn(UART4_RX_PERIPH_ID);
#else
    RCC_APB1PeriphClockCmd(UART4_PERIPH_ID, ENABLE);
    RCC_APB2PeriphClockCmd(UART4_TX_PERIPH_ID | UART4_RX_PERIPH_ID, ENABLE);
#endif  //#if (defined(APP_OS))

#if defined(STM32F1XX)
#if (defined(APP_OS))
    BSP_PeriphEn(BSP_PERIPH_ID_AFIO);
#else
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
#endif  //#if (defined(APP_OS))

#if UART4_REMAP_ENABLE
    GPIO_PinRemapConfig(UART4_REMAP_ID, ENABLE);
#endif                                                               
                                                              
    gpio_init.GPIO_Pin   = UART4_TX_PIN;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(UART4_TX_PORT, &gpio_init);

                                                                
    gpio_init.GPIO_Pin   = UART4_RX_PIN;
    //gpio_init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;

	gpio_init.GPIO_Mode = GPIO_Mode_IPU;

    GPIO_Init(UART4_RX_PORT, &gpio_init);

#endif //:~STM32F1XX

#if defined(STM32F4XX)
    gpio_init.GPIO_OType 	= GPIO_OType_PP;					
    gpio_init.GPIO_PuPd 	= GPIO_PuPd_UP;
    gpio_init.GPIO_Mode 	= GPIO_Mode_AF;
    gpio_init.GPIO_Speed 	= GPIO_Speed_50MHz;
    gpio_init.GPIO_Pin 		= UART4_TX_PIN;
	GPIO_Init(UART4_TX_PORT, &gpio_init);

	gpio_init.GPIO_Pin 		= UART4_RX_PIN;				
	GPIO_Init(UART4_RX_PORT, &gpio_init);

	GPIO_PinAFConfig(UART4_TX_PORT, UART4_TX_PIN_SOURCE, UART4_TX_AF);	
	GPIO_PinAFConfig(UART4_RX_PORT, UART4_RX_PIN_SOURCE, UART4_RX_AF);	
#endif //:~STM32F4XX
#endif
}

#if (defined(STM32F1XX) || defined(STM32F4XX))
/****************************************************************************************
* 函数名称: STM32Fxxx_Ser1GPIOInit()
* 功能描述: IO初始化
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
static void STM32Fxxx_Ser5GPIOInit(void)
{
#if STM32FXXX_SERIAL5_ENABLE
	GPIO_InitTypeDef        gpio_init;

#if (defined(APP_OS))
    BSP_PeriphEn(UART5_PERIPH_ID);
    BSP_PeriphEn(UART5_TX_PERIPH_ID);
    BSP_PeriphEn(UART5_RX_PERIPH_ID);
#else
    RCC_APB1PeriphClockCmd(UART5_PERIPH_ID, ENABLE);
    RCC_APB2PeriphClockCmd(UART5_TX_PERIPH_ID | UART5_RX_PERIPH_ID, ENABLE);
#endif  //#if (defined(APP_OS))

#if defined(STM32F1XX)
#if (defined(APP_OS))
    BSP_PeriphEn(BSP_PERIPH_ID_AFIO);
#else
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
#endif  //#if (defined(APP_OS))

#if UART5_REMAP_ENABLE
    GPIO_PinRemapConfig(UART5_REMAP_ID, ENABLE);
#endif                                                                
                                                                
    gpio_init.GPIO_Pin   = UART5_TX_PIN;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(UART5_TX_PORT, &gpio_init);

                                                              
    gpio_init.GPIO_Pin   = UART5_RX_PIN;
    gpio_init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(UART5_RX_PORT, &gpio_init);

#endif //:~STM32F1XX

#if defined(STM32F4XX)
    gpio_init.GPIO_OType 	= GPIO_OType_PP;					
    gpio_init.GPIO_PuPd 	= GPIO_PuPd_UP;
    gpio_init.GPIO_Mode 	= GPIO_Mode_AF;
    gpio_init.GPIO_Speed 	= GPIO_Speed_50MHz;
    gpio_init.GPIO_Pin 		= UART5_TX_PIN;
	GPIO_Init(UART5_TX_PORT, &gpio_init);

	gpio_init.GPIO_Pin 		= UART5_RX_PIN;				
	GPIO_Init(UART5_RX_PORT, &gpio_init);

	GPIO_PinAFConfig(UART5_TX_PORT, UART5_TX_PIN_SOURCE, UART5_TX_AF);	
	GPIO_PinAFConfig(UART5_RX_PORT, UART5_RX_PIN_SOURCE, UART5_RX_AF);	
#endif //:~STM32F4XX
#endif
}
#endif  //#if (defined(STM32F1XX) || defined(STM32F4XX))

#if defined(STM32F4XX)
/****************************************************************************************
* 函数名称: STM32Fxxx_Ser1GPIOInit()
* 功能描述: IO初始化
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
static void STM32Fxxx_Ser6GPIOInit(void)
{
#if STM32FXXX_SERIAL6_ENABLE

	GPIO_InitTypeDef        gpio_init;

#if (defined(APP_OS))
    BSP_PeriphEn(UART6_PERIPH_ID);
    BSP_PeriphEn(UART6_TX_PERIPH_ID);
    BSP_PeriphEn(UART6_RX_PERIPH_ID);
#else
    RCC_APB1PeriphClockCmd(UART6_PERIPH_ID, ENABLE);
    RCC_APB2PeriphClockCmd(UART6_TX_PERIPH_ID | UART6_RX_PERIPH_ID, ENABLE);
#endif  //#if (defined(APP_OS))

#if defined(STM32F1XX)
#if (defined(APP_OS))
    BSP_PeriphEn(BSP_PERIPH_ID_AFIO);
#else
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
#endif  //#if (defined(APP_OS))

#if UART6_REMAP_ENABLE
    GPIO_PinRemapConfig(UART6_REMAP_ID, ENABLE);
#endif                                                                
                                                                
    gpio_init.GPIO_Pin   = UART6_TX_PIN;
    gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_init.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(UART6_TX_PORT, &gpio_init);

                                                                
    gpio_init.GPIO_Pin   = UART6_RX_PIN;
    gpio_init.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(UART6_RX_PORT, &gpio_init);

#endif //:~STM32F1XX

#if defined(STM32F4XX)
    gpio_init.GPIO_OType 	= GPIO_OType_PP;					
    gpio_init.GPIO_PuPd 	= GPIO_PuPd_UP;
    gpio_init.GPIO_Mode 	= GPIO_Mode_AF;
    gpio_init.GPIO_Speed 	= GPIO_Speed_50MHz;
    gpio_init.GPIO_Pin 		= UART6_TX_PIN;
	GPIO_Init(UART6_TX_PORT, &gpio_init);

	gpio_init.GPIO_Pin 		= UART6_RX_PIN;				
	GPIO_Init(UART6_RX_PORT, &gpio_init);

	GPIO_PinAFConfig(UART6_TX_PORT, UART6_TX_PIN_SOURCE, UART6_TX_AF);	
	GPIO_PinAFConfig(UART6_RX_PORT, UART6_RX_PIN_SOURCE, UART6_RX_AF);	
#endif //:~STM32F4XX
#endif
}
#endif  //defined(STM32F4XX)

#if (defined(APP_OS))
/****************************************************************************************
* 函数名称: 
* 功能描述: 中断函数
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void STM32Fxxx_Serail1_EventISR_Handler(void)
{
	STM32Fxxx_Serail_EventISR_Handler(SERIAL_PORT1);
}

/****************************************************************************************
* 函数名称: 
* 功能描述: 中断函数
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void STM32Fxxx_Serail2_EventISR_Handler(void)
{
	STM32Fxxx_Serail_EventISR_Handler(SERIAL_PORT2);
}

/****************************************************************************************
* 函数名称: 
* 功能描述: 中断函数
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void STM32Fxxx_Serail3_EventISR_Handler(void)
{
	STM32Fxxx_Serail_EventISR_Handler(SERIAL_PORT3);
}

/****************************************************************************************
* 函数名称: 
* 功能描述: 中断函数
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void STM32Fxxx_Serail4_EventISR_Handler(void)
{
	STM32Fxxx_Serail_EventISR_Handler(SERIAL_PORT4);
}

/****************************************************************************************
* 函数名称: 
* 功能描述: 中断函数
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void STM32Fxxx_Serail5_EventISR_Handler(void)
{
	STM32Fxxx_Serail_EventISR_Handler(SERIAL_PORT5);
}

#if defined(STM32F4XX)
/****************************************************************************************
* 函数名称: 
* 功能描述: 中断函数
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void STM32Fxxx_Serail6_EventISR_Handler(void)
{
	STM32Fxxx_Serail_EventISR_Handler(SERIAL_PORT6);
}
#endif //:~STM32F4XX
#else
/****************************************************************************************
* 函数名称: USART1_IRQHandler()
* 功能描述: 中断函数
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void USART1_IRQHandler(void)
{
    STM32Fxxx_Serail_EventISR_Handler(SERIAL_PORT1);
}

/****************************************************************************************
* 函数名称: USART2_IRQHandler()
* 功能描述: 中断函数
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void USART2_IRQHandler(void)
{
    STM32Fxxx_Serail_EventISR_Handler(SERIAL_PORT2);
}

/****************************************************************************************
* 函数名称: USART3_IRQHandler()
* 功能描述: 中断函数
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void USART3_IRQHandler(void)
{
    STM32Fxxx_Serail_EventISR_Handler(SERIAL_PORT3);
}

#if defined(STM32F1XX)
/****************************************************************************************
* 函数名称: UART4_IRQHandler()
* 功能描述: 中断函数
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void UART4_IRQHandler(void)
{
    STM32Fxxx_Serail_EventISR_Handler(SERIAL_PORT4);
}

/****************************************************************************************
* 函数名称: UART5_IRQHandler()
* 功能描述: 中断函数
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void UART5_IRQHandler(void)
{
    STM32Fxxx_Serail_EventISR_Handler(SERIAL_PORT5);
}
#elif (defined(STM32F4XX))
/****************************************************************************************
* 函数名称: USART4_IRQHandler()
* 功能描述: 中断函数
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void USART4_IRQHandler(void)
{
    STM32Fxxx_Serail_EventISR_Handler(SERIAL_PORT4);
}

/****************************************************************************************
* 函数名称: USART5_IRQHandler()
* 功能描述: 中断函数
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void USART5_IRQHandler(void)
{
    STM32Fxxx_Serail_EventISR_Handler(SERIAL_PORT5);
}

/****************************************************************************************
* 函数名称: USART6_IRQHandler()
* 功能描述: 中断函数
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void USART6_IRQHandler(void)
{
    STM32Fxxx_Serail_EventISR_Handler(SERIAL_PORT6);
}
#endif  //#if defined(STM32F1XX)
#endif  //#if (defined(APP_OS))

/*******************************版权所有(c)**************END OF FILE********************/
