/************************************???????(c)****************************************
* ???????(c), 2012-2013, XXXXXXXX???, ?????????
* ?????      ??Cooker_Parse.c
* ????        ??XXXXXX
* ????????    ??XXXXXX
* ?·Ú        ??V1.0.0.0
* ????        ??2017-11-14
* ????????    ??
* ???        ??
* ????????§Ò???
* ?????    ??
  ??      ??          ?·Ú          ?????          ???????
  2017-11-14          1.0           XXXXXXXX        ???????	
****************************************************************************************/

/* ?????????? -----------------------------------------------------------------------*/

/* ??????? ---------------------------------------------------------------------------*/
#include "Cooker_Parse.h"
#include "utilities.h"
#include "STM32Fxxx_Misc.h"

#include <string.h>
#include "SX1212.h"

/* ???? -----------------------------------------------------------------------------*/
#define Cooker_TimeGet()			BSP_OS_TimeGet()

/* ????????? -----------------------------------------------------------------------*/
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

/* ??????????? -----------------------------------------------------------------------*/
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

/* ?????????? -----------------------------------------------------------------------*/

/* ??????????? -----------------------------------------------------------------------*/
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
* ?????????CookerParse_Nop()
* ????????: 
* ????????
* ?????  ??
* ????    ??
* ???????2017-11-14 23:44
****************************************************************************************/
static void CookerParse_Nop(int c)
{

}

/****************************************************************************************
* ?????????CookerParse_Header()
* ????????: 
* ????????
* ?????  ??
* ????    ??
* ???????2017-11-14 23:44
****************************************************************************************/
static void CookerParse_Header(int c)
{
	if ((unsigned char)COOKER_PARSE_HEADER == (unsigned char)c)
		eCooker_State = eCOOKER_PARSE_ADDR;
	else
		eCooker_State = eCOOKER_PARSE_NOP;
}

/****************************************************************************************
* ?????????CookerParse_Addr()
* ????????: 
* ????????
* ?????  ??
* ????    ??
* ???????2017-11-14 23:44
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
* ?????????
* ????????: 
* ????????
* ?????  ??
* ????    ??
* ???????2017-11-15 0:14
****************************************************************************************/
static void CookerParse_Length(int c)
{
	tCooker_Entity.length	= c;
	eCooker_State			= eCOOKER_PARSE_CMD;
}

/****************************************************************************************
* ?????????CookerParse_Cmd()
* ????????: 
* ????????
* ?????  ??
* ????    ??
* ???????2017-11-14 23:44
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
* ?????????CookerParse_Payload()
* ????????: 
* ????????
* ?????  ??
* ????    ??
* ???????2017-11-14 23:44
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
* ?????????CookerParse_Check()
* ????????: 
* ????????
* ?????  ??
* ????    ??
* ???????2017-11-14 23:44
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
			else Cooker_AFNChk(&tCooker_Entity);
		}
	}
}

/****************************************************************************************
* ?????????Cooker_Parse()
* ????????: 
* ????????
* ?????  ??
* ????    ??
* ???????2017-11-14 23:44
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
* ?????????Cooker_Load()
* ????????: 
* ????????
* ?????  ??
* ????    ??
* ???????2017-11-15 0:37
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

	//??????????????????
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

void Cooker_WirelessSendLoad(char *load, unsigned int len)
{
	if (BuildPacket((unsigned char *)load, len))
		SendRfFrame((unsigned char *)(&RF_Pkt), sizeof(RF_Pkt));
    
//    {
//        unsigned int count;
//        
//        APP_TRACE("??????????");
//        for (count = 0; count < RF_Pkt.key; count++)
//            APP_TRACE("%02X ", (unsigned int)RF_Pkt.fill[count]);
//        APP_TRACE("\r\n");
//    }
}


/*******************************???????(c)**************END OF FILE********************/
