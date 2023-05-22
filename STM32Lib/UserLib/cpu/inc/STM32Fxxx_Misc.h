/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXXXXXXX, 保留所有权
* 文件名      ：STM32Fxxx_Misc.h
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

/* 防止头文件重复包含 -----------------------------------------------------------------*/
#ifndef __STM32FXXX_MISC_H__
#define __STM32FXXX_MISC_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* 调试部分开关 -----------------------------------------------------------------------*/

/* 外部定义 ---------------------------------------------------------------------------*/
#ifdef    STM32FXXX_MISC_MODULE
#define   STM32FXXX_MISC_EXT
#else
#define   STM32FXXX_MISC_EXT  extern
#endif

/* 包含文件 ---------------------------------------------------------------------------*/
#include "NewType.h"

/* 宏定义 -----------------------------------------------------------------------------*/
#define FACTOR_1_S									1000

#define TIMER_100_MS								100
#define TIMER_200_MS								200
#define TIMER_400_MS								400
#define TIMER_500_MS								500
#define TIMER_700_MS								700
#define TIMER_800_MS								800
#define TIMER_1_S									1000
#define TIMER_1_5_S									1500
#define TIMER_2_S									2000
#define TIMER_3_S									3000
#define TIMER_4_S									4000
#define TIMER_5_S									5000
#define TIMER_6_S									6000
#define TIMER_7_S									7000
#define TIMER_8_S									8000
#define TIMER_10_S									10000
#define TIMER_12_S									12000
#define TIMER_15_S									15000
#define TIMER_20_S									20000
#define TIMER_30_S									30000
#define TIMER_1_M									60000
#define TIMER_2_M									120000
#define TIMER_3_M									180000
#define TIMER_5_M       							300000

#define MCU_ETAG_LENGTH								12

#if defined(STM32F4XX)
#define ADDR_FLASH_SECTOR_0     					((uint32_t)0x08000000) //Base @ of Sector 0, 16 Kbytes
#define ADDR_FLASH_SECTOR_1     					((uint32_t)0x08004000) //Base @ of Sector 1, 16 Kbytes
#define ADDR_FLASH_SECTOR_2     					((uint32_t)0x08008000) //Base @ of Sector 2, 16 Kbytes
#define ADDR_FLASH_SECTOR_3    					 	((uint32_t)0x0800C000) //Base @ of Sector 3, 16 Kbytes
#define ADDR_FLASH_SECTOR_4     					((uint32_t)0x08010000) //Base @ of Sector 4, 64 Kbytes
#define ADDR_FLASH_SECTOR_5     					((uint32_t)0x08020000) //Base @ of Sector 5, 128 Kbytes
#define ADDR_FLASH_SECTOR_6     					((uint32_t)0x08040000) //Base @ of Sector 6, 128 Kbytes
#define ADDR_FLASH_SECTOR_7     					((uint32_t)0x08060000) //Base @ of Sector 7, 128 Kbytes
#define ADDR_FLASH_SECTOR_8     					((uint32_t)0x08080000) //Base @ of Sector 8, 128 Kbytes
#define ADDR_FLASH_SECTOR_9     					((uint32_t)0x080A0000) //Base @ of Sector 9, 128 Kbytes
#define ADDR_FLASH_SECTOR_10    					((uint32_t)0x080C0000) //Base @ of Sector 10, 128 Kbytes
#define ADDR_FLASH_SECTOR_11    					((uint32_t)0x080E0000) //Base @ of Sector 11, 128 Kbytes
#define ADDR_FLASH_SECTOR_END						((uint32_t)0x08100000) //
#endif //:~STM32F4XX

/* 结构类型定义 -----------------------------------------------------------------------*/
typedef struct
{
	uint16		flash_size;
	uint16		page_size;
	char 		id[MCU_ETAG_LENGTH];
}ETAG_TypeDef;

typedef enum _MCU_RstType_e
{
//    eMCU_PIN_RST,
    eMCU_POR_RST,
    eMCU_SFT_RST,
    eMCU_IWDG_RST,
    eMCU_WWDG_RST,
    eMCU_LPWR_RST,
    eMCU_NO_RST,
}MCU_RstType_e;

/* 原型声明部分 -----------------------------------------------------------------------*/
STM32FXXX_MISC_EXT void 	STM32Fxxx_GetEtag(ETAG_TypeDef *Info);

STM32FXXX_MISC_EXT void	    STM32Fxxx_CountDelayus(uint16 us);
STM32FXXX_MISC_EXT void	    STM32Fxxx_AppDescription(void);

STM32FXXX_MISC_EXT void     STM32Fxxx_FlashInit(void);
STM32FXXX_MISC_EXT BOOL 	STM32Fxxx_FlashErase(uint32 addr, uint32 len);
STM32FXXX_MISC_EXT BOOL 	STM32Fxxx_FlashWrite(uint8 *data, uint32 addr, uint32 len);
STM32FXXX_MISC_EXT BOOL 	STM32Fxxx_FlashRead(uint8 *data, uint32 addr, uint32 len);
STM32FXXX_MISC_EXT BOOL 	STM32F4XX_FlashGetSector(uint32 Address, uint32 *sector);
STM32FXXX_MISC_EXT BOOL     STM32Fxx_FlashPageWrite(uint8 *data, uint32 addr, uint32 len);

#if (!defined(APP_OS))
STM32FXXX_MISC_EXT void     SysTick_HandleInit(void);
STM32FXXX_MISC_EXT void     SysTick_FunReg(void (*pfun)(void));
STM32FXXX_MISC_EXT void 	STM32Fxxx_TimeIRQHandler(void);
STM32FXXX_MISC_EXT uint32 	BSP_OS_TimeGet(void);
STM32FXXX_MISC_EXT BOOL 	BSP_OS_Timeout(uint32 Start, uint32 Delay);
STM32FXXX_MISC_EXT void 	BSP_OS_TimeDlyMs(uint32 dly_ms);
#endif //:~APP_OS


STM32FXXX_MISC_EXT void     STM32Fxxx_JumpToAppCode(uint32 app_addr);
STM32FXXX_MISC_EXT void     STM32Fxxx_SysReset(void);
STM32FXXX_MISC_EXT void     STM32Fxxx_AppVectorTabInit(uint32 offest);
STM32FXXX_MISC_EXT MCU_RstType_e STM32Fxxx_RstType(void);


#ifdef __cplusplus
}
#endif
#endif /* __STM32FXXX_MISC_H__ */
/*******************************版权所有(c)**************END OF FILE********************/
