/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXXXXXXX, 保留所有权
* 文件名      ：Package_SX1212.h
* 作者        ：
* 电子邮箱    ：
* 版本        ：V1.0.0.0
* 日期        ：2017-03-26
* 功能描述    ：
* 备注        ：
* 主要函数列表：
* 修改记录    ：
  日      期          版本          修改人          修改内容
  2017-03-26          1.0           XXXXXXXX        创建文件	
****************************************************************************************/

/* 防止头文件重复包含 -----------------------------------------------------------------*/
#ifndef __PACKAGE_SX1212_H__
#define __PACKAGE_SX1212_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* 调试部分开关 -----------------------------------------------------------------------*/

/* 外部定义 ---------------------------------------------------------------------------*/
#ifdef    PACKAGE_SX1212_MODULE
#define   PACKAGE_SX1212_EXT
#else
#define   PACKAGE_SX1212_EXT  extern
#endif

/* 包含文件 ---------------------------------------------------------------------------*/
#include "NewType.h"

/* 宏定义 -----------------------------------------------------------------------------*/

/* 结构类型定义 -----------------------------------------------------------------------*/

/* 原型声明部分 -----------------------------------------------------------------------*/
PACKAGE_SX1212_EXT void Package_SX1212_Init (void);

PACKAGE_SX1212_EXT void Package_SX1212_Service(void);


#ifdef __cplusplus
}
#endif
#endif /* __PACKAGE_SX1212_H__ */
/*******************************版权所有(c)**************END OF FILE********************/
