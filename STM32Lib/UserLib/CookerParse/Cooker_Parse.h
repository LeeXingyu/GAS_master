/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXX公司, 保留所有权
* 文件名      ：Cooker_Parse.h
* 作者        ：XXXXXX
* 电子邮箱    ：XXXXXX
* 版本        ：V1.0.0.0
* 日期        ：2017-11-14
* 功能描述    ：
* 备注        ：
* 主要函数列表：
* 修改记录    ：
  日      期          版本          修改人          修改内容
  2017-11-14          1.0           XXXXXXXX        创建文件	
****************************************************************************************/

/* 防止头文件重复包含 -----------------------------------------------------------------*/
#ifndef __COOKER_PARSE_H__
#define __COOKER_PARSE_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* 调试部分开关 -----------------------------------------------------------------------*/

/* 外部定义 ---------------------------------------------------------------------------*/
#ifdef    COOKER_PARSE_MODULE
#define   COOKER_PARSE_EXT
#else
#define   COOKER_PARSE_EXT  extern
#endif

/* 包含文件 ---------------------------------------------------------------------------*/
#include "NewType.h"

/* 宏定义 -----------------------------------------------------------------------------*/
#define COOKER_PARSE_HEADER				'C'
#define COOKER_PARSE_ADDR_LEN			12
#define COOKER_PARSE_PAYLOAD_LEN		30

#define COOKER_PARSE_TRUE				(0xAA)
#define COOKER_PARSE_FALSE				(0x55)

/* 结构类型定义 -----------------------------------------------------------------------*/
typedef struct _Cooker_Parse_t
{
	unsigned char addr[COOKER_PARSE_ADDR_LEN];
	unsigned char length;
	unsigned char cmd;
	unsigned char payload[COOKER_PARSE_PAYLOAD_LEN];
}Cooker_Parse_t;

typedef enum _Cooker_Cmd_e
{
	eCOOKER_SET_SYS_ID = 0,
	eCOOKER_FIRE_STATE = 1,
	eCOOKER_CTRL_Gas  = 2,
	eCOOKER_STATE_Gas = 3,
	eCOOKER_STATE_Bat = 4,
	//eCOOKER_RSP = 0xEE,
}Cooker_Cmd_e;

/* 原型声明部分 -----------------------------------------------------------------------*/
extern unsigned int Cooker_Load(Cooker_Parse_t *entity);
extern void Cooker_Parse(int c);


#ifdef __cplusplus
}
#endif
#endif /* __COOKER_PARSE_H__ */
/*******************************版权所有(c)**************END OF FILE********************/
