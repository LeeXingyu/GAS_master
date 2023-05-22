/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210E-EVAL Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : BAN
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


/*
*********************************************************************************************************
*                                       STM32FXXX WATCH DOG ENABLE / DISABLE
*********************************************************************************************************
*/
#if (defined(PLC_JZQ_DEBUG))
#define  STM32FXXX_WATCH_DOG				        DEF_ENABLED					//ucos服务看门狗是否打开
#define  STM32FXXX_WATCH_DOG_DBG_INFO               DEF_ENABLED
#define  STM32FXXX_WATCH_DOG_REAL					DEF_DISABLED			    //iwdg是否打开
#endif  //#if (defined(PLC_JZQ_DEBUG))

#if (defined(PLC_JZQ_APP))
#define  STM32FXXX_WATCH_DOG				        DEF_ENABLED
#define  STM32FXXX_WATCH_DOG_DBG_INFO               DEF_ENABLED
#define  STM32FXXX_WATCH_DOG_REAL					DEF_ENABLED					//iwdg是否打开
#endif  //#if (defined(PLC_JZQ_APP))

#if (defined(STM32FXXX_WATCH_DOG) && (STM32FXXX_WATCH_DOG))
#define STM32FXXX_WATCH_DOG_PRESCALER				(IWDG_Prescaler_128)		//看门狗时钟分频系数(40KHz分频不固定)
#define STM32FXXX_WATCH_DOG_RELOAD					(2500u)						//最长3.2ms*X看门狗清空
#endif	//#if (defined(STM32FXXX_WATCH_DOG) && (STM32FXXX_WATCH_DOG))

#define BSP_OS_WATCH_DOG_RELOAD						(8000u)					    //看门狗应用程序，清空看门狗的时间
																				//主程序看门狗查询时间
#define BSP_OS_WATCH_DOG_SERVER_CLR					((BSP_OS_WATCH_DOG_RELOAD / 1000) >> 1)


/*
*********************************************************************************************************
*                                       APP DEBUG ENABLE / DISABLE
*********************************************************************************************************
*/
#if (defined(CODE_DEBUG))
//#define	DEBUG_EEPROM                                DEF_ENABLED
#define DEBUG_RTC                                   DEF_ENABLED

//#define DEBUG_LED_SEG                               DEF_ENABLED

#define DEBUG_BUZZER                                DEF_ENABLED

#define DEBUG_HT1621                                DEF_ENABLED


//#define DEBUG_74HC164_LED                           DEF_ENABLED

//#define DEBUG_PLCP									DEF_ENABLED					//测试新电力线载波协议节点部分

//#if ((defined(PLC_JZQ_HARDWARE_GWDLX)) || (defined(PLC_JZQ_HARDWARE_KG7000A)))
//#define DEBUG_PLC_REGISTER							DEF_DISABLED				//测试节点自动注册
//#endif  //#if ((defined(PLC_JZQ_HARDWARE_GWDLX)) || (defined(PLC_JZQ_HARDWARE_KG7000A)))

//#if (defined(PLC_JZQ_HARDWARE_PARK_V3))
//#define DEBUG_W5500									DEF_ENABLED
//#endif  //#if (defined(PLC_JZQ_HARDWARE_PARK_V3))

//#define DEBUG_TASK_STK_CHK                          DEF_ENABLED

//#define DEBUG_HEAP                                  DEF_ENABLED                 //动态内存分配使用测试

//#define DEBUG_NODE                                  DEF_ENABLED

//#define DEBUG_HFP2014_MAIN                          DEF_ENABLED                 //主任务调试

//#define OS_TASK_RUN_TIME_DEBUG						DEF_DISABLED				//任务运行时间统计

//#if (defined(PLC_JZQ_HARDWARE_PARK))
//#define DEBUG_ESP8266								DEF_ENABLED					//调试ESP8266

//#define DEBUG_SHOW_WIFI_PARA						DEF_ENABLED					//显示wifi连接参数
//#endif  //#if (defined(PLC_JZQ_HARDWARE_PARK))

//#define DEBUG_TEST_BUG								DEF_DISABLED				//Bug测试

//#define DEBUG_W25Q128FV								DEF_ENABLED					//测试flash

#define DEBUG_SX1212								DEF_ENABLED
#endif  //#if (defined(CODE_DEBUG))



#if (defined(CODE_RELEASE))
//#define	DEBUG_EEPROM                                DEF_DISABLED
//#define DEBUG_RTC                                   DEF_DISABLED

//#define DEBUG_PLCP									DEF_DISABLED

//#if ((defined(PLC_JZQ_HARDWARE_GWDLX)) || (defined(PLC_JZQ_HARDWARE_KG7000A)))
//#define DEBUG_PLC_REGISTER							DEF_DISABLED
//#endif  //#if ((defined(PLC_JZQ_HARDWARE_GWDLX)) || (defined(PLC_JZQ_HARDWARE_KG7000A)))

//#if (defined(PLC_JZQ_HARDWARE_PARK_V3))
//#define DEBUG_W5500									DEF_ENABLED
//#endif  //#if (defined(PLC_JZQ_HARDWARE_PARK_V3))

//#define DEBUG_TASK_STK_CHK                          DEF_ENABLED

//#define DEBUG_NODE                                  DEF_ENABLED

//#define DEBUG_HFP2014_MAIN                          DEF_ENABLED                 //主任务调试

//#define OS_TASK_RUN_TIME_DEBUG						DEF_DISABLED

//#if (defined(PLC_JZQ_HARDWARE_PARK))
//#define DEBUG_ESP8266								DEF_ENABLED					//调试ESP8266

//#define DEBUG_SHOW_WIFI_PARA						DEF_DISABLED				//显示wifi连接参数
//#endif  //#if (defined(PLC_JZQ_HARDWARE_PARK))

//#define DEBUG_TEST_BUG								DEF_DISABLED				//Bug测试
#endif  //#if (defined(CODE_RELEASE))

/*
*********************************************************************************************************
*                                       SERIAL ENABLE / DISABLE
*********************************************************************************************************
*/
#define  STM32FXXX_SERIAL1_ENABLE					DEF_ENABLED
#define  STM32FXXX_SERIAL2_ENABLE					DEF_DISABLED
#define  STM32FXXX_SERIAL3_ENABLE					DEF_DISABLED
#define  STM32FXXX_SERIAL4_ENABLE					DEF_DISABLED
#define  STM32FXXX_SERIAL5_ENABLE					DEF_DISABLED
#define  STM32FXXX_SERIAL6_ENABLE					DEF_DISABLED

/*
*********************************************************************************************************
*                                     SERIAL1 CONFIGURATION
*********************************************************************************************************
*/
#if (defined(APP_OS))
#define  UART1_PERIPH_ID							BSP_PERIPH_ID_USART1
#define  UART1_TX_PERIPH_ID							BSP_PERIPH_ID_IOPA
#define  UART1_RX_PERIPH_ID							BSP_PERIPH_ID_IOPA
#else
#if (defined(STM32F0XX))
#define  UART1_PERIPH_ID							RCC_APB2Periph_USART1
#define  UART1_TX_PERIPH_ID							RCC_AHBPeriph_GPIOA
#define  UART1_RX_PERIPH_ID							RCC_AHBPeriph_GPIOA
#elif (defined(STM32F1XX))

#elif (defined(STM32F4XX))

#endif  //#if (defined(STM32F0XX))
#endif  //APP_OS

#define  UART1_REMAP_ENABLE							0

#if (defined(UART1_REMAP_ENABLE) && (UART1_REMAP_ENABLE))
#define  UART1_REMAP_ID
#endif  //#if (defined(UART1_REMAP_ENABLE) && (UART1_REMAP_ENABLE))

#define  UART1_TX_PORT								GPIOA
#define	 UART1_RX_PORT								GPIOA

#define  UART1_TX_PIN								GPIO_Pin_9
#define  UART1_RX_PIN								GPIO_Pin_10

#if (defined(STM32F0XX))
#define  UART1_TX_PIN_SOURCE                        GPIO_PinSource9
#define  UART1_RX_PIN_SOURCE                        GPIO_PinSource10

#define  UART1_TX_PIN_AF                            GPIO_AF_1
#define  UART1_RX_PIN_AF                            GPIO_AF_1
#elif (defined(STM32F4XX))
#define  UART1_TX_PIN_SOURCE                        GPIO_PinSource9
#define  UART1_RX_PIN_SOURCE                        GPIO_PinSource10
#endif  //#if ((defined(STM32F0XX) || (defined(STM32F4XX))))
/*
*********************************************************************************************************
*                                     SERIAL2 CONFIGURATION
*********************************************************************************************************
*/
#define  UART2_PERIPH_ID							BSP_PERIPH_ID_USART2
#define  UART2_TX_PERIPH_ID							BSP_PERIPH_ID_IOPA
#define  UART2_RX_PERIPH_ID							BSP_PERIPH_ID_IOPA

#define  UART2_REMAP_ENABLE							0

#if (defined(UART2_REMAP_ENABLE) && (UART2_REMAP_ENABLE))
#define  UART2_REMAP_ID
#endif  //#if (defined(UART2_REMAP_ENABLE) && (UART2_REMAP_ENABLE))

#define  UART2_TX_PORT								GPIOA
#define	 UART2_RX_PORT								GPIOA

#define  UART2_TX_PIN								GPIO_Pin_2
#define  UART2_RX_PIN								GPIO_Pin_3

/*
*********************************************************************************************************
*                                     SERIAL3 CONFIGURATION
*********************************************************************************************************
*/
#define  UART3_PERIPH_ID							BSP_PERIPH_ID_USART3
#define  UART3_TX_PERIPH_ID							BSP_PERIPH_ID_IOPD
#define  UART3_RX_PERIPH_ID							BSP_PERIPH_ID_IOPD

#define  UART3_REMAP_ENABLE							0

#if (defined(UART3_REMAP_ENABLE) && (UART3_REMAP_ENABLE))
#define  UART3_REMAP_ID                             GPIO_FullRemap_USART3
#endif  //#if (defined(UART3_REMAP_ENABLE) && (UART3_REMAP_ENABLE))

#define  UART3_TX_PORT								GPIOD
#define	 UART3_RX_PORT								GPIOD

#define  UART3_TX_PIN								GPIO_Pin_8
#define  UART3_RX_PIN								GPIO_Pin_9

/*
*********************************************************************************************************
*                                     SERIAL4 CONFIGURATION
*********************************************************************************************************
*/

#define  UART4_PERIPH_ID							BSP_PERIPH_ID_USART4
#define  UART4_TX_PERIPH_ID							BSP_PERIPH_ID_IOPC
#define  UART4_RX_PERIPH_ID							BSP_PERIPH_ID_IOPC

#define  UART4_REMAP_ENABLE							0

#if (defined(UART4_REMAP_ENABLE) && (UART4_REMAP_ENABLE))
#define  UART4_REMAP_ID
#endif  //#if (defined(UART4_REMAP_ENABLE) && (UART4_REMAP_ENABLE))

#define  UART4_TX_PORT								GPIOC
#define	 UART4_RX_PORT								GPIOC

#define  UART4_TX_PIN								GPIO_Pin_10
#define  UART4_RX_PIN								GPIO_Pin_11

/*
*********************************************************************************************************
*                                     SERIAL5 CONFIGURATION
*********************************************************************************************************
*/

#define  UART5_PERIPH_ID							BSP_PERIPH_ID_USART5
#define  UART5_TX_PERIPH_ID							BSP_PERIPH_ID_IOPC
#define  UART5_RX_PERIPH_ID							BSP_PERIPH_ID_IOPD

#define  UART5_REMAP_ENABLE							0

#if (defined(UART5_REMAP_ENABLE) && (UART5_REMAP_ENABLE))
#define  UART5_REMAP_ID
#endif  //#if (defined(UART5_REMAP_ENABLE) && (UART5_REMAP_ENABLE))

#define  UART5_TX_PORT								GPIOC
#define	 UART5_RX_PORT								GPIOD

#define  UART5_TX_PIN								GPIO_Pin_12
#define  UART5_RX_PIN								GPIO_Pin_2

/*
*********************************************************************************************************
*                                       SPI PERIH ENABLE / DISABLE
*********************************************************************************************************
*/
#define STM32FXXX_SPI1_ENABLE						DEF_ENABLED
#define STM32FXXX_SPI2_ENABLE						DEF_DISABLED
#define STM32FXXX_SPI3_ENABLE						DEF_DISABLED

/*
*********************************************************************************************************
*                                     	SPI1 CONFIGURATION
*********************************************************************************************************
*/

#if defined(APP_OS)
#define  SPI1_PERIPH_ID                             BSP_PERIPH_ID_SPI1
#define  SPI1_MOSI_PERIPH_ID						BSP_PERIPH_ID_IOPA
#define  SPI1_MISO_PERIPH_ID						BSP_PERIPH_ID_IOPA
#define  SPI1_SCLK_PERIPH_ID						BSP_PERIPH_ID_IOPA
#else
#if defined(STM32F1XX)
#define  SPI1_PERIPH_ID                             RCC_APB2Periph_SPI1
#define  SPI1_MOSI_PERIPH_ID						RCC_APB2Periph_GPIOA
#define  SPI1_MISO_PERIPH_ID						RCC_APB2Periph_GPIOA
#define  SPI1_SCLK_PERIPH_ID						RCC_APB2Periph_GPIOA
#endif	//#if defined(STM32F1XX)

#if defined(STM32F0XX)
#define  SPI1_PERIPH_ID                             RCC_APB1Periph_SPI2
#define  SPI1_MOSI_PERIPH_ID						RCC_AHBPeriph_GPIOA
#define  SPI1_MISO_PERIPH_ID						RCC_AHBPeriph_GPIOA
#define  SPI1_SCLK_PERIPH_ID						RCC_AHBPeriph_GPIOA
#endif	//#if defined(STM32F0XX)

#endif  //#if defined(APP_OS)

#define  SPI1_MOSI_PORT								GPIOA
#define	 SPI1_MISO_PORT								GPIOA
#define  SPI1_SCLK_PORT								GPIOA

#define  SPI1_MOSI_PIN								GPIO_Pin_7
#define  SPI1_MISO_PIN								GPIO_Pin_6
#define  SPI1_SCLK_PIN								GPIO_Pin_5

#if ((defined(STM32F0XX) || (defined(STM32F4XX))))
#define  SPI1_MOSI_PIN_SOURCE						GPIO_PinSource7
#define  SPI1_MISO_PIN_SOURCE                       GPIO_PinSource6
#define  SPI1_SCLK_PIN_SOURCE                       GPIO_PinSource5

#if (defined(STM32F0XX))
#define  SPI1_MOSI_PIN_AF                           GPIO_AF_0
#define  SPI1_MISO_PIN_AF                           GPIO_AF_0
#define  SPI1_SCLK_PIN_AF                           GPIO_AF_0
#endif	//#if (defined(STM32F0XX))
#endif  //#if ((defined(STM32F0XX) || (defined(STM32F4XX))))

/*
*********************************************************************************************************
*                                     	SPI2 CONFIGURATION
*********************************************************************************************************
*/

#if defined(APP_OS)
#define  SPI2_PERIPH_ID                             BSP_PERIPH_ID_SPI2
#define  SPI2_MOSI_PERIPH_ID						BSP_PERIPH_ID_IOPB
#define  SPI2_MISO_PERIPH_ID						BSP_PERIPH_ID_IOPB
#define  SPI2_SCLK_PERIPH_ID						BSP_PERIPH_ID_IOPB
#else
#define  SPI2_PERIPH_ID                             RCC_APB1Periph_SPI2
#define  SPI2_MOSI_PERIPH_ID						RCC_APB2Periph_GPIOB
#define  SPI2_MISO_PERIPH_ID						RCC_APB2Periph_GPIOB
#define  SPI2_SCLK_PERIPH_ID						RCC_APB2Periph_GPIOB
#endif  //#if defined(APP_OS)

#define  SPI2_MOSI_PORT								GPIOB
#define	 SPI2_MISO_PORT								GPIOB
#define  SPI2_SCLK_PORT								GPIOB

#define  SPI2_MOSI_PIN								GPIO_Pin_15
#define  SPI2_MISO_PIN								GPIO_Pin_14
#define  SPI2_SCLK_PIN								GPIO_Pin_13


/*
*********************************************************************************************************
*                                     	SPI3 CONFIGURATION
*********************************************************************************************************
*/

#if defined(APP_OS)
#define  SPI3_PERIPH_ID                             BSP_PERIPH_ID_SPI3
#define  SPI3_MOSI_PERIPH_ID						BSP_PERIPH_ID_IOPB
#define  SPI3_MISO_PERIPH_ID						BSP_PERIPH_ID_IOPB
#define  SPI3_SCLK_PERIPH_ID						BSP_PERIPH_ID_IOPB
#else
#define  SPI3_PERIPH_ID                             RCC_APB1Periph_SPI3
#define  SPI3_MOSI_PERIPH_ID						RCC_APB2Periph_GPIOB
#define  SPI3_MISO_PERIPH_ID						RCC_APB2Periph_GPIOB
#define  SPI3_SCLK_PERIPH_ID						RCC_APB2Periph_GPIOB
#endif  //#if defined(APP_OS)

#define  SPI3_MOSI_PORT								GPIOB
#define	 SPI3_MISO_PORT								GPIOB
#define  SPI3_SCLK_PORT								GPIOB

#define  SPI3_MOSI_PIN								GPIO_Pin_5
#define  SPI3_MISO_PIN								GPIO_Pin_4
#define  SPI3_SCLK_PIN								GPIO_Pin_3


/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/
#define  CLI_UART									SERIAL_PORT1

extern int App_SerialPrintf(const char *format, ...);

#define  TRACE_LEVEL_OFF                       		(0)
#define  TRACE_LEVEL_INFO                      		(1)
#define  TRACE_LEVEL_DEBUG                     		(2)

#if (defined(CODE_DEBUG))
#define  APP_TRACE_LEVEL                        	TRACE_LEVEL_DEBUG
#endif  //#if (defined(CODE_DEBUG))

#if (defined(CODE_RELEASE))
#define  APP_TRACE_LEVEL                        	TRACE_LEVEL_INFO
#endif  //#if (defined(CODE_RELEASE))

#if (defined(CODE_DEBUG))
#define  APP_TRACE(...)                      		App_SerialPrintf(__VA_ARGS__)
#define  APP_TRACE_INFO(...)            			((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_TRACE(__VA_ARGS__)) : (void)0)
#define  APP_TRACE_DEBUG(...)          				((APP_TRACE_LEVEL >= TRACE_LEVEL_DEBUG) ? (void)(APP_TRACE(__VA_ARGS__)) : (void)0)
#endif  //#if (defined(CODE_DEBUG))

#if (defined(CODE_RELEASE))
#define  APP_TRACE(...)                      		(void)0
#define  APP_TRACE_INFO(...)            			((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_TRACE(__VA_ARGS__)) : (void)0)
#define  APP_TRACE_DEBUG(...)          				((APP_TRACE_LEVEL >= TRACE_LEVEL_DEBUG) ? (void)(APP_TRACE(__VA_ARGS__)) : (void)0)
#endif  //#if (defined(CODE_RELEASE))


/*
*********************************************************************************************************
*                                     RFM64(SX1212) CONFIGURATION
*********************************************************************************************************
*/
#define SX1212_SPI_MONI_ENABLE						DEF_ENABLED

#define SX1212_NSS_DATA_PERIPH_CLK					RCC_AHBPeriph_GPIOB
#define SX1212_NSS_DATA_PORT						GPIOB
#define SX1212_NSS_DATA_PIN							GPIO_Pin_0

#define SX1212_NSS_CFG_PERIPH_CLK					RCC_AHBPeriph_GPIOA
#define SX1212_NSS_CFG_PORT							GPIOA
#define SX1212_NSS_CFG_PIN							GPIO_Pin_7

#define SX1212_IRQ1_PERIPH_CLK						RCC_AHBPeriph_GPIOA
#define SX1212_IRQ1_PORT							GPIOA
#define SX1212_IRQ1_PIN								GPIO_Pin_0

#define SX1212_IRQ0_PERIPH_CLK						RCC_AHBPeriph_GPIOA
#define SX1212_IRQ0_PORT							GPIOA
#define SX1212_IRQ0_PIN								GPIO_Pin_1

#define SX1212_MISO_PERIPH_CLK						RCC_AHBPeriph_GPIOB
#define SX1212_MISO_PORT							GPIOB
#define SX1212_MISO_PIN								GPIO_Pin_10

#define SX1212_MOSI_PERIPH_CLK						RCC_AHBPeriph_GPIOB
#define SX1212_MOSI_PORT							GPIOB
#define SX1212_MOSI_PIN								GPIO_Pin_2

#define SX1212_SCK_PERIPH_CLK						RCC_AHBPeriph_GPIOB
#define SX1212_SCK_PORT								GPIOB
#define SX1212_SCK_PIN								GPIO_Pin_1


//#define SX1212_PLL_LOCK_PERIPH_CLK			    RCC_AHBPeriph_GPIOB			//~~
//#define SX1212_PLL_LOCK_PORT						GPIOB						//~~
//#define SX1212_PLL_LOCK_PIN					    GPIO_Pin_1					//~~

//#define SX1212_RF_DATA_PERIPH_CLK					RCC_AHBPeriph_GPIOB			//~~
//#define SX1212_RF_DATA_PORT					    GPIOB						//~~
//#define SX1212_RF_DATA_PIN					    GPIO_Pin_2					//~~

//#define SX1212_POR_CHK_PERIPH_CLK					RCC_AHBPeriph_GPIOB			//~~
//#define SX1212_POR_CHK_PORT						GPIOB						//~~
//#define SX1212_POR_CHK_PIN						GPIO_Pin_3					//~~





#define SX1212_2_NSS_DATA_PERIPH_CLK					RCC_AHBPeriph_GPIOB
#define SX1212_2_NSS_CFG_PERIPH_CLK					    RCC_AHBPeriph_GPIOA
//#define SX1212_2_PLL_LOCK_PERIPH_CLK					RCC_AHBPeriph_GPIOB			//~~
//#define SX1212_2_RF_DATA_PERIPH_CLK					RCC_AHBPeriph_GPIOB			//~~
#define SX1212_2_IRQ1_PERIPH_CLK						RCC_AHBPeriph_GPIOA
#define SX1212_2_IRQ0_PERIPH_CLK						RCC_AHBPeriph_GPIOA
#define SX1212_2_MISO_PERIPH_CLK						RCC_AHBPeriph_GPIOB
#define SX1212_2_MOSI_PERIPH_CLK						RCC_AHBPeriph_GPIOB
#define SX1212_2_SCK_PERIPH_CLK						    RCC_AHBPeriph_GPIOB
//#define SX1212_2_POR_CHK_PERIPH_CLK					RCC_AHBPeriph_GPIOB			//~~

#define SX1212_2_NSS_DATA_PORT						    GPIOB
#define SX1212_2_NSS_CFG_PORT							GPIOA
//#define SX1212_2_PLL_LOCK_PORT						GPIOB						//~~
//#define SX1212_2_RF_DATA_PORT							GPIOB						//~~
#define SX1212_2_IRQ1_PORT							    GPIOA
#define SX1212_2_IRQ0_PORT							    GPIOA
#define SX1212_2_MISO_PORT							    GPIOB
#define SX1212_2_MOSI_PORT							    GPIOB
#define SX1212_2_SCK_PORT								GPIOB
//#define SX1212_2_POR_CHK_PORT							GPIOB						//~~

#define SX1212_2_NSS_DATA_PIN							GPIO_Pin_1
#define SX1212_2_NSS_CFG_PIN							GPIO_Pin_7
//#define SX1212_2_PLL_LOCK_PIN							GPIO_Pin_1					//~~
//#define SX1212_2_RF_DATA_PIN							GPIO_Pin_2					//~~
#define SX1212_2_IRQ1_PIN								GPIO_Pin_0
#define SX1212_2_IRQ0_PIN								GPIO_Pin_1
#define SX1212_2_MISO_PIN								GPIO_Pin_10
#define SX1212_2_MOSI_PIN								GPIO_Pin_2
#define SX1212_2_SCK_PIN								GPIO_Pin_1
//#define SX1212_POR_CHK_PIN							GPIO_Pin_3					//~~

#if (defined(SX1212_SPI_MONI_ENABLE) && (SX1212_SPI_MONI_ENABLE))

#else
#define  SX1212_SPI_DEV								STM32_SPI1
#define  SX1212_2_SPI_DEV							STM32_SPI2
#endif	//#if (defined(SX1212_SPI_MONI_ENABLE) && (SX1212_SPI_MONI_ENABLE))


/*
*********************************************************************************************************
*                                     EXT PIN CONFIGURATION
*********************************************************************************************************
*/
#define FAN_ROTATION_PERIPH_CLK					    RCC_AHBPeriph_GPIOA
#define FAN_ROTATION_PORT						    GPIOA
#define FAN_ROTATION_PIN						    GPIO_Pin_2

#define GAS_SENSOR_PERIPH_CLK					    RCC_AHBPeriph_GPIOA
#define GAS_SENSOR_PORT						        GPIOA
#define GAS_SENSOR_PIN						        GPIO_Pin_3

/*
*********************************************************************************************************
*                                     KEYBOARD CONFIGURATION
*********************************************************************************************************
*/
#define KEY_BOARD_1PIN_PERIPH_CLK				RCC_AHBPeriph_GPIOA
#define KEY_BOARD_1PIN_PORT						GPIOA
#define KEY_BOARD_1PIN							GPIO_Pin_15

#define KEY_BOARD_2PIN_PERIPH_CLK				RCC_AHBPeriph_GPIOB
#define KEY_BOARD_2PIN_PORT						GPIOB
#define KEY_BOARD_2PIN							GPIO_Pin_3

#define KEY_BOARD_3PIN_PERIPH_CLK				RCC_AHBPeriph_GPIOA
#define KEY_BOARD_3PIN_PORT						GPIOA
#define KEY_BOARD_3PIN							GPIO_Pin_8

#define KEY_BOARD_4PIN_PERIPH_CLK				RCC_AHBPeriph_GPIOB
#define KEY_BOARD_4PIN_PORT						GPIOB
#define KEY_BOARD_4PIN							GPIO_Pin_15

#define KEYBOARD_MAX_CNT					    (5u)


/*
*********************************************************************************************************
*                                     LED 74HC164 CONFIGURATION
*********************************************************************************************************
*/
#define LED_74HC164_A_PERIPH_CLK                RCC_AHBPeriph_GPIOB
#define LED_74HC164_A_PORT                      GPIOB
#define LED_74HC164_A_PIN                       GPIO_Pin_14

#define LED_74HC164_CLR_PERIPH_CLK              RCC_AHBPeriph_GPIOB
#define LED_74HC164_CLR_PORT                    GPIOB
#define LED_74HC164_CLR_PIN                     GPIO_Pin_13

#define LED_74HC164_CLK_PERIPH_CLK              RCC_AHBPeriph_GPIOB
#define LED_74HC164_CLK_PORT                    GPIOB
#define LED_74HC164_CLK_PIN                     GPIO_Pin_12


/*
*********************************************************************************************************
*                                     ESP8266 WIFI USART CONFIGURATION
*********************************************************************************************************
*/
#if (defined(PLC_JZQ_HARDWARE_PARK_V2))
#define	 ESP8266_SERIAL_PORT						SERIAL_PORT5
#define	 ESP8266_SERIAL_PORT_BAUDRATE				SERIAL_BAUDRATE_115200

#if (defined(APP_OS))
#define  ESP8266_POWER_CTRL_PERIPH_ID				BSP_PERIPH_ID_IOPB
#else
#define  ESP8266_POWER_CTRL_PERIPH_ID				RCC_APB2Periph_GPIOB
#endif	//#if (defined(APP_OS))
	
#define  ESP8266_POWER_CTRL_PORT					GPIOB
#define  ESP8266_POWER_CTRL_PIN						GPIO_Pin_7
#endif	//#if (defined(PLC_JZQ_HARDWARE_PARK_V2))



#if (defined(PLC_JZQ_HARDWARE_PARK_V3))
#define	 ESP8266_SERIAL_PORT						SERIAL_PORT1
#define	 ESP8266_SERIAL_PORT_BAUDRATE				SERIAL_BAUDRATE_115200

#if (defined(APP_OS))
#define  ESP8266_POWER_CTRL_PERIPH_ID				BSP_PERIPH_ID_IOPA
#else
#define  ESP8266_POWER_CTRL_PERIPH_ID				RCC_APB2Periph_GPIOA
#endif	//#if (defined(APP_OS))
	
#define  ESP8266_POWER_CTRL_PORT					GPIOA
#define  ESP8266_POWER_CTRL_PIN						GPIO_Pin_8
#endif	//#if (defined(PLC_JZQ_HARDWARE_PARK_V3))



//#define  SIMCOM_SMS_PDU_INIT						0
//#define  SIMCOM_CONNECT_COUNT						4
//#define  SIMCOM_SEND_UNIT_COUNT						4
//#define  SIMCOM_SEND_BUF_SIZE						1024
//#define  SIMCOM_RECEIVE_BUF_SIZE					1200

#define  ESP8266_LOCK_DELAY							(10000u)		//AT串口等待时间
#define  ESP8266_AT_SIZE             				(64u)			//AT命令缓存大小
#define  ESP8266_AT_RX_RSP_SIZE						(256u)			//AT命令返回命令缓存大小
#define  ESP8266_RSP_SIZE							(600u)


/*
*********************************************************************************************************
*                                     PLC BOTTOM CONFIG CONFIGURATION
*********************************************************************************************************
*/
#if (defined(PLC_JZQ_HARDWARE_GWDLX))
#define  PLC_RSK_SERIAL_PORT                        SERIAL_PORT1

#define  PLC_RSK_RST_PORT                           GPIOB
#define  PLC_RSK_RST_PIN                            GPIO_Pin_1
#define  PLC_RSK_RST_PERIPH_ID                      BSP_PERIPH_ID_IOPB

#define  PLC_RSK_STA_PORT                           GPIOB
#define  PLC_RSK_STA_PIN                            GPIO_Pin_0
#define  PLC_RSK_STA_PERIPH_ID                      BSP_PERIPH_ID_IOPB
#endif	//#if (defined(PLC_JZQ_HARDWARE_GWDLX))

#if (defined(PLC_JZQ_HARDWARE_KG7000A))
#define  PLC_RSK_SERIAL_PORT                        SERIAL_PORT1

#define  PLC_RSK_RST_PORT                           GPIOA
#define  PLC_RSK_RST_PIN                            GPIO_Pin_8
#define  PLC_RSK_RST_PERIPH_ID                      BSP_PERIPH_ID_IOPA

#define  PLC_RSK_STA_PORT                           GPIOA
#define  PLC_RSK_STA_PIN                            GPIO_Pin_11
#define  PLC_RSK_STA_PERIPH_ID                      BSP_PERIPH_ID_IOPA
#endif	//#if (defined(PLC_JZQ_HARDWARE_KG7000A))

#if (defined(PLC_JZQ_HARDWARE_PARK_V2))
#define  PLC_ZIGBEE_SERIAL_PORT                     SERIAL_PORT1

#define  PLC_ZIGBEE_RST_PORT                        GPIOC
#define  PLC_ZIGBEE_RST_PIN                         GPIO_Pin_8
#define  PLC_ZIGBEE_RST_PERIPH_ID                   BSP_PERIPH_ID_IOPC

#if (defined(PLC_JZQ_HARDWARE_PARK_OLD))
#define  PLC_ZIGBEE_PWR_CTRL_PORT                   GPIOA
#define  PLC_ZIGBEE_PWR_CTRL_PIN                    GPIO_Pin_11
#define  PLC_ZIGBEE_PWR_CTRL_PERIPH_ID              BSP_PERIPH_ID_IOPA
#else
#define  PLC_ZIGBEE_PWR_CTRL_PORT                   GPIOC						//新版zigbee排针2Km模块，电源控制引脚配置PC6
#define  PLC_ZIGBEE_PWR_CTRL_PIN                    GPIO_Pin_6
#define  PLC_ZIGBEE_PWR_CTRL_PERIPH_ID              BSP_PERIPH_ID_IOPC
#endif	//#if (defined(PLC_JZQ_HARDWARE_PARK_OLD))
#endif  //#if (defined(PLC_JZQ_HARDWARE_PARK_V2))


#if (defined(PLC_JZQ_HARDWARE_PARK_V3))
#define  PLC_ZIGBEE_SERIAL_PORT                     SERIAL_PORT5

#define  PLC_ZIGBEE_RST_PORT                        GPIOB
#define  PLC_ZIGBEE_RST_PIN                         GPIO_Pin_7
#define  PLC_ZIGBEE_RST_PERIPH_ID                   BSP_PERIPH_ID_IOPB

#define  PLC_ZIGBEE_PWR_CTRL_PORT                   GPIOB
#define  PLC_ZIGBEE_PWR_CTRL_PIN                    GPIO_Pin_6
#define  PLC_ZIGBEE_PWR_CTRL_PERIPH_ID              BSP_PERIPH_ID_IOPB
#endif  //#if (defined(PLC_JZQ_HARDWARE_PARK_V3))



#if (defined(PLC_JZQ_DEBUG))
#define  PLC_RSK_MTASK_OUPUT                        DEF_ENABLED
#endif  //#if (defined(PLC_JZQ_DEBUG))

#if (defined(PLC_JZQ_APP))
#define  PLC_RSK_MTASK_OUPUT                        DEF_DISABLED
#endif  //#if (defined(PLC_JZQ_APP))


/*
*********************************************************************************************************
*                                     I2C SIMULATION CONFIGURATION
*********************************************************************************************************
*/
#if (defined(PLC_JZQ_HARDWARE_GWDLX))
#if (defined(APP_OS))
#define I2C_SIMULATION_SCL_PERIPH_ID				BSP_PERIPH_ID_IOPB
#define I2C_SIMULATION_SDA_PERIPH_ID				BSP_PERIPH_ID_IOPB
#else
#define I2C_SIMULATION_SCL_PERIPH_ID				RCC_APB2Periph_GPIOB
#define I2C_SIMULATION_SDA_PERIPH_ID				RCC_APB2Periph_GPIOB
#endif	//#if (defined(APP_OS))

#define I2C_SIMULATION_SCL_PORT						GPIOB
#define I2C_SIMULATION_SCL_PIN						GPIO_Pin_8

#define I2C_SIMULATION_SDA_PORT						GPIOB
#define I2C_SIMULATION_SDA_PIN						GPIO_Pin_9
#endif	//#if (defined(PLC_JZQ_HARDWARE_GWDLX))

#if (defined(PLC_JZQ_HARDWARE_KG7000A))
#if (defined(APP_OS))
#define I2C_SIMULATION_SCL_PERIPH_ID				BSP_PERIPH_ID_IOPB
#define I2C_SIMULATION_SDA_PERIPH_ID				BSP_PERIPH_ID_IOPB
#else
#define I2C_SIMULATION_SCL_PERIPH_ID				RCC_APB2Periph_GPIOB
#define I2C_SIMULATION_SDA_PERIPH_ID				RCC_APB2Periph_GPIOB
#endif	//#if (defined(APP_OS))

#define I2C_SIMULATION_SCL_PORT						GPIOB
#define I2C_SIMULATION_SCL_PIN						GPIO_Pin_8

#define I2C_SIMULATION_SDA_PORT						GPIOB
#define I2C_SIMULATION_SDA_PIN						GPIO_Pin_9
#endif	//#if (defined(PLC_JZQ_HARDWARE_KG7000A))


#if (defined(PLC_JZQ_HARDWARE_PARK))
#if (defined(PLC_JZQ_HARDWARE_PARK_V2))
#if (defined(APP_OS))
#define I2C_SIMULATION_SCL_PERIPH_ID				BSP_PERIPH_ID_IOPC
#define I2C_SIMULATION_SDA_PERIPH_ID				BSP_PERIPH_ID_IOPC
#else
#define I2C_SIMULATION_SCL_PERIPH_ID				RCC_APB2Periph_GPIOC
#define I2C_SIMULATION_SDA_PERIPH_ID				RCC_APB2Periph_GPIOC
#endif	//#if (defined(APP_OS))

#define I2C_SIMULATION_SCL_PORT						GPIOC
#define I2C_SIMULATION_SCL_PIN						GPIO_Pin_2

#define I2C_SIMULATION_SDA_PORT						GPIOC
#define I2C_SIMULATION_SDA_PIN						GPIO_Pin_3


#elif (defined(PLC_JZQ_HARDWARE_PARK_V3))
#if (defined(APP_OS))
#define I2C_SIMULATION_SCL_PERIPH_ID				BSP_PERIPH_ID_IOPE
#define I2C_SIMULATION_SDA_PERIPH_ID				BSP_PERIPH_ID_IOPB
#else
#define I2C_SIMULATION_SCL_PERIPH_ID				RCC_APB2Periph_GPIOE
#define I2C_SIMULATION_SDA_PERIPH_ID				RCC_APB2Periph_GPIOB
#endif	//#if (defined(APP_OS))

#define I2C_SIMULATION_SCL_PORT						GPIOE
#define I2C_SIMULATION_SCL_PIN						GPIO_Pin_15

#define I2C_SIMULATION_SDA_PORT						GPIOB
#define I2C_SIMULATION_SDA_PIN						GPIO_Pin_10

#else

#if (defined(APP_OS))
#define I2C_SIMULATION_SCL_PERIPH_ID				BSP_PERIPH_ID_IOPB
#define I2C_SIMULATION_SDA_PERIPH_ID				BSP_PERIPH_ID_IOPC
#else
#define I2C_SIMULATION_SCL_PERIPH_ID				RCC_APB2Periph_GPIOB
#define I2C_SIMULATION_SDA_PERIPH_ID				RCC_APB2Periph_GPIOC
#endif	//#if (defined(APP_OS))

#define I2C_SIMULATION_SCL_PORT						GPIOB
#define I2C_SIMULATION_SCL_PIN						GPIO_Pin_13

#define I2C_SIMULATION_SDA_PORT						GPIOC
#define I2C_SIMULATION_SDA_PIN						GPIO_Pin_8
#endif	//#if (defined(PLC_JZQ_HARDWARE_PARK_V2))
#endif  //#if (defined(PLC_JZQ_HARDWARE_PARK))


/*
*********************************************************************************************************
*                                     RX8010 GPIO CONFIGURATION
*********************************************************************************************************
*/
#if (defined(APP_OS))
#define RX8010_I2C_IRQ_PERIPH_ID					BSP_PERIPH_ID_IOPC
#else
#define RX8010_I2C_IRQ_PERIPH_ID					RCC_APB2Periph_GPIOC
#endif	//#if (defined(APP_OS))

#define RX8010_I2C_IRQ_PORT							GPIOC
#define RX8010_I2C_IRQ_PIN							GPIO_Pin_13


/*
*********************************************************************************************************
*                                     LED SEG GPIO CONFIGURATION
*********************************************************************************************************
*/
//#if (defined(APP_OS))
//#define LED_74HC595_DAT_PERIPH_ID					BSP_PERIPH_ID_IOPA
//#define LED_74HC595_SCLK_PERIPH_ID					BSP_PERIPH_ID_IOPA
//#define LED_74HC595_SRCK_PERIPH_ID					BSP_PERIPH_ID_IOPC
//#define LED_74HC595_CS_PERIPH_ID					BSP_PERIPH_ID_IOPA

//#define LED_SEG_1_COM_PERIPH_ID						BSP_PERIPH_ID_IOPB
//#define LED_SEG_2_COM_PERIPH_ID						BSP_PERIPH_ID_IOPB
//#define LED_SEG_3_COM_PERIPH_ID						BSP_PERIPH_ID_IOPB
//#define LED_SEG_4_COM_PERIPH_ID						BSP_PERIPH_ID_IOPB
//#else					
//#define LED_74HC595_DAT_PERIPH_ID					RCC_AHBPeriph_GPIOA
//#define LED_74HC595_SCLK_PERIPH_ID					RCC_AHBPeriph_GPIOA
//#define LED_74HC595_SRCK_PERIPH_ID					RCC_AHBPeriph_GPIOC
//#define LED_74HC595_CS_PERIPH_ID					RCC_AHBPeriph_GPIOA

//#define LED_SEG_1_COM_PERIPH_ID						RCC_AHBPeriph_GPIOB
//#define LED_SEG_2_COM_PERIPH_ID						RCC_AHBPeriph_GPIOB
//#define LED_SEG_3_COM_PERIPH_ID						RCC_AHBPeriph_GPIOB
//#define LED_SEG_4_COM_PERIPH_ID						RCC_AHBPeriph_GPIOB
//#endif	//#if (defined(APP_OS))


//#define LED_74HC595_DAT_PORT					    GPIOA
//#define LED_74HC595_SCLK_PORT					    GPIOA
//#define LED_74HC595_SRCK_PORT					    GPIOC
//#define LED_74HC595_CS_PORT					        GPIOA

//#define LED_SEG_1_COM_PORT							GPIOB
//#define LED_SEG_2_COM_PORT							GPIOB
//#define LED_SEG_3_COM_PORT							GPIOB
//#define LED_SEG_4_COM_PORT							GPIOB


//#define LED_74HC595_DAT_PIN					        GPIO_Pin_11
//#define LED_74HC595_SCLK_PIN					    GPIO_Pin_11
//#define LED_74HC595_SRCK_PIN					    GPIO_Pin_13
//#define LED_74HC595_CS_PIN					        GPIO_Pin_11

//#define LED_SEG_1_COM_PIN							GPIO_Pin_4 //GPIO_Pin_5
//#define LED_SEG_2_COM_PIN							GPIO_Pin_5 //GPIO_Pin_6
//#define LED_SEG_3_COM_PIN							GPIO_Pin_6 //GPIO_Pin_7
//#define LED_SEG_4_COM_PIN							GPIO_Pin_7 //GPIO_Pin_4


/*
*********************************************************************************************************
*                                     BUZZER CONFIGURATION
*********************************************************************************************************
*/
#define BUZZER_PWR_CTRL_PERIPH_ID                   RCC_AHBPeriph_GPIOB
#define BUZZER_CTRL_PERIPH_ID                       RCC_AHBPeriph_GPIOB

#define BUZZER_PWR_CTRL_PORT                        GPIOB
#define BUZZER_CTRL_PORT                            GPIOB

#define BUZZER_PWR_CTRL_PIN                         GPIO_Pin_9
#define BUZZER_CTRL_PIN                             GPIO_Pin_8


/*
*********************************************************************************************************
*                                     PLC HFP2014 CONFIGURATION
*********************************************************************************************************
*/
#if (defined(PLC_JZQ_HARDWARE_GWDLX))
#define  PLC_HFP2014_SERIAL_PORT					SERIAL_PORT2
#define  PLC_HFP2014_SERIAL_BAUDRATE				SERIAL_BAUDRATE_57600
#endif	//#if (defined(PLC_JZQ_HARDWARE_GWDLX))

#if (defined(PLC_JZQ_HARDWARE_KG7000A))
#define  PLC_HFP2014_SERIAL_PORT					SERIAL_PORT2
//#define  PLC_HFP2014_SERIAL_PORT					SERIAL_PORT5
#define  PLC_HFP2014_SERIAL_BAUDRATE				SERIAL_BAUDRATE_57600
#endif	//#if (defined(PLC_JZQ_HARDWARE_KG7000A))


/*
*********************************************************************************************************
*                                     W5500 CONFIGURATION
*********************************************************************************************************
*/
#if (defined(APP_OS))
#define W5500_CS_PERIPH_ID							BSP_PERIPH_ID_IOPB
#define W5500_RST_PERIPH_ID							BSP_PERIPH_ID_IOPD
#else
#define W5500_CS_PERIPH_ID							RCC_APB2Periph_GPIOB
#define W5500_RST_PERIPH_ID							RCC_APB2Periph_GPIOD
#endif	//#if (defined(APP_OS))

#define W5500_CS_PORT								GPIOB
#define W5500_CS_PIN								GPIO_Pin_12

#define W5500_RST_PORT								GPIOD
#define W5500_RST_PIN								GPIO_Pin_10

#define W5500_IRQ_EXTI_PORT							eEXTI_PORTD
#define W5500_IRQ_EXTI_PIN							EXTI_PIN13
#define W5500_IRQ_EXTI_TRIGGER						eEXTI_FALLING

#define W5500_SPI_DEV								STM32_SPI2

//
//#if (defined(APP_OS))
//#define W5500_CS_PERIPH_ID							BSP_PERIPH_ID_IOPB
//#define W5500_RST_PERIPH_ID							BSP_PERIPH_ID_IOPB
//#else
//#define W5500_CS_PERIPH_ID							RCC_APB2Periph_GPIOB
//#define W5500_RST_PERIPH_ID							RCC_APB2Periph_GPIOB
//#endif	//#if (defined(APP_OS))
//
//#define W5500_CS_PORT								GPIOB
//#define W5500_CS_PIN								GPIO_Pin_8
//
//#define W5500_RST_PORT								GPIOB
//#define W5500_RST_PIN								GPIO_Pin_6
//
//#define W5500_IRQ_EXTI_PORT							eEXTI_PORTB
//#define W5500_IRQ_EXTI_PIN							EXTI_PIN9
//#define W5500_IRQ_EXTI_TRIGGER						eEXTI_FALLING
//
//#define W5500_SPI_DEV								STM32_SPI3

/*
*********************************************************************************************************
*                                  COMMUNICATION MODULE CONFIG
*********************************************************************************************************
*/
#if ((defined(PLC_JZQ_HARDWARE_GWDLX)) || (defined(PLC_JZQ_HARDWARE_KG7000A)))
#define PLC_BOTTOM_PRTOCOL_RISECOMM					DEF_ENABLED
#endif	//#if ((defined(PLC_JZQ_HARDWARE_GWDLX)) || (defined(PLC_JZQ_HARDWARE_KG7000A)))

#if (defined(PLC_JZQ_HARDWARE_PARK))
#define PLC_BOTTOM_PRTOCOL_ZIGBEE					DEF_ENABLED
#endif	//#if (defined(PLC_JZQ_HARDWARE_PARK))

#if (defined(PLC_BOTTOM_PRTOCOL_RISECOMM) && (PLC_BOTTOM_PRTOCOL_RISECOMM))
#define PLC_JZQ_TERMINAL_TYPE						((char *)"PLCRsk_NodePlan_U")					//终端型号
#if (defined(PLC_JZQ_HARDWARE_GWDLX))
#define PLC_JZQ_HARDWARE_VER						((char *)"GWDLX-JZQ V12")						//硬件版本
#define PLC_JZQ_UPDATE_FILE_NAME                    "PLCRsk_NodePlan_U_KG5000.bin"					//集中器文件更新名称
#elif (defined(PLC_JZQ_HARDWARE_KG7000A))
#define PLC_JZQ_HARDWARE_VER						((char *)"KG70000A-3ACP V51")					//硬件版本
#define PLC_JZQ_UPDATE_FILE_NAME                    "PLCRsk_NodePlan_U_KG7000.bin"					//集中器文件更新名称
#else
#error "No define Hardware Version."
#endif	//#if (defined(PLC_JZQ_HARDWARE_GWDLX))


#define PLC_JZQ_SOFTWARE_VER						((char *)"PLCJZQ_Ver20170112")					//程序版本
#define PLC_JZQ_COM_MODULE_TYPE						((char *)"Risecomm")							//通讯模块
#endif	//#if (defined(PLC_BOTTOM_PRTOCOL_RISECOMM) && (PLC_BOTTOM_PRTOCOL_RISECOMM))


#if (defined(PLC_BOTTOM_PRTOCOL_ZIGBEE) && (PLC_BOTTOM_PRTOCOL_ZIGBEE))
#define PLC_JZQ_TERMINAL_TYPE						((char *)"PLCZigbee_NodePlan_U")				//终端型号
#if (defined(PLC_JZQ_HARDWARE_PARK_V2))
#define PLC_JZQ_HARDWARE_VER						((char *)"HF-TingChe Ver12")					//硬件版本
#elif (defined(PLC_JZQ_HARDWARE_PARK_V3))
#define PLC_JZQ_HARDWARE_VER						((char *)"HF-TingCheBoard-2")					//硬件版本
#else
#define PLC_JZQ_HARDWARE_VER						((char *)"HF-TingChe Ver11")					//硬件版本
#endif	//#if (defined(PLC_JZQ_HARDWARE_PARK_V2))

#define PLC_JZQ_SOFTWARE_VER						((char *)"PLCJZQ_Ver20170112")					//程序版本
#define PLC_JZQ_COM_MODULE_TYPE						((char *)"Zigbee")								//通讯模块

#if (defined(PLC_JZQ_HARDWARE_PARK))
#if (defined(PLC_JZQ_HARDWARE_PARK_V2))
#define PLC_JZQ_UPDATE_FILE_NAME                    "PLCZigbee_NodePlan_U_Park_V2.bin"				//集中器文件更新名称
#elif (defined(PLC_JZQ_HARDWARE_PARK_V3))
#define PLC_JZQ_UPDATE_FILE_NAME                    "PLCZigbee_NodePlan_U_Park_V3.bin"				//集中器文件更新名称
#else
#define PLC_JZQ_UPDATE_FILE_NAME                    "PLCZigbee_NodePlan_U_Park_V1.bin"				//集中器文件更新名称
#endif	//#if (defined(PLC_JZQ_HARDWARE_PARK_V2))
#endif	//#if (defined(PLC_JZQ_HARDWARE_PARK))
#endif	//#if (defined(PLC_BOTTOM_PRTOCOL_ZIGBEE) && (PLC_BOTTOM_PRTOCOL_ZIGBEE))

/*
*********************************************************************************************************
*                                     EEPROM CONFIGURATION
*********************************************************************************************************
*/
#define EEPROM_PAGE_SIZE_BY_WORD					64
#define EEPROM_PAGE_SIZE_BY_BYTE					256

#if (defined(PLC_JZQ_HARDWARE_PARK_V2))
#define	EEPROM_PAGE_COUNT							65536
#else
#define	EEPROM_PAGE_COUNT							4096
#endif	//#if (defined(PLC_JZQ_HARDWARE_PARK_V2))
#define EEPROM_DEBUG_PAGE_COUNT						0x3F

#define EEPROM_START_ADDR		                    0
#define EEPROM_VERIFY_COUNT                         3

#define EEPROM_LOAD_SIZE		                    (0x28000u)     //160K bytes	    //断点下载数据空间
#define EEPROM_LOAD_CODE_INFO_SIZE					(0x800u)       //2k的下载信息空间
#define EEPROM_LOAD_BLOCK_CHECK_SIZE				(0x2800u)      //10k块校验空间  

#define EEPROM_BRP_LOAD_SIZE		                (0x28000)      //160K bytes	    //断点下载数据空间
#define EEPROM_NODE_LOAD_SIZE						(0x14000)       //80K bytes

#define EEPROM_MULTI_PACK_RX_SIZE		            (0x14000)       //80K bytes     //这个缓存可以减小，可使用断点下载
#define EEPROM_MULTI_PACK_TX_SIZE		            (0x14000)       //80K bytes

#define EEPROM_NODE_MULTI_PACK_RX_SIZE				(10240U)        //10K bytes
#define EEPROM_NODE_MULTI_PACK_TX_SIZE				(10240U)        //10K bytes

#define EEPROM_NODE_DATA_RAM_STORAGE				(0xD000)		//52K bytes		//节点数据堆整理时使用

/*
*********************************************************************************************************
*                                     PLC RSK DEBUG CONFIGURATION
*********************************************************************************************************
*/
#if (defined(PLC_JZQ_DEBUG))
#if (defined(DEBUG_BOARD) && (DEBUG_BOARD))
#define PLC_PROTOCOL_DEBUG                          DEF_DISABLED
#else
#define PLC_PROTOCOL_DEBUG                          DEF_ENABLED
#endif  //#if (defined(DEBUG_BOARD) && (DEBUG_BOARD))
#endif  //#if (defined(PLC_JZQ_DEBUG))

//底层协议数据收发显示
#if (defined(PLC_JZQ_DEBUG))
#define PLC_PROTOCOL_BOTTOM_DATA_DEBUG				DEF_ENABLED
#endif	//#if (defined(PLC_JZQ_DEBUG))

#if (defined(PLC_JZQ_APP))
#define PLC_PROTOCOL_BOTTOM_DATA_DEBUG				DEF_DISABLED
#endif	//#if (defined(PLC_JZQ_APP))

/*
*********************************************************************************************************
*                                     PLC HF PROTOCOL DEBUG CONFIGURATION
*********************************************************************************************************
*/
#if (defined(PLC_JZQ_DEBUG))
#define PLC_HFP2014_PROTOCOL_DEBUG					DEF_ENABLED
#endif  //#if (defined(PLC_JZQ_DEBUG))

#if (defined(PLC_JZQ_APP))
#define PLC_HFP2014_PROTOCOL_DEBUG					DEF_DISABLED
#endif  //#if (defined(PLC_JZQ_APP))


/*
*********************************************************************************************************
*                                     PLC NODE AUTO FIND CONFIGURATION
*********************************************************************************************************
*/
#if (defined(PLC_JZQ_DEBUG))
#define PLC_NODE_AUTO_FIND_ENABLE                   DEF_ENABLED
#endif  //#if (defined(PLC_JZQ_DEBUG))

#if (defined(PLC_JZQ_APP))
#define PLC_NODE_AUTO_FIND_ENABLE                   DEF_ENABLED
#endif  //#if (defined(PLC_JZQ_APP))


/*
*********************************************************************************************************
*                                     PLC FUN MASK OUTPUT DEBUG CONFIGURATION
*********************************************************************************************************
*/
#if (defined(PLC_JZQ_DEBUG))
#define PLC_FUN_SERVICE_MASK_OUTPUT					DEF_DISABLED
#endif  //#if (defined(PLC_JZQ_DEBUG))

#if (defined(PLC_JZQ_APP))
#define PLC_FUN_SERVICE_MASK_OUTPUT					DEF_DISABLED
#endif  //#if (defined(PLC_JZQ_APP))


///*
//*********************************************************************************************************
//*                                     PLC FUN SERVICE DEBUG CONFIGURATION
//*********************************************************************************************************
//*/
//#if (defined(PLC_JZQ_DEBUG))
//#define PLC_FUN_SERVICE_DEBUG						DEF_ENABLED
//#endif  //#if (defined(PLC_JZQ_DEBUG))

//#if (defined(PLC_JZQ_APP))
//#define PLC_FUN_SERVICE_DEBUG						DEF_DISABLED
//#endif  //#if (defined(PLC_JZQ_APP))


/*
*********************************************************************************************************
*                                     PLC FUN SERVICE DEBUG CONFIGURATION
*********************************************************************************************************
*/
#if (defined(PLC_JZQ_DEBUG))
#define PLC_PROTOCOL_USER_SEND_DEBUG				DEF_DISABLED
#endif  //#if (defined(PLC_JZQ_DEBUG))

#if (defined(PLC_JZQ_APP))
#define PLC_PROTOCOL_USER_SEND_DEBUG                DEF_DISABLED
#endif  //#if (defined(PLC_JZQ_APP))



/*
*********************************************************************************************************
*                                     PLC FUN SERVICE DEBUG CONFIGURATION
*********************************************************************************************************
*/
#if (defined(PLC_JZQ_DEBUG))
#define PLC_FUN_UPDATE_FLAG_DEBUG					DEF_ENABLED
#endif  //#if (defined(PLC_JZQ_DEBUG))

#if (defined(PLC_JZQ_APP))
#define PLC_FUN_UPDATE_FLAG_DEBUG					DEF_DISABLED
#endif  //#if (defined(PLC_JZQ_APP))


/*
*********************************************************************************************************
*                                     uCOS RUN TIME DEBUG CONFIGURATION
*********************************************************************************************************
*/
#if (defined(OS_TASK_RUN_TIME_DEBUG) && (OS_TASK_RUN_TIME_DEBUG))
typedef struct _TASK_USER_DATA
{
	unsigned int TaskCtr;
	unsigned int TaskExecTime;
	unsigned int TaskTotExecTime;
}TASK_USER_DATA;
#endif	//#if (defined(OS_TASK_RUN_TIME_DEBUG) && (OS_TASK_RUN_TIME_DEBUG))


/*
*********************************************************************************************************
*                                     WIFI PARA CONFIGURATION
*********************************************************************************************************
*/
#if (defined(PLC_JZQ_HARDWARE_PARK))
#define PLC_JZQ_DEFAULT_WIFI_SSID					"TPLINK_CDHF209"
#define PLC_JZQ_DEFAULT_WIFI_PASSWORD				"Tp.1234567890"

#define PLC_JZQ_DEFAULT_CONNECT_IP					"120.27.142.74"
#define PLC_JZQ_DEFAULT_CONNECT_PORT				(10000u)
#define PLC_JZQ_DEFAULT_CONNECT_TYPE				ESP8266_CTYPE_UDP


#define PLC_JZQ_DEFAULT_AP_ADDR						"192.168.4.1"				//默认ap模式下的ip
#define PLC_JZQ_DEFAULT_AP_SSID						"PLCJZQ_WIFI"				//AP模式下默认ssid
#define PLC_JZQ_DEFAULT_AP_PASSWORD					"PLCJZQ_CDHF123"			//AP模式下默认password

#define PLC_JZQ_DEFAULT_TCP_SERVER_PORT				(12000)						//设置TCP服务器的端口
#endif	//#if (defined(PLC_JZQ_HARDWARE_PARK))


/*
*********************************************************************************************************
*                                     BOTTOM ADDR CONFIGURATION
*********************************************************************************************************
*/
#define PLC_BOTTOM_ADDR_CFG_FRONT					DEF_DISABLED				//底层通讯模块地址: ENABLE前面补零, DISABLE后面补零

/*
*********************************************************************************************************
*                                     PLC OUTPUT ACTIVE UPLOAD CONFIGURATION
*********************************************************************************************************
*/
#if (defined(PLC_JZQ_HARDWARE_PARK))
#define PLC_OUTPUT_DATA_ACTIVE_UPLOAD_DISABLE		DEF_DISABLED				//plc输出状态后，数据变化节点主动上传
#endif	//#if (defined(PLC_JZQ_HARDWARE_PARK))									//DEF_DISABLED节点不主动上传，DEF_ENABLED节点主动上传数据

#if ((defined(PLC_JZQ_HARDWARE_GWDLX)) || (defined(PLC_JZQ_HARDWARE_KG7000A)))
#define PLC_OUTPUT_DATA_ACTIVE_UPLOAD_DISABLE		DEF_ENABLED					//plc输出状态后，数据变化节点主动上传
#endif	//#if ((defined(PLC_JZQ_HARDWARE_GWDLX)) || (defined(PLC_JZQ_HARDWARE_KG7000A)))


/*
*********************************************************************************************************
*                                     LOG CONFIGURATION
*********************************************************************************************************
*/
//#define PLC_LOG_START_ADDR							(0x100000u)					//日志记录起始地址
//#define PLC_LOG_MAX_SIZE							(0xF00000u)					//日志记录总空间大小

#define PLC_LOG_START_ADDR							(0xF0000u)					//日志记录起始地址960K开始


/*
*********************************************************************************************************
*                                     PARA CFG CH CONFIGURATION
*********************************************************************************************************
*/
#define PLC_PARA_CFG_CLI_COM					((uint8)0x00)				//当前串口正在设置参数
#define PLC_PARA_CFG_WIFI_AP					((uint8)0x01)				//当前正通过wifi的ap模式设置参数
#define PLC_PARA_CFG_UNKNOW						((uint8)0x02)				//未知

#endif
