/************************************��Ȩ����(c)****************************************
* ��Ȩ����(c), 2012-2013, XXXXXXXX��˾, ��������Ȩ
* �ļ���      ��HT1621.h
* ����        ��XXXXXX
* ��������    ��XXXXXX
* �汾        ��V1.0.0.0
* ����        ��2018-09-15
* ��������    ��
* ��ע        ��
* ��Ҫ�����б�
* �޸ļ�¼    ��
  ��      ��          �汾          �޸���          �޸�����
  2018-09-15          1.0           XXXXXXXX        �����ļ�	
****************************************************************************************/

/* ��ֹͷ�ļ��ظ����� -----------------------------------------------------------------*/
#ifndef __HT1621_H__
#define __HT1621_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* ���Բ��ֿ��� -----------------------------------------------------------------------*/

/* �ⲿ���� ---------------------------------------------------------------------------*/
#ifdef    HT1621_MODULE
#define   HT1621_EXT
#else
#define   HT1621_EXT  extern
#endif

/* �����ļ� ---------------------------------------------------------------------------*/
#include "NewType.h"

/* �궨�� -----------------------------------------------------------------------------*/
//LEDչʾGASй¶
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

#define LED_SEG_BLINK_DOT						(0x10)				//��˸�м�����
#define LED_SEG_BLINK_DOT_AND_ONE				(0x11)				//��˸�м�����͵�һλ

/* �ṹ���Ͷ��� -----------------------------------------------------------------------*/

/* ԭ���������� -----------------------------------------------------------------------*/
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
/*******************************��Ȩ����(c)**************END OF FILE********************/
