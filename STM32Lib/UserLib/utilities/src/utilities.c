/************************************��Ȩ����(c)****************************************
* ��Ȩ����(c), 2012-2013, XXXXXXXXXXXX, ��������Ȩ  
* �ļ���      ��utilities.c
* ����        ��XXXX
* �汾        ��V 1.0
* ����        ��2013/03/01
* ��������    �����ù��ܵĺ���
* ��ע        ��
* ��Ҫ�����б�
* �޸ļ�¼    ��
  ��      ��          �汾          �޸���          �޸�����  
  2013/03/01          1.0           XXXX            �����ļ�
****************************************************************************************/

/* ���Բ��ֿ��� -----------------------------------------------------------------------*/

/* �����ļ� ---------------------------------------------------------------------------*/
#include "utilities.h"

/* �궨�� -----------------------------------------------------------------------------*/
#define     __nop()         __asm{NOP}

/* �ṹ���Ͷ��� -----------------------------------------------------------------------*/
typedef struct _DataByte_Bit_t
{
	unsigned a :1;
	unsigned b :1;
	unsigned c :1;
	unsigned d :1;

	unsigned e :1;
	unsigned f :1;
	unsigned g :1;
	unsigned h :1;
}DataByte_Bit_t;

/* ԭ���������� -----------------------------------------------------------------------*/

/* ȫ�ֱ������� -----------------------------------------------------------------------*/

/* ��̬�������� -----------------------------------------------------------------------*/
static const uint8 c_aDataHalfByte_BitCount[16] =
{
	0, 1, 1, 2, 
	1, 2, 2, 3, 
	1, 2, 2, 3, 
	2, 3, 3, 4
};

//#ifdef STM32F0XX
//volatile uint32_t G_tickValue = 0;
///****************************************************************************************
//* ��������: delay_ms()
//* ��������: ʹ��sysTick��ʱ����ms����ʱ
//* ��ڲ���: ��ʱ����ms
//* ���ڲ���: 
//* ����ֵ  : 
//* ����    : ��ȷ��ʱ
//****************************************************************************************/
//void delay_ms(uint32_t n_ms)
//{
//    // SysTick interrupt each 1000 Hz with HCLK equal to 32MHz
//    // - 30 to compensate the overhead of this sub routine
//    SysTick_Config(16000*PLL_MUL_X - 30);
//    // Enable the SysTick Counter

//    G_tickValue = n_ms;
//    while(G_tickValue == n_ms)
//    {
//        ;
//    }

//    // SysTick interrupt each 1000 Hz with HCLK equal to 32MHz
//    SysTick_Config(16000*PLL_MUL_X);
//    while(G_tickValue != 0)
//    {
//        ;
//    }
//}

///****************************************************************************************
//* ��������: delay_10us()
//* ��������: ʹ��sysTick��ʱ����10us����ʱ
//* ��ڲ���: ��ʱ���ٸ�10us
//* ���ڲ���: 
//* ����ֵ  : 
//* ����    : ��ȷ��ʱ
//****************************************************************************************/
//void delay_10us(uint32_t n_10us)
//{
//    G_tickValue = n_10us;
//    
//    SysTick_Config(160 * PLL_MUL_X - 30);  
//    while(G_tickValue == n_10us)
//    {
//        ;
//    }

////     SysTick_Config(80 * PLL_MUL_X);
//    while(G_tickValue != 0)
//    {
//        ;
//    }
//}

///****************************************************************************************
//* ��������: delay_5us()
//* ��������: ѭ��ʹ�ÿղ������ﵽ5us����ʱ
//* ��ڲ���: ��ʱ���ٸ�5us
//* ���ڲ���: 
//* ����ֵ  : 
//* ����    : ��ʱ���Ƿǳ���ȷ
//****************************************************************************************/
//void delay_5us(uint32_t n_5us)
//{
//    unsigned int uiDelay = n_5us << 2;
//    
//    for(; uiDelay > 0; uiDelay--)
//    {
//        __nop();
//    }
//}
//#endif  //STM32F0XX

/****************************************************************************************
* ��������: LeapYear()
* ��������: �����ж�
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
****************************************************************************************/
BOOL LeapYear(uint8 year)
{
    int leap_year = year + 2000;
    
    if (((0 == leap_year % 4) && ((leap_year % 100) != 0)) || (0 == (leap_year % 400)))
        return TRUE;
    else
        return FALSE;
}

/****************************************************************************************
* ��������: CheckRTCRight()
* ��������: ���д��ʱ�䣬�Ƿ���ȷ
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 2000���Ժ��
****************************************************************************************/
BOOL CheckRTCRight(const RTCTime *rtc)
{
    BOOL ret = TRUE;
    
    if (rtc->year > 99 || rtc->month > 12 || rtc->month == 0)
        ret = FALSE;
    
    switch (rtc->month)
    {
        case 1:	case 3: case 5: case 7: case 8: case 10:
        case 12: if ((rtc->day > 31) || (!rtc->day))
                    ret = FALSE;
                 break;
        case  2: if (LeapYear(rtc->year))
                 {
                     if ((rtc->day > 29) || (!rtc->day))
                        ret = FALSE;
                 }
                 else
                 {
                     if ((rtc->day > 28) || (!rtc->day))
                        ret = FALSE;
                 }
                 break;
        case 4:	case 6: case 9:
        case 11: if ((rtc->day > 30) || (!rtc->day))
                    ret = FALSE;
                 break;
        default: break;
    }
    
    if (rtc->hour > 23 || rtc->minute > 59 || rtc->second > 59)
        ret = FALSE;
        
    return ret;
}

/****************************************************************************************
* ��������: CmdOperate_CheckRTC()
* ��������: ���д��ʱ�䣬�Ƿ���ȷ��������ʱ��ṹ����
* ��ڲ���: д���ʱ����������ʱ���Ƿ�Ϸ����������޸�ΪĬ��0ֵ�������޸�
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
****************************************************************************************/
void CheckRTCLegal(RTCTime *rtc)
{
    rtc->year = (rtc->year <= 99) ? rtc->year : 99;                  //�����жϣ��������д�(������Χ)����ָ�ΪĬ�ϸ�ֵ
    
    rtc->month = (rtc->month <= 12) ? rtc->month : 12;
    rtc->month = (rtc->month) ? rtc->month : 1;

    switch (rtc->month)
    {
        case 1:	case 3: case 5: case 7: case 8: case 10:
        case 12: rtc->day = (rtc->day <= 31) ? rtc->day : 31;
                 break;
        case  2: if (LeapYear(rtc->year))
                     rtc->day = (rtc->day <= 29) ? rtc->day : 29;
                 else
                     rtc->day = (rtc->day <= 28) ? rtc->day : 28;
                 break;
        case 4:	case 6: case 9:
        case 11: rtc->day = (rtc->day <= 31) ? rtc->day : 31;
                 break;
        default: break;
    }
    rtc->day    = (rtc->day) ? rtc->day : 1;
    
    rtc->hour   = (rtc->hour <= 23) ? rtc->hour : 23;
    rtc->minute = (rtc->minute <= 59) ? rtc->minute : 59;
    rtc->second = (rtc->second <= 59) ? rtc->second : 59;
}

/****************************************************************************************
* ��������: SmallEndian_Check()
* ��������: ��˻�С��ģʽ�ж�
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : TRUE��С��ģʽ��FALSE�����ģʽ
* ����    : 
****************************************************************************************/
BOOL SmallEndian_Check(void)
{
    union _CheckEndian
    {
        int  check_int;
        char check_char[4];    
    }CheckEndian;
    
    CheckEndian.check_int = 0x01;
    
    return (CheckEndian.check_char[0] == 0x01) ? TRUE : FALSE;
}

/****************************************************************************************
* ��������: NetWork32ToHostByte()
* ��������: ����ϵͳ��С��ģʽ��32λ���ݴ������ֽ�˳��ת��Ϊ�����ֽ�˳��
* ��ڲ���: ��Ҫת��������
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
****************************************************************************************/
uint32 NetWork32ToHostByte(uint32 convert)
{
    union _ConverEndian
    {
        uint32  Cov_int;
        char    Cov_char[4];    
    }ConverEndian;
    
    ConverEndian.Cov_int = convert;
    if (SmallEndian_Check())
    {
        ConverEndian.Cov_char[0] = (char)(convert >> 24);
        ConverEndian.Cov_char[1] = (char)(convert >> 16);
        ConverEndian.Cov_char[2] = (char)(convert >> 8);
        ConverEndian.Cov_char[3] = (char)(convert);
    }
    
    return ConverEndian.Cov_int;
}

/****************************************************************************************
* ��������: NetWork16ToHostByte()
* ��������: ����ϵͳ��С��ģʽ��16λ���ݴ������ֽ�˳��ת��Ϊ�����ֽ�˳��
* ��ڲ���: ��Ҫת��������
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
****************************************************************************************/
uint16 NetWork16ToHostByte(uint16 convert)
{
    union _ConverEndian
    {
        uint16  Cov_int;
        char    Cov_char[2];    
    }ConverEndian;
    
    ConverEndian.Cov_int = convert;
    if (SmallEndian_Check())
    {
        ConverEndian.Cov_char[0] = (char)(convert >> 8);
        ConverEndian.Cov_char[1] = (char)(convert);
    }
    
    return ConverEndian.Cov_int;
}

/****************************************************************************************
* ��������: Host32ToNetWorkByte()
* ��������: ����ϵͳ��С��ģʽ��32λ���ݴ������ֽ�˳��ת��Ϊ�����ֽ�˳��
* ��ڲ���: ��Ҫת��������
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
****************************************************************************************/
uint32 Host32ToNetWorkByte(uint32 convert)
{
    union _ConverEndian
    {
        uint32  Cov_int;
        char    Cov_char[4];    
    }ConverEndian;
    
    ConverEndian.Cov_int = convert;
    if (SmallEndian_Check())
    {
        ConverEndian.Cov_char[0] = (char)(convert >> 24);
        ConverEndian.Cov_char[1] = (char)(convert >> 16);
        ConverEndian.Cov_char[2] = (char)(convert >> 8);
        ConverEndian.Cov_char[3] = (char)(convert);
    }
    
    return ConverEndian.Cov_int;
}

/****************************************************************************************
* ��������: Host16ToNetWorkByte()
* ��������: ����ϵͳ��С��ģʽ��16λ���ݴ������ֽ�˳��ת��Ϊ�����ֽ�˳��
* ��ڲ���: ��Ҫת��������
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
****************************************************************************************/
uint16 Host16ToNetWorkByte(uint16 convert)
{
    union _ConverEndian
    {
        uint16  Cov_int;
        char    Cov_char[2];    
    }ConverEndian;
    
    ConverEndian.Cov_int = convert;

    if (SmallEndian_Check())
    {
        ConverEndian.Cov_char[0] = (char)(convert >> 8);
        ConverEndian.Cov_char[1] = (char)(convert);
    }
    
    return ConverEndian.Cov_int;
}

/****************************************************************************************
* �������ƣ�GetByte_BitOneCount()
* ������������ȡ�ֽ���bit1�ĸ���
* ��ڲ�����
* ����ֵ  ��
* ����    ��
* ����ʱ�䣺2016-07-08 11:07
****************************************************************************************/
uint32 GetByte_BitOneCount(uint8 data_byte)
{
 //   DataByte_Bit_t *p_tDataByte = (DataByte_Bit_t *)&data_byte;

	//return (p_tDataByte->a + p_tDataByte->b + p_tDataByte->c + p_tDataByte->d +\
	//		p_tDataByte->e + p_tDataByte->f + p_tDataByte->g + p_tDataByte->h);
	//		

	return (c_aDataHalfByte_BitCount[data_byte & 0x0F] + c_aDataHalfByte_BitCount[data_byte >> 4]);
}

/****************************************************************************************
* ��������: cmpbuf()
* ��������: �ַ����Ƚ�
* ��ڲ���: ��Ҫ�Ƚϵ������ַ���������
* ���ڲ���: 
* ����ֵ  : ���or�����
* ����    : 
****************************************************************************************/
BOOL cmp_buf(const char * const soc, const char *const dest, uint32 len)
{
	uint32 count = 0;
	
	while((count < len) && (soc[count] == dest[count]))
    {
		count++;
    }

	if(count == len)
		return TRUE;
	else
		return FALSE;		
}

/*******************************��Ȩ����(c)**************END OF FILE********************/
