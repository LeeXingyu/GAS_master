/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXX公司, 保留所有权
* 文件名      ：KeyBoard.h
* 作者        ：XXXXXX
* 电子邮箱    ：XXXXXX
* 版本        ：V1.0.0.0
* 日期        ：2017-10-14
* 功能描述    ：
* 备注        ：
* 主要函数列表：
* 修改记录    ：
  日      期          版本          修改人          修改内容
  2017-10-14          1.0           XXXXXXXX        创建文件	
****************************************************************************************/

/* 防止头文件重复包含 -----------------------------------------------------------------*/
#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* 调试部分开关 -----------------------------------------------------------------------*/

/* 外部定义 ---------------------------------------------------------------------------*/
#ifdef    KEYBOARD_MODULE
#define   KEYBOARD_EXT
#else
#define   KEYBOARD_EXT  extern
#endif

/* 包含文件 ---------------------------------------------------------------------------*/
#include "NewType.h"

/* 宏定义 -----------------------------------------------------------------------------*/
#define KEYBOARD_DEBOUNCE_DLY					(200u)			//消抖时间10ms
#define KEYBOARD_WAIT_RELEASE_TIME				(500u)			//等待释放时间

//按键定义
#define KEYBOARD_KEY_HOUR						(0x00)			//小时设置
#define KEYBOARD_KEY_TEN_MINUTE					(0x01)			//分钟10位设置
#define KEYBOARD_KEY_MINUTE						(0x04)			//分钟个位设置
#define KEYBOARD_KEY_CANCLE						(0x03)			//取消按键

#define KEYBOARD_KEY_HOUR_AND_CANCLE			(0x00)			//小时和取消同时按下

/* 结构类型定义 -----------------------------------------------------------------------*/

/* 原型声明部分 -----------------------------------------------------------------------*/
KEYBOARD_EXT void KeyBoard_Init (void);
KEYBOARD_EXT void KeyBoard_Service(void);
KEYBOARD_EXT void KeyBoard_FunReg(uint8 key, void (*pfnKey)(void));


#ifdef __cplusplus
}
#endif
#endif /* __KEYBOARD_H__ */
/*******************************版权所有(c)**************END OF FILE********************/
