/************************************��Ȩ����(c)****************************************
* ��Ȩ����(c), 2012-2013, XXXXXXXX��˾, ��������Ȩ
* �ļ���      ��Cooker_Parse.h
* ����        ��XXXXXX
* ��������    ��XXXXXX
* �汾        ��V1.0.0.0
* ����        ��2017-11-14
* ��������    ��
* ��ע        ��
* ��Ҫ�����б�
* �޸ļ�¼    ��
  ��      ��          �汾          �޸���          �޸�����
  2017-11-14          1.0           XXXXXXXX        �����ļ�	
****************************************************************************************/

/* ��ֹͷ�ļ��ظ����� -----------------------------------------------------------------*/
#ifndef __COOKER_PARSE_H__
#define __COOKER_PARSE_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* ���Բ��ֿ��� -----------------------------------------------------------------------*/

/* �ⲿ���� ---------------------------------------------------------------------------*/
#ifdef    COOKER_PARSE_MODULE
#define   COOKER_PARSE_EXT
#else
#define   COOKER_PARSE_EXT  extern
#endif

/* �����ļ� ---------------------------------------------------------------------------*/
#include "NewType.h"

/* �궨�� -----------------------------------------------------------------------------*/
#define COOKER_PARSE_HEADER				'C'
#define COOKER_PARSE_ADDR_LEN			12
#define COOKER_PARSE_PAYLOAD_LEN		30

#define COOKER_PARSE_TRUE				(0xAA)
#define COOKER_PARSE_FALSE				(0x55)

/* �ṹ���Ͷ��� -----------------------------------------------------------------------*/
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

/* ԭ���������� -----------------------------------------------------------------------*/
extern unsigned int Cooker_Load(Cooker_Parse_t *entity);
extern void Cooker_Parse(int c);


#ifdef __cplusplus
}
#endif
#endif /* __COOKER_PARSE_H__ */
/*******************************��Ȩ����(c)**************END OF FILE********************/
