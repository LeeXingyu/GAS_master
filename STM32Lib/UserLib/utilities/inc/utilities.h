/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXXXXXXX, 保留所有权  
* 文件名      ：utilities.h
* 作者        ：XXXX
* 版本        ：V 1.0
* 日期        ：2013/03/01
* 功能描述    ：utilities头文件
* 备注        ：
* 主要函数列表：
* 修改记录    ：
  日      期          版本          修改人          修改内容  
  2013/03/01          1.0           XXXX            创建文件
****************************************************************************************/

/* 防止头文件重复包含 -----------------------------------------------------------------*/
#ifndef __UTILITIES_H__
#define __UTILITIES_H__

/* 调试部分开关 -----------------------------------------------------------------------*/

/* 包含文件 ---------------------------------------------------------------------------*/
#include "NewType.h"

/* 宏定义 -----------------------------------------------------------------------------*/
//delay
#define     PLL_MUL_X  3

//CRC
#define     CRC_INITIAL                 0xFFFF                  //Initial FCS value
#define     CHECK_SUM                   0xF0B8                  //Good final FCS value

#define     CRC16_INITIAL               0xFFFF                  //crc16校验初始值
#define     CRC16_CHECK_SUM             0x0                     //crc16校验结果，连校验位一起校验后

#define     CRC32_INITIAL               0xFFFFFFFF              //初始化CRC32值

#define     CRC32_TABLE_INIT            0xFFFFFFFF              //初始化CRC32值

#define     CRC32_CMD_INIT              0x01                    //STM32内部CRC，初始化操作
#define     CRC32_CMD_CHK               0x02                    //STM32内部CRC，数据CRC校验操作
#define     CRC32_CMD_END               0x04                    //STM32内部CRC，结束CRC操作

#define     COMPARE_DAY_TIME_EQUCAL     0                       //时间相等(小时和分钟)
#define     COMPARE_DAY_TIME_PAST       1                       //时间过了(小时和分钟)
#define     COMPARE_DAY_TIME_NOTYET     -1                      //时间没到(小时和分钟)

/* 结构类型定义 -----------------------------------------------------------------------*/
typedef enum _TimeZoneList
{
    eTIME_ZONE_DISTRICT = 0,
    eEAST_ONE_DISTRICT,
    eEAST_TWO_DISTRICT,
    eEAST_THREE_DISTRICT,
    eEAST_FOUR_DISTRICT,
    eEAST_FIVE_DISTRICT,
    eEAST_SIX_DISTRICT,
    eEAST_SEVEN_DISTRICT,
    eEAST_EIGHT_DISTRICT,
    eEAST_NINE_DISTRICT,
    eEAST_TEN_DISTRICT,
    eEAST_ELEVEN_DISTRICT,
    eEAST_TWELVE_DISTRICT,
    eWEST_TWELVE_DISTRICT,
    eWEST_ELEVEN_DISTRICT,
    eWEST_TEN_DISTRICT,
    eWEST_NINE_DISTRICT,
    eWEST_EIGHT_DISTRICT,
    eWEST_SEVEN_DISTRICT,
    eWEST_SIX_DISTRICT,
    eWEST_FIVE_DISTRICT,
    eWEST_FOUR_DISTRICT,
    eWEST_THREE_DISTRICT,
    eWEST_TWO_DISTRICT,
    eWEST_ONE_DISTRICT
}TimeZoneList;

/* 变量声明 ---------------------------------------------------------------------------*/

/* 原型声明部分 -----------------------------------------------------------------------*/
//delay
void delay_ms(uint32 n_ms);
void delay_10us(uint32 n_10us);
void delay_5us(uint32 n_5us);
void delay_us(uint32 n_us);

//CRC
uint16	crc16_ccitt_byte(uint16 check, uint8 dat);
uint16 	crc16_ccitt_block(uint8 *buf, uint16 check, uint32 len);
BOOL    DS18B20_CRC8Check(unsigned char *p_CRCCheckData, unsigned char ucCRCDataLength);
uint16  crc16_byte(uint16 check, uint8 dat);
uint16  crc16_block(uint8 *buf, uint16 check, uint32 len);

//以太网CRC
uint32	crc_byte_ethernet(uint32 check, uint8 dat);
uint32  crc_block_ethernet(uint8 *buf, uint32 check, uint32 len);

//File DownLoad CRC32
uint32  crc32_ethernet_block(const uint8 *const data, uint32 length);
uint32  crc32_EthernetBlock(const uint8 *const data, uint32 length, uint8 cmd);

//CRC32 Table
uint32 CRC32_TableBlock(const char *const load, uint32 check, uint32 length);
uint32 CRC32_TableByte(char byte, uint32 check);

//NMEA Check
uint16  NMEA_CheckSum(uint8 check);

//和校验
uint8   SumCheck(uint8 check, uint8 check_data);
uint16  HalfWordSumCheck(uint16 check, uint8 check_data);

//compare
BOOL    cmpbuf(const uint8 *soc, const uint8 *dest, uint32 len);
BOOL    cmp_buf(const char * const soc, const char *const dest, uint32 len);

//resolve string
uint8* ResolveString(const uint8 *buf, uint8 ucResolveChar, uint8 **ResolvedAddr);
BOOL ReStrcmp(const uint8 *soc, const uint8 *dest);
int ReStrlen(const uint8 *str);
void *ReMemcpy(const void *dest, const void *source, int len);

//闰年判断
BOOL LeapYear(uint8 year);
BOOL CheckRTCRight(const RTCTime *const rtc);   //检查时间合法性
void CheckRTCLegal(RTCTime *rtc);               //检查时间合法性，若不合法，恢复时间默认值

//UTC时间转换为时区时间
void UTCToTimeZone(RTCTime *tConvert, TimeZoneList eTimeZone);

//char to unsigned int or unsigned short int
uint32 CharsToUint32(const uint8 *const value);
uint16 CharsToUint16(const uint8 *const value);
void Uint32ToChars(uint32 value, uint8 *const buf);
void Uint16ToChars(uint16 value, uint8 *const buf);

//file check
BOOL Safety_FileReloadCheck(void);
BOOL App_FileCopyVerify(void);

//Coordinate convert
void CoordinateConv(float fDegree, uint16 *usDegree, uint16 *usMinute, float *fSecond);

//大小端模式判断，TRUE：小端;
BOOL SmallEndian_Check(void);
uint32 NetWork32ToHostByte(uint32 convert);
uint16 NetWork16ToHostByte(uint16 convert);
uint32 Host32ToNetWorkByte(uint32 convert);
uint16 Host16ToNetWorkByte(uint16 convert);

//hex和bcd码转换
uint8 hex_to_bcd(uint8 data);
uint8 bcd_to_hex(uint8 data);
void ConvertHexToBCD(uint32 hex, uint8 *bcd, uint16 len);
uint32 ConvertBCDToHex(uint8 *bcd, uint16 len);

//chang hex string to int
unsigned int htoi(const char s[]);

//convert string to number
BOOL ISStringNumber(int *const result, char const *const buf);

//IPv4
BOOL IS_IPv4(char *ip_str, uint8 cLen);
BOOL Find_IPv4(char *ip);


//日期计算星期
uint8 DateToWeek(const RTCTime *const rtc);

//冒泡排序
void BubbleSort_HalfWord(uint16 *buf, uint32 count);

//RTC压缩
uint32 PackRTC(const RTCTime *const rtc);
void UnpackRTC(RTCTime *const rtc, uint32 time);
BOOL CompareRTC(const RTCTime *const start_rtc, const RTCTime *const end_rtc);
BOOL IsEndRTCAfterStart_NoSec(const RTCTime *p_tStartRTC, const RTCTime *p_tEndRTC);
int32 GetRTCInterval(const RTCTime *const tStartTime, const RTCTime *const tEndTime);
BOOL RTCIntervalOverflow(const RTCTime *const p_tStartTime, const RTCTime *const p_tEndTime, uint32 interval);
uint32 GetRTCMinuteFromDate(RTCTime *rtc);
BOOL BetweenRTCTime(const RTCTime *const start, const RTCTime *const end, const RTCTime *const now);
BOOL BeforeTheDay(const RTCTime *const start, const RTCTime *const end, const RTCTime *const now);
int CompareTheDayTime(const RTCTime *const start, const RTCTime *const now);
uint32 RTCAddOneDay(const RTCTime *const rtc);

//定时计划，时间有效性检查
BOOL PlanTimeChk_24H(const RTCTime *const rtc);
BOOL PlanTimeChk_Other(const RTCTime *const rtc);

//获取字节数据的二进制1的个数
uint32 GetByte_BitOneCount(uint8 data_byte);

#endif /* __UTILITIES_H__ */
/*******************************版权所有(c)**************END OF FILE********************/
