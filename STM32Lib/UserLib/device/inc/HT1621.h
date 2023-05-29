/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXX公司, 保留所有权
* 文件名      ：HT1621.h
* 作者        ：XXXXXX
* 电子邮箱    ：XXXXXX
* 版本        ：V1.0.0.0
* 日期        ：2018-09-15
* 功能描述    ：
* 备注        ：
* 主要函数列表：
* 修改记录    ：
  日      期          版本          修改人          修改内容
  2018-09-15          1.0           XXXXXXXX        创建文件	
****************************************************************************************/

/* 防止头文件重复包含 -----------------------------------------------------------------*/
#ifndef __HT1621_H__
#define __HT1621_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* 调试部分开关 -----------------------------------------------------------------------*/

/* 外部定义 ---------------------------------------------------------------------------*/
#ifdef    HT1621_MODULE
#define   HT1621_EXT
#else
#define   HT1621_EXT  extern
#endif

/* 包含文件 ---------------------------------------------------------------------------*/
#include "NewType.h"

/* 宏定义 -----------------------------------------------------------------------------*/
//LED展示GAS泄露
#define LED_DIS_GAS_LEAK                        0x10    //0x01
#define LED_DIS_GAS_FIRE                        0x20    //0x02
#define LED_DIS_FAN_ROTATION                    0x40    //0x04
#define LED_DIS_BATTERY                         0x80    //0x08
#define LED_DIS_GASCHECK                        0x0100 //

#define LED_DIS_SET_RTC                         0x01    //0x10
#define LED_DIS_SET_START_TIME                  0x02    //0x20
#define LED_DIS_SET_END_TIME                    0x04    //0x40
#define LED_DIS_SET_SLAVE_CODE                  0x08    //0x80



#define LED_SEG_BLINK_TIME						(1000u)

#define LED_SEG_BLINK_DOT						(0x10)				//闪烁中间两点
#define LED_SEG_BLINK_DOT_AND_ONE				(0x11)				//闪烁中间两点和第一位

/* 结构类型定义 -----------------------------------------------------------------------*/

/* 原型声明部分 -----------------------------------------------------------------------*/
void HT1621_Init (void);

void HT1621_WriteCmd(uint8 cmd);
void HT1621_WriteData(uint8 addr, const uint8 *const buffer, uint16 length);

void HT1621_SignService(void);

void LED_SEG_Update(uint8 *pVaulue);
void LED_SEGBlinkSet(uint8 value);

void Led_DisplayOn(uint16 value);
void Led_DisplayOff(uint16 value);

#ifdef __cplusplus
}
#endif
#endif /* __HT1621_H__ */
/*******************************版权所有(c)**************END OF FILE********************/
