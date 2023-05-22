/************************************��Ȩ����(c)****************************************
* ��Ȩ����(c), 2012-2013, XXXXXXXXXXXXX, ��������Ȩ  
* �ļ���      ��NewType.h
* ����        ��
* �汾        ��V 1.0
* ����        ��2012/12/05
* ��������    ����������һЩ���������Լ�����ͨ�õ�����
* ��ע        ��
* ��Ҫ�����б�
* �޸ļ�¼    ��
  ��      ��          �汾          �޸���          �޸�����  
  2012/12/05          1.0           XXXX            �����ļ�
****************************************************************************************/

/* ��ֹͷ�ļ��ظ����� -----------------------------------------------------------------*/
#ifndef __NewType_H__
#define __NewType_H__

/* ���Բ��ֿ��� -----------------------------------------------------------------------*/
#if (defined(STM32F1XX) || defined(STM32F0XX) || defined(STM32F4XX))
#define ARM_CORE
#endif  //(defined(STM32F1XX) || defined(STM32F0XX) || defined(STM32F4XX))

/* �����ļ� ---------------------------------------------------------------------------*/

/* �궨�� -----------------------------------------------------------------------------*/
#ifdef USE_FULL_ASSERT_CHECK
#define ASSERT_PARAM(expr) ((expr) ? (void)0 : Assert_Failed((uint8_t *)__FILE__, __LINE__))
void Assert_Failed(uint8_t* file, uint32_t line);
#else
#define ASSERT_PARAM(exp)     ((void)0)
#endif  //USE_FULL_ASSERT_CHECK

#define ENDIAN_TYPE_BIG			1
#define ENDIAN_TYPE_LITTLE 		2

#ifndef CPU_DEF_ENDIAN_TYPE
#define CPU_DEF_ENDIAN_TYPE     ENDIAN_TYPE_LITTLE
#endif  //CPU_DEF_ENDIAN_TYPE

#ifdef ARM_CORE
#define CPU_XDATA
#define CPU_IDATA
#define CPU_CODE
#else
#define CPU_XDATA               xdata
#define CPU_IDATA               idata
#define CPU_CODE                code
#endif  //ARM_CORE

#ifndef NULL
#define NULL                    ((void *)0)
#endif

#ifndef EOF
#define EOF                     -1
#endif

#ifndef DEF_DISABLED
#define  DEF_DISABLED           0
#endif  //DEF_DISABLED

#ifndef DEF_ENABLED
#define  DEF_ENABLED            1
#endif  //DEF_ENABLED

#ifndef DEF_FALSE
#define  DEF_FALSE              0
#endif  //DEF_FALSE

#ifndef DEF_TRUE
#define  DEF_TRUE               1
#endif  //DEF_TRUE

#ifndef RET_FAIL
#define RET_FAIL  		        1
#endif  //#ifndef RET_FAIL

#ifndef RET_OK
#define RET_OK  		        0
#endif  //#ifndef RET_OK

#ifndef RET_NOK
#define RET_NOK  		        -1
#endif  //#ifndef RET_NOK

/* �ṹ���Ͷ��� -----------------------------------------------------------------------*/
#ifdef ARM_CORE
typedef unsigned long long      uint64;
typedef signed long long        int64;
typedef signed int              int32;
typedef unsigned int            uint32;
typedef unsigned char           uint8;
typedef signed char             int8;
typedef unsigned char           BOOL;
typedef signed short int        int16;
typedef unsigned short int      uint16;

#else
typedef unsigned long           uint32;
typedef signed long             int32;
typedef signed int              int16;
typedef unsigned int            uint16;
typedef unsigned char           uint8;
typedef signed char             int8;
typedef unsigned char           BOOL;
typedef signed short int        sint16;
typedef unsigned short int      usint16;
#endif      //ARM_CORE

//RTC
typedef struct
{
    uint8 ucYear;
    uint8 ucMonth;
    uint8 ucDay;
    uint8 ucHour;
    uint8 ucMinute;
    uint8 ucSecond;
    uint8 ucWeekDay;
}_RTC;

#define FALSE   (0)
#define TRUE    (!FALSE)
//typedef enum 
//{
//    FALSE = 0,
//    TRUE = !FALSE
//}BOOL;

typedef enum
{
    LOW_LEVEL   = 0,
    HIGH_LEVEL  = !LOW_LEVEL
}Level_e;

typedef union
{
	uint32 			lword;
	uint16 			word[2];
	uint8  			byte[4];
}Word32;

typedef union
{
	uint32 			lword[2];
	uint16 			word[4];
	uint8  			byte[8];
}Word64;

#pragma pack(1)
typedef struct _RTCTime
{
 	unsigned char year;
 	unsigned char month;
 	unsigned char day;
 	unsigned char hour;
 	unsigned char minute;
 	unsigned char second;
}RTCTime;
#pragma pack()

/* ԭ���������� -----------------------------------------------------------------------*/

#endif /* __NewType_H__ */
/*******************************��Ȩ����(c)**************END OF FILE********************/
