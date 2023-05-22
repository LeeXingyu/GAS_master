/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXXXXXX, 保留所有权
* 文件名      ：STM32Fxxx_Serial.h
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

/* 防止头文件重复包含 -----------------------------------------------------------------*/
#ifndef __STM32FXXX_SERIAL_H__
#define __STM32FXXX_SERIAL_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* 调试部分开关 -----------------------------------------------------------------------*/

/* 外部定义 ---------------------------------------------------------------------------*/
#ifdef    STM32FXXX_SERIAL_MODULE
#define   STM32FXXX_SERIAL_EXT
#else
#define   STM32FXXX_SERIAL_EXT  extern
#endif

/* 包含文件 ---------------------------------------------------------------------------*/
#include "NewTYpe.h"

/* 宏定义 -----------------------------------------------------------------------------*/

/* 结构类型定义 -----------------------------------------------------------------------*/
typedef enum
{
	SERIAL_PORT1 = 0,
	SERIAL_PORT2,
	SERIAL_PORT3,
	SERIAL_PORT4,
#if defined(STM32F1XX)
	SERIAL_PORT5,
#elif defined(STM32F4XX)
    SERIAL_PORT5,
	SERIAL_PORT6,
#endif //:~STM32F4XX
	SERIAL_PORT_MAX
}SERIAL_Port;
//#define IS_SERIAL_PORT(PORT)		(((PORT) >= SERIAL_PORT1) && ((SERIAL_PORT5) <= 5))

typedef enum
{
	SERIAL_BAUDRATE_1200 = 0,
	SERIAL_BAUDRATE_2400,
	SERIAL_BAUDRATE_4800,
	SERIAL_BAUDRATE_9600,
	SERIAL_BAUDRATE_19200,
	SERIAL_BAUDRATE_38400,
	SERIAL_BAUDRATE_57600,
	SERIAL_BAUDRATE_115200,
	SERIAL_BAUDRATE_MAX
}SERIAL_BaudRate;
//#define IS_SERIAL_BAUDRATE(BAUDRATE) (((BAUDRATE) >= SERIAL_BAUDRATE_1200) && ((BAUDRATE) <= SERIAL_BAUDRATE_115200))

typedef enum
{
	SERIAL_PARITY_NONE = 0,
	SERIAL_PARITY_ODD,
	SERIAL_PARITY_EVEN,
	SERIAL_PARITY_MAX
}SERIAL_Parity;
//#define IS_SERIAL_PARITY(PARITY) (((PARITY) >= SERIAL_PARITY_NONE) && ((PARITY) <= SERIAL_PARITY_EVEN))

typedef enum
{
	SERIAL_BYTE_BITS_8 = 0,
	SERIAL_BYTE_BITS_9,
	SERIAL_BYTE_BITS_MAX
}SERIAL_ByteBits;
//#define IS_SERIAL_WORDLENGTH(WORDLENGTH) (((WORDLENGTH) == SERIAL_DATA_8) || ((WORDLENGTH) == SERIAL_DATA_9))

typedef enum
{
	SERIAL_STOP_BITS_1 = 0,
#ifdef STM32F0XX
    
#else
	SERIAL_STOP_BITS_0_5,
#endif  //#ifdef STM32F0XX
	SERIAL_STOP_BITS_2,
	SERIAL_STOP_BITS_1_5,
	SERIAL_STOP_MAX
}SERIAL_StopBits;

typedef void (*STM32Fxxx_SerRxPtr)(int c);

/* 原型声明部分 -----------------------------------------------------------------------*/
STM32FXXX_SERIAL_EXT BOOL STM32Fxxx_SerialInit(SERIAL_Port port, SERIAL_BaudRate baud, SERIAL_Parity parity,
						  SERIAL_ByteBits bits, SERIAL_StopBits stop, STM32Fxxx_SerRxPtr handler);

#if (defined(APP_OS))
STM32FXXX_SERIAL_EXT BOOL STM32Fxxx_SerialLock(SERIAL_Port port, uint32 dly_ms);
STM32FXXX_SERIAL_EXT BOOL STM32Fxxx_SerialUnlock(SERIAL_Port port);
#endif  //#if (defined(APP_OS))

STM32FXXX_SERIAL_EXT BOOL STM32Fxxx_SerialWriteByte(SERIAL_Port port, char byte);
STM32FXXX_SERIAL_EXT BOOL STM32Fxxx_SerialWriteBlock(SERIAL_Port port, char *payload, int length);;

STM32FXXX_SERIAL_EXT void STM32Fxxx_SerialISREnable(SERIAL_Port port);
STM32FXXX_SERIAL_EXT void STM32Fxxx_SerialISRDisable(SERIAL_Port port);


#ifdef __cplusplus
}
#endif
#endif /* __STM32FXXX_SERIAL_H__ */
/*******************************版权所有(c)**************END OF FILE********************/
