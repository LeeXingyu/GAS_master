/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXX公司, 保留所有权
* 文件名      ：Cooker_Parse.c
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

/* 调试部分开关 -----------------------------------------------------------------------*/

/* 包含文件 ---------------------------------------------------------------------------*/
#include "Cooker_Parse.h"
#include "utilities.h"
#include "STM32Fxxx_Misc.h"

#include <string.h>
#include "SX1212.h"

/* 宏定义 -----------------------------------------------------------------------------*/
#define Cooker_TimeGet()			BSP_OS_TimeGet()

/* 结构类型定义 -----------------------------------------------------------------------*/
typedef enum _Cooker_State_e
{
	eCOOKER_PARSE_NOP,
	eCOOKER_PARSE_HEADER,
	eCOOKER_PARSE_ADDR,
	eCOOKER_PARSE_LENGTH,
	eCOOKER_PARSE_CMD,
	eCOOKER_PARSE_PAYLOAD,
	eCOOKER_PARSE_CHECK,
}Cooker_State_e;

/* 原型声明部分 -----------------------------------------------------------------------*/
static void CookerParse_Nop(int c);
static void CookerParse_Header(int c);
static void CookerParse_Addr(int c);
static void CookerParse_Length(int c);
static void CookerParse_Cmd(int c);
static void CookerParse_Payload(int c);
static void CookerParse_Check(int c);

extern void Cooker_AFNChk(Cooker_Parse_t *entity);
extern void Cooker_GetId(unsigned char *id);
extern void Cooker_WirelessSendLoad(char *load, unsigned int len);

/* 全局变量声明 -----------------------------------------------------------------------*/

/* 静态变量定义 -----------------------------------------------------------------------*/
static Cooker_Parse_t				tCooker_Entity;
static Cooker_State_e				eCooker_State		= eCOOKER_PARSE_NOP;
static unsigned int					usCooker_ParseLength= 0;
static unsigned int					usCooker_ParseCheck = CRC_INITIAL;

static void (*const a_pfnCooker_Parse[])(int c) =
{
	CookerParse_Nop,
	CookerParse_Header,
	CookerParse_Addr,
	CookerParse_Length,
	CookerParse_Cmd,
	CookerParse_Payload,
	CookerParse_Check,
};

/****************************************************************************************
* 函数名称：CookerParse_Nop()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-11-14 23:44
****************************************************************************************/
static void CookerParse_Nop(int c)
{

}

/****************************************************************************************
* 函数名称：CookerParse_Header()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-11-14 23:44
****************************************************************************************/
static void CookerParse_Header(int c)
{
	if ((unsigned char)COOKER_PARSE_HEADER == (unsigned char)c)
		eCooker_State = eCOOKER_PARSE_ADDR;
	else
		eCooker_State = eCOOKER_PARSE_NOP;
}

/****************************************************************************************
* 函数名称：CookerParse_Addr()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-11-14 23:44
****************************************************************************************/
static void CookerParse_Addr(int c)
{
	tCooker_Entity.addr[usCooker_ParseLength++] = c;

	if (usCooker_ParseLength >= COOKER_PARSE_ADDR_LEN)
	{
		usCooker_ParseLength = 0;

		eCooker_State = eCOOKER_PARSE_LENGTH;
	}
}

/****************************************************************************************
* 函数名称：
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-11-15 0:14
****************************************************************************************/
static void CookerParse_Length(int c)
{
	tCooker_Entity.length	= c;
	eCooker_State			= eCOOKER_PARSE_CMD;
}

/****************************************************************************************
* 函数名称：CookerParse_Cmd()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-11-14 23:44
****************************************************************************************/
static void CookerParse_Cmd(int c)
{
	tCooker_Entity.cmd	= c;
	
    if (tCooker_Entity.length)
        eCooker_State   = eCOOKER_PARSE_PAYLOAD;
    else
        eCooker_State   = eCOOKER_PARSE_CHECK;
}


/****************************************************************************************
* 函数名称：CookerParse_Payload()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-11-14 23:44
****************************************************************************************/
static void CookerParse_Payload(int c)
{
	tCooker_Entity.payload[usCooker_ParseLength++] = c;

	if ((usCooker_ParseLength >= tCooker_Entity.length) || (usCooker_ParseLength >= COOKER_PARSE_PAYLOAD_LEN))
	{
		usCooker_ParseLength = 0;

		eCooker_State =  eCOOKER_PARSE_CHECK;
	}
}


/****************************************************************************************
* 函数名称：CookerParse_Check()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-11-14 23:44
****************************************************************************************/
static void CookerParse_Check(int c)
{
	if (++usCooker_ParseLength >= 2)
	{
		usCooker_ParseLength = 0;

		if (CHECK_SUM == usCooker_ParseCheck)
		{
			unsigned char id[COOKER_PARSE_ADDR_LEN];

			Cooker_GetId(id);

			if (cmp_buf((char *)id, (char *)tCooker_Entity.addr, COOKER_PARSE_ADDR_LEN))
				Cooker_AFNChk(&tCooker_Entity);
		}
	}
}

/****************************************************************************************
* 函数名称：Cooker_Parse()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-11-14 23:44
****************************************************************************************/
void Cooker_Parse(int c)
{
	static unsigned int delay;

	if (eCooker_State > eCOOKER_PARSE_CHECK)
		eCooker_State = eCOOKER_PARSE_NOP;

	if ((eCOOKER_PARSE_NOP != eCooker_State) && ((Cooker_TimeGet() - delay) > 500))
		eCooker_State = eCOOKER_PARSE_NOP;

	if (eCOOKER_PARSE_NOP == eCooker_State)
	{
		memset((char *)&tCooker_Entity, 0, sizeof(Cooker_Parse_t));

		usCooker_ParseCheck = CRC_INITIAL;
		usCooker_ParseLength= 0;

		eCooker_State = eCOOKER_PARSE_HEADER;
	}

	delay = Cooker_TimeGet();
	usCooker_ParseCheck = crc16_ccitt_byte(usCooker_ParseCheck, c);

	a_pfnCooker_Parse[eCooker_State](c);
}

#include "app_cfg.h"
/****************************************************************************************
* 函数名称：Cooker_Load()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-11-15 0:37
****************************************************************************************/
unsigned int Cooker_Load(Cooker_Parse_t *entity)
{
	unsigned int check = CRC_INITIAL;
	unsigned int index = 0, count;
	unsigned char id[COOKER_PARSE_ADDR_LEN];
	char load[60];

	load[index++] = COOKER_PARSE_HEADER;
	check = crc16_ccitt_byte(check, COOKER_PARSE_HEADER);

	Cooker_GetId(id);
	for (count = 0; count < COOKER_PARSE_ADDR_LEN; count++)
	{
		load[index++] = id[count];
		check = crc16_ccitt_byte(check, id[count]);
	}
	
	load[index++] = entity->length;
	check = crc16_ccitt_byte(check, entity->length);

	load[index++] = entity->cmd;
	check = crc16_ccitt_byte(check, entity->cmd);

	for (count = 0; count < entity->length; count++)
	{
		load[index++] = entity->payload[count];
		check = crc16_ccitt_byte(check, entity->payload[count]);
	}

	check = ~check;
	load[index++] = (unsigned char)check;
	load[index++] = (unsigned char)(check >> 8);

	//这里添加发送数据函数
	Cooker_WirelessSendLoad(load, index);

	return index;
}

__weak void Cooker_GetId(unsigned char *id)
{
	ETAG_TypeDef etag;

	STM32Fxxx_GetEtag(&etag);

	memcpy((char *)id, (char *)etag.id, COOKER_PARSE_ADDR_LEN);
}

__weak void Cooker_AFNChk(Cooker_Parse_t *entity)
{

}

__weak void Cooker_WirelessSendLoad(char *load, unsigned int len)
{
	if (BuildPacket((unsigned char *)load, len))
		SendRfFrame((unsigned char *)(&RF_Pkt), sizeof(RF_Pkt));
    
    {
        unsigned int count;
        
        APP_TRACE("发送到数据：");
        for (count = 0; count < RF_Pkt.key; count++)
            APP_TRACE("%02X ", (unsigned int)RF_Pkt.fill[count]);
        APP_TRACE("\r\n");
    }
}


/*******************************版权所有(c)**************END OF FILE********************/
