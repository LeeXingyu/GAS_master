/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXX公司, 保留所有权
* 文件名      ：ChordTone.h
* 作者        ：XXXXXX
* 电子邮箱    ：XXXXXX
* 版本        ：V1.0.0.0
* 日期        ：2017-10-07
* 功能描述    ：
* 备注        ：
* 主要函数列表：
* 修改记录    ：
  日      期          版本          修改人          修改内容
  2017-10-07          1.0           XXXXXXXX        创建文件	
****************************************************************************************/

/* 防止头文件重复包含 -----------------------------------------------------------------*/
#ifndef __CHORDTONE_H__
#define __CHORDTONE_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* 调试部分开关 -----------------------------------------------------------------------*/

/* 外部定义 ---------------------------------------------------------------------------*/
#ifdef    CHORDTONE_MODULE
#define   CHORDTONE_EXT
#else
#define   CHORDTONE_EXT  extern
#endif

/* 包含文件 ---------------------------------------------------------------------------*/
#include "NewType.h"

/* 宏定义 -----------------------------------------------------------------------------*/

/* 结构类型定义 -----------------------------------------------------------------------*/
typedef struct _ChordTone_t					//音调结构体
{
	uint32 uiFreq;							//频率
	uint32 uiOSCTime;						//振荡持续时间
	uint32 uiPWRTime;						//供电持续时间
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

/* 原型声明部分 -----------------------------------------------------------------------*/
CHORDTONE_EXT void ChordTone_CtrlService(void);
CHORDTONE_EXT void ChordTone_Start(ChordTone_Music_e eMusic);
CHORDTONE_EXT void ChordTone_Init(void);
CHORDTONE_EXT void ChordTone_GasClosevoice(ChordTone_Music_e eMusic);  //气阀关闭语音启动函数
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
/*******************************版权所有(c)**************END OF FILE********************/
