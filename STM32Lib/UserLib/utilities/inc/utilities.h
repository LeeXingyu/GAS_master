/************************************��Ȩ����(c)****************************************
* ��Ȩ����(c), 2012-2013, XXXXXXXXXXXXX, ��������Ȩ  
* �ļ���      ��utilities.h
* ����        ��XXXX
* �汾        ��V 1.0
* ����        ��2013/03/01
* ��������    ��utilitiesͷ�ļ�
* ��ע        ��
* ��Ҫ�����б�
* �޸ļ�¼    ��
  ��      ��          �汾          �޸���          �޸�����  
  2013/03/01          1.0           XXXX            �����ļ�
****************************************************************************************/

/* ��ֹͷ�ļ��ظ����� -----------------------------------------------------------------*/
#ifndef __UTILITIES_H__
#define __UTILITIES_H__

/* ���Բ��ֿ��� -----------------------------------------------------------------------*/

/* �����ļ� ---------------------------------------------------------------------------*/
#include "NewType.h"

/* �궨�� -----------------------------------------------------------------------------*/
//delay
#define     PLL_MUL_X  3

//CRC
#define     CRC_INITIAL                 0xFFFF                  //Initial FCS value
#define     CHECK_SUM                   0xF0B8                  //Good final FCS value

#define     CRC16_INITIAL               0xFFFF                  //crc16У���ʼֵ
#define     CRC16_CHECK_SUM             0x0                     //crc16У��������У��λһ��У���

#define     CRC32_INITIAL               0xFFFFFFFF              //��ʼ��CRC32ֵ

#define     CRC32_TABLE_INIT            0xFFFFFFFF              //��ʼ��CRC32ֵ

#define     CRC32_CMD_INIT              0x01                    //STM32�ڲ�CRC����ʼ������
#define     CRC32_CMD_CHK               0x02                    //STM32�ڲ�CRC������CRCУ�����
#define     CRC32_CMD_END               0x04                    //STM32�ڲ�CRC������CRC����

#define     COMPARE_DAY_TIME_EQUCAL     0                       //ʱ�����(Сʱ�ͷ���)
#define     COMPARE_DAY_TIME_PAST       1                       //ʱ�����(Сʱ�ͷ���)
#define     COMPARE_DAY_TIME_NOTYET     -1                      //ʱ��û��(Сʱ�ͷ���)

/* �ṹ���Ͷ��� -----------------------------------------------------------------------*/
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

/* �������� ---------------------------------------------------------------------------*/

/* ԭ���������� -----------------------------------------------------------------------*/
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

//��̫��CRC
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

//��У��
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

//�����ж�
BOOL LeapYear(uint8 year);
BOOL CheckRTCRight(const RTCTime *const rtc);   //���ʱ��Ϸ���
void CheckRTCLegal(RTCTime *rtc);               //���ʱ��Ϸ��ԣ������Ϸ����ָ�ʱ��Ĭ��ֵ

//UTCʱ��ת��Ϊʱ��ʱ��
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

//��С��ģʽ�жϣ�TRUE��С��;
BOOL SmallEndian_Check(void);
uint32 NetWork32ToHostByte(uint32 convert);
uint16 NetWork16ToHostByte(uint16 convert);
uint32 Host32ToNetWorkByte(uint32 convert);
uint16 Host16ToNetWorkByte(uint16 convert);

//hex��bcd��ת��
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


//���ڼ�������
uint8 DateToWeek(const RTCTime *const rtc);

//ð������
void BubbleSort_HalfWord(uint16 *buf, uint32 count);

//RTCѹ��
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

//��ʱ�ƻ���ʱ����Ч�Լ��
BOOL PlanTimeChk_24H(const RTCTime *const rtc);
BOOL PlanTimeChk_Other(const RTCTime *const rtc);

//��ȡ�ֽ����ݵĶ�����1�ĸ���
uint32 GetByte_BitOneCount(uint8 data_byte);

#endif /* __UTILITIES_H__ */
/*******************************��Ȩ����(c)**************END OF FILE********************/
