/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXX公司, 保留所有权
* 文件名      ：ChordTone.c
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

/* 调试部分开关 -----------------------------------------------------------------------*/

/* 包含文件 ---------------------------------------------------------------------------*/
#include "ChordTone.h"
#include "STM32Fxxx_Misc.h"

#include <string.h>

/* 宏定义 -----------------------------------------------------------------------------*/
#define BUZZER_STATE_START				(0x00)
#define BUZZER_STATE_RUN				(0x01)
#define BUZZER_STATE_IDLE				(0x02)


#define BUZZER_SINGLE_FREQ              2700
#define BUZZER_SINGLE_OS_TIME           50
#define BUZZER_SINGLE_PWR_TIME          70

#define BUZZER_SINGLE_OFF_OS_TIME       100
#define BUZZER_SINGLE_OFF_PWR_TIME      100

#define BUZZER_SINGLE_OFF_FREQ          10


#define BUZZER_LONG_SINGLE_FREQ         2700  //这里是报警音配置，尽量只修改这个参数（BUZZER_LONG_SINGLE_FREQ），只能修改后面在数字
#define BUZZER_LONG_SINGLE_LAST_FREQ    3400
#define BUZZER_LONG_SINGLE_OS_TIME      26
#define BUZZER_LONG_SINGLE_PWR_TIME     60
#define BUZZER_LONG_SINGLE_LAST_OS_TIME      28
#define BUZZER_LONG_SINGLE_LAST_PWR_TIME     50

#define BUZZER_LONG_SINGLE_OFF_FREQ     10
#define BUZZER_LONG_SINGLE_OFF_OS_TIME  20
#define BUZZER_LONG_SINGLE_OFF_PWR_TIME 20


/* 结构类型定义 -----------------------------------------------------------------------*/

/* 原型声明部分 -----------------------------------------------------------------------*/

/* 全局变量声明 -----------------------------------------------------------------------*/

/* 静态变量定义 -----------------------------------------------------------------------*/
static uint8		ucBuzzerState = BUZZER_STATE_IDLE;
static ChordTone_t	*p_tChordTone = NULL;

static const ChordTone_t a_tChordTone_1_SingleSong[] =		//单音
{
	{BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
	{0, 0, 0},
};
static const ChordTone_t a_tChordTone_2_SingleSong[] =
{
	{BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
	{0, 0, 0},
};
static const ChordTone_t a_tChordTone_3_SingleSong[] =
{
	{BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
	{0, 0, 0},
};
static const ChordTone_t a_tChordTone_4_SingleSong[] =
{
	{BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
	{0, 0, 0},
};
static const ChordTone_t a_tChordTone_5_SingleSong[] =
{
	{BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
	{0, 0, 0},
};
static const ChordTone_t a_tChordTone_6_SingleSong[] =
{
	{BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
	{0, 0, 0},
};
static const ChordTone_t a_tChordTone_7_SingleSong[] =
{
	{BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
	{0, 0, 0},
};
static const ChordTone_t a_tChordTone_8_SingleSong[] =
{
	{BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
	{0, 0, 0},
};
static const ChordTone_t a_tChordTone_9_SingleSong[] =
{
	{BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
	{0, 0, 0},
};
static const ChordTone_t a_tChordTone_10_SingleSong[] =
{
	{BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
	{0, 0, 0},
};
static const ChordTone_t a_tChordTone_11_SingleSong[] =
{
	{BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
    {BUZZER_SINGLE_OFF_FREQ, BUZZER_SINGLE_OFF_OS_TIME, BUZZER_SINGLE_OFF_PWR_TIME},
    {BUZZER_SINGLE_FREQ, BUZZER_SINGLE_OS_TIME, BUZZER_SINGLE_PWR_TIME},
	{0, 0, 0},
};

static const ChordTone_t a_tChordTone_LongSingleSong[] =	//长单音
{
	//{2600, 2000, 2000},
	//{0, 0, 0},

	{BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
//    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
//	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
//    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
//	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
//    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
//	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
//    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
//	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
//    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
//	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
//    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
//	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
//    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
//	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
//    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
//	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
//    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
//	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
//    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
//	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
//    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
//	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
//    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
//	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
//    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
//	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
//    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
//	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
//    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
//	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
//    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
//	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
//    {BUZZER_LONG_SINGLE_FREQ, BUZZER_LONG_SINGLE_OS_TIME, BUZZER_LONG_SINGLE_PWR_TIME},
//	{BUZZER_LONG_SINGLE_OFF_FREQ, BUZZER_LONG_SINGLE_OFF_OS_TIME, BUZZER_LONG_SINGLE_OFF_PWR_TIME},
	
    
	{BUZZER_LONG_SINGLE_LAST_FREQ, BUZZER_LONG_SINGLE_LAST_OS_TIME, BUZZER_LONG_SINGLE_LAST_PWR_TIME},
    
    
//	{2600, 100, 20},
//	{10, 100, 20},
//	{2600, 100, 20},
//	{10, 100, 20},
//	{2600, 100, 20},
//	{10, 100, 20},
//	{2600, 100, 20},
//	{10, 100, 20},
//    {2600, 100, 20},
//	{10, 100, 20},
	{0, 0, 0},
};

//static const ChordTone_t a_tChordTone_OffSong[] =			//off
//{
//	{0, 0, 0},
//};

static const ChordTone_t a_tChordTone_PwrOnSong[] =			//1
{
	{600, 20, 20},
	{900, 10, 20},
	{1200, 40, 60},        //{2900, 210, 10},
	{0, 0, 0},
};
static const ChordTone_t a_tChordTone_PwrOffSong[] =		//2
{
	{300, 20, 20},
  {600, 10, 20},
	{1000, 40, 60},        //{2300, 210, 10},
	{0, 0, 0},
};

static const ChordTone_t a_tChordTone_Song2[] =				//3
{
	{100, 20, 20},
	{400, 10, 20},
	{700, 40, 60},        //{2300, 210, 10},
	{0, 0, 0},
};

static const ChordTone_t a_tChordTone_Song1[] =				//4
{
  {1000, 20, 20},
	{800, 20, 30},
	{200, 50, 80},        //{2900, 210, 10},
	{0, 0, 0},
};

static const ChordTone_t a_tChordTone_Song3[] =				//关机和弦音
{
	//{4000, 20, 20},
	//{3200, 20, 20},
	//{2000, 40, 10},        //{2300, 210, 10},
	//{0, 0, 0},

	{1200, 10, 10},		    //1
	{1200, 10, 10},			//2
{400, 10, 10},			//3

	{1200, 10, 10},			//4	稍有停顿
	{0, 0, 0},
};

static const ChordTone_t a_tChordTone_Song4[] =				//关闭气阀门和弦音
{
	//{4000, 20, 20},
	//{3200, 20, 20},
	//{2000, 40, 10},        //{2300, 210, 10},
	//{0, 0, 0},

	{200, 10, 10},		    //1
	{2500, 30, 10},			//2
//  {100, 10, 10},
	{0, 0, 0},
};

static const ChordTone_t a_tChordTone_TwoTigerSong[] =		//两只老虎和弦音
{
	{2000, 25, 25},			//1
	{2300, 25, 25},		    //2
	{2600, 25, 25},			//3
	{2000, 25, 25},			//1
	{2000, 25, 25},			//1
	{2300, 25, 25},			//2
	{2600, 25, 25},			//3
	{2000, 25, 25},		    //1
	{2300, 25, 25},			//2
	{2600, 25, 25},			//3

	{2900, 50, 25},			//4	稍有停顿
	{2300, 25, 25},			//2
	{2600, 25, 25},			//3
	{2900, 75, 25},		//4	和弦效果    {2900, 100, 25},
	{0, 0, 0},
};

static BOOL bChordTone_Running = FALSE;

/****************************************************************************************
* 函数名称：ChordTone_CtrlService()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-10-07 15:43
****************************************************************************************/
void ChordTone_CtrlService(void)
{

	static ChordTone_t	tTone;
	static uint32		uiChordTone_RunDly = 0;

	if (BSP_OS_Timeout(uiChordTone_RunDly, 10))
		uiChordTone_RunDly = BSP_OS_TimeGet();
	else
		return;

	switch (ucBuzzerState)
	{
		case BUZZER_STATE_START:
		{
			memcpy((char *)&tTone, (char *)p_tChordTone, sizeof(ChordTone_t));

			if (tTone.uiPWRTime)
			{
				tTone.uiPWRTime--;
				Buzzer_PwrOn();
			}
			else
			{
				ucBuzzerState = BUZZER_STATE_IDLE;
				break;
			}

			if (tTone.uiOSCTime)
			{
				tTone.uiOSCTime--;
				ChordTone_PWMEnable(tTone.uiFreq);
			}
			else
			{
				Buzzer_PwrOff();
				ucBuzzerState = BUZZER_STATE_IDLE;
				break;
			}

			ucBuzzerState = BUZZER_STATE_RUN;
		}
		break;

		case BUZZER_STATE_RUN:
		{
			if (tTone.uiPWRTime)
				tTone.uiPWRTime--;
			else
				Buzzer_PwrOff();

			if (tTone.uiOSCTime)
				tTone.uiOSCTime--;
			else
			{
				ChordTone_PWMDisable();
				p_tChordTone++;
				ucBuzzerState = BUZZER_STATE_START;
			}
		}
		break;

		case BUZZER_STATE_IDLE:	
        {
            Buzzer_PwrOff();
			      ChordTone_PWMDisable();
            bChordTone_Running = FALSE; 
        }
        break;
	}
	
}
/****************************************************************************************
* 函数名称：ChordTone_Start()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-10-07 15:43
****************************************************************************************/
void ChordTone_Start(ChordTone_Music_e eMusic)
{
	BOOL ret = TRUE;

    if (bChordTone_Running)
    {
        if (eCHORD_TONE_MUSIC_OFF == eMusic)
            bChordTone_Running = FALSE;
        else
            return;
    }
    else
        bChordTone_Running = TRUE;
    
	switch (eMusic)
	{
	    	case eCHORD_TONE_MUSIC_1_SINGLE		: p_tChordTone = (ChordTone_t *)a_tChordTone_1_SingleSong;	break;
        case eCHORD_TONE_MUSIC_2_SINGLE     : p_tChordTone = (ChordTone_t *)a_tChordTone_2_SingleSong;	break;
        case eCHORD_TONE_MUSIC_3_SINGLE     : p_tChordTone = (ChordTone_t *)a_tChordTone_3_SingleSong;	break;
        case eCHORD_TONE_MUSIC_4_SINGLE     : p_tChordTone = (ChordTone_t *)a_tChordTone_4_SingleSong;	break;
        case eCHORD_TONE_MUSIC_5_SINGLE     : p_tChordTone = (ChordTone_t *)a_tChordTone_5_SingleSong;	break;
        case eCHORD_TONE_MUSIC_6_SINGLE     : p_tChordTone = (ChordTone_t *)a_tChordTone_6_SingleSong;	break;
        case eCHORD_TONE_MUSIC_7_SINGLE     : p_tChordTone = (ChordTone_t *)a_tChordTone_7_SingleSong;	break;
        case eCHORD_TONE_MUSIC_8_SINGLE     : p_tChordTone = (ChordTone_t *)a_tChordTone_8_SingleSong;	break;
        case eCHORD_TONE_MUSIC_9_SINGLE     : p_tChordTone = (ChordTone_t *)a_tChordTone_9_SingleSong;	break;
        case eCHORD_TONE_MUSIC_10_SINGLE    : p_tChordTone = (ChordTone_t *)a_tChordTone_10_SingleSong;	break;
        case eCHORD_TONE_MUSIC_11_SINGLE    : p_tChordTone = (ChordTone_t *)a_tChordTone_11_SingleSong;	break;
        
        
				case eCHORD_TONE_MUSIC_LONG_SINGLE	: p_tChordTone = (ChordTone_t *)a_tChordTone_LongSingleSong;break;
				case eCHORD_TONE_MUSIC_OFF			: //p_tChordTone = (ChordTone_t *)a_tChordTone_OffSong;		break;
				{
					ret = FALSE;

					p_tChordTone = NULL;

					Buzzer_PwrOff();
					ChordTone_PWMDisable();

					ucBuzzerState = BUZZER_STATE_IDLE;
				}
				break;
		  	case eCHORD_TONE_MUSIC_PWR_ON		: p_tChordTone = (ChordTone_t *)a_tChordTone_PwrOnSong;	    break;
			  case eCHORD_TONE_MUSIC_PWR_OFF		: p_tChordTone = (ChordTone_t *)a_tChordTone_PwrOffSong;	break;
		  	case eCHORD_TONE_MUSIC_TWO_TIGER	: p_tChordTone = (ChordTone_t *)a_tChordTone_TwoTigerSong;	break;
			  case eCHORD_TONE_MUSIC_SONG1		: p_tChordTone = (ChordTone_t *)a_tChordTone_Song1;			break;
				case eCHORD_TONE_MUSIC_SONG2		: p_tChordTone = (ChordTone_t *)a_tChordTone_Song2;			break;
				case eCHORD_TONE_MUSIC_SONG3		: p_tChordTone = (ChordTone_t *)a_tChordTone_Song3;			break;
        //关闭指令语音
				case eCHORD_TONE_MUSIC_SONG4		: p_tChordTone = (ChordTone_t *)a_tChordTone_Song4;			break;
				default                             : p_tChordTone = (ChordTone_t *)a_tChordTone_1_SingleSong;  break;
	}

	if (ret)
		ucBuzzerState = BUZZER_STATE_START;
}
/****************************************************************************************
* 函数名称：ChordTone_Init()
* 功能描述: 
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-10-07 15:43
****************************************************************************************/
void ChordTone_Init(void)
{
	Buzzer_GPIOInit();
	ChordTone_PWMDevInit();
    //ChordTone_PWMEnable(100);

	Buzzer_PwrOff();
}


/****************************************************************************************
* 函数名称：ChordTone_GasClosevoice()
* 功能描述: 气阀关闭按键声音函数
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2017-10-07 15:43
****************************************************************************************/
void ChordTone_GasClosevoice(ChordTone_Music_e eMusic)
{
  static ChordTone_t	tTone;
  //static uint32		uiChordTone_RunDly = 0;
	
    if (bChordTone_Running)
    {
        if (eCHORD_TONE_MUSIC_OFF == eMusic)
            bChordTone_Running = FALSE;
        else
            return;
    }
    else
        bChordTone_Running = TRUE;
  p_tChordTone = (ChordTone_t *)a_tChordTone_Song4;

BUZZER_STATE:		
			memcpy((char *)&tTone, (char *)p_tChordTone, sizeof(ChordTone_t));
		  tTone.uiPWRTime = tTone.uiPWRTime * 40000;
		  tTone.uiOSCTime = tTone.uiOSCTime * 40000;
			Buzzer_PwrOn();
			ChordTone_PWMEnable(tTone.uiFreq);
			while(tTone.uiPWRTime != 0 && tTone.uiOSCTime != 0)
			{	
				if (tTone.uiPWRTime)
					tTone.uiPWRTime--;
				else
					Buzzer_PwrOff();

				if (tTone.uiOSCTime)
					tTone.uiOSCTime--;
				else
				{
					ChordTone_PWMDisable();
					p_tChordTone++;
					goto BUZZER_STATE;
				}
			}
			Buzzer_PwrOff();
			ChordTone_PWMDisable();
			bChordTone_Running = FALSE; 
}

/*******************************版权所有(c)**************END OF FILE********************/
