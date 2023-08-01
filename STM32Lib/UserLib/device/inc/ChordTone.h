/************************************��Ȩ����(c)****************************************
* ��Ȩ����(c), 2012-2013, XXXXXXXX��˾, ��������Ȩ
* �ļ���      ��ChordTone.h
* ����        ��XXXXXX
* ��������    ��XXXXXX
* �汾        ��V1.0.0.0
* ����        ��2017-10-07
* ��������    ��
* ��ע        ��
* ��Ҫ�����б�
* �޸ļ�¼    ��
  ��      ��          �汾          �޸���          �޸�����
  2017-10-07          1.0           XXXXXXXX        �����ļ�	
****************************************************************************************/

/* ��ֹͷ�ļ��ظ����� -----------------------------------------------------------------*/
#ifndef __CHORDTONE_H__
#define __CHORDTONE_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* ���Բ��ֿ��� -----------------------------------------------------------------------*/

/* �ⲿ���� ---------------------------------------------------------------------------*/
#ifdef    CHORDTONE_MODULE
#define   CHORDTONE_EXT
#else
#define   CHORDTONE_EXT  extern
#endif

/* �����ļ� ---------------------------------------------------------------------------*/
#include "NewType.h"

/* �궨�� -----------------------------------------------------------------------------*/

/* �ṹ���Ͷ��� -----------------------------------------------------------------------*/
typedef struct _ChordTone_t					//�����ṹ��
{
	uint32 uiFreq;							//Ƶ��
	uint32 uiOSCTime;						//�񵴳���ʱ��
	uint32 uiPWRTime;						//�������ʱ��
}ChordTone_t;

typedef enum _ChordTone_Music_e
{
	eCHORD_TONE_MUSIC_1_SINGLE = 0,
    eCHORD_TONE_MUSIC_2_SINGLE,
    eCHORD_TONE_MUSIC_3_SINGLE,
    eCHORD_TONE_MUSIC_4_SINGLE,
    eCHORD_TONE_MUSIC_5_SINGLE,
    eCHORD_TONE_MUSIC_6_SINGLE,
    eCHORD_TONE_MUSIC_7_SINGLE,
    eCHORD_TONE_MUSIC_8_SINGLE,
    eCHORD_TONE_MUSIC_9_SINGLE,
    eCHORD_TONE_MUSIC_10_SINGLE,
    eCHORD_TONE_MUSIC_11_SINGLE,
    eCHORD_TONE_MUSIC_LONG_SINGLE,
    eCHORD_TONE_MUSIC_OFF,
	eCHORD_TONE_MUSIC_PWR_ON,
	eCHORD_TONE_MUSIC_PWR_OFF,
	eCHORD_TONE_MUSIC_TWO_TIGER,
	eCHORD_TONE_MUSIC_SONG1,
	eCHORD_TONE_MUSIC_SONG2,
	eCHORD_TONE_MUSIC_SONG3,
	eCHORD_TONE_MUSIC_SONG4,
}ChordTone_Music_e;

/* ԭ���������� -----------------------------------------------------------------------*/
CHORDTONE_EXT void ChordTone_CtrlService(void);
CHORDTONE_EXT void ChordTone_Start(ChordTone_Music_e eMusic);
CHORDTONE_EXT void ChordTone_Init(void);
CHORDTONE_EXT void ChordTone_GasClosevoice(ChordTone_Music_e eMusic);  //�����ر�������������
CHORDTONE_EXT void ChordTone_PWMDevInit(void);
CHORDTONE_EXT void ChordTone_PWMEnable(uint32 freq);
CHORDTONE_EXT void ChordTone_PWMDisable(void);
CHORDTONE_EXT void Buzzer_GPIOInit(void);
CHORDTONE_EXT void Buzzer_PwrOn(void);
CHORDTONE_EXT void Buzzer_PwrOff(void);


#ifdef __cplusplus
}
#endif
#endif /* __CHORDTONE_H__ */
/*******************************��Ȩ����(c)**************END OF FILE********************/
