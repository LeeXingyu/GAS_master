/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXXXXXX, 保留所有权  
* 文件名      ：utilities.c
* 作者        ：XXXX
* 版本        ：V 1.0
* 日期        ：2013/03/01
* 功能描述    ：常用功能的函数
* 备注        ：
* 主要函数列表：
* 修改记录    ：
  日      期          版本          修改人          修改内容  
  2013/03/01          1.0           XXXX            创建文件
****************************************************************************************/

/* 调试部分开关 -----------------------------------------------------------------------*/

/* 包含文件 ---------------------------------------------------------------------------*/
#include "utilities.h"

/* 宏定义 -----------------------------------------------------------------------------*/
#define     __nop()         __asm{NOP}

/* 结构类型定义 -----------------------------------------------------------------------*/
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

/* 原型声明部分 -----------------------------------------------------------------------*/

/* 全局变量声明 -----------------------------------------------------------------------*/

/* 静态变量定义 -----------------------------------------------------------------------*/
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
//* 函数名称: delay_ms()
//* 功能描述: 使用sysTick定时器做ms级延时
//* 入口参数: 延时多少ms
//* 出口参数: 
//* 返回值  : 
//* 其它    : 精确延时
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
//* 函数名称: delay_10us()
//* 功能描述: 使用sysTick定时器做10us级延时
//* 入口参数: 延时多少个10us
//* 出口参数: 
//* 返回值  : 
//* 其它    : 精确延时
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
//* 函数名称: delay_5us()
//* 功能描述: 循环使用空操作，达到5us级延时
//* 入口参数: 延时多少个5us
//* 出口参数: 
//* 返回值  : 
//* 其它    : 延时不是非常精确
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
* 函数名称: LeapYear()
* 功能描述: 闰年判断
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
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
* 函数名称: CheckRTCRight()
* 功能描述: 检查写入时间，是否正确
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 2000年以后的
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
* 函数名称: CmdOperate_CheckRTC()
* 功能描述: 检查写入时间，是否正确，并放入时间结构体中
* 入口参数: 写入的时间参数，检查时间是否合法，若否，则修改为默认0值，否则不修改
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void CheckRTCLegal(RTCTime *rtc)
{
    rtc->year = (rtc->year <= 99) ? rtc->year : 99;                  //输入判断，若输入有错(超出范围)，则恢复为默认赋值
    
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
* 函数名称: SmallEndian_Check()
* 功能描述: 大端或小端模式判断
* 入口参数: 
* 出口参数: 
* 返回值  : TRUE：小端模式，FALSE：大端模式
* 其它    : 
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
* 函数名称: NetWork32ToHostByte()
* 功能描述: 根据系统大小端模式，32位数据从网络字节顺序转换为主机字节顺序
* 入口参数: 需要转换的数据
* 出口参数: 
* 返回值  : 
* 其它    : 
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
* 函数名称: NetWork16ToHostByte()
* 功能描述: 根据系统大小端模式，16位数据从网络字节顺序转换为主机字节顺序
* 入口参数: 需要转换的数据
* 出口参数: 
* 返回值  : 
* 其它    : 
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
* 函数名称: Host32ToNetWorkByte()
* 功能描述: 根据系统大小端模式，32位数据从主机字节顺序转换为网络字节顺序
* 入口参数: 需要转换的数据
* 出口参数: 
* 返回值  : 
* 其它    : 
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
* 函数名称: Host16ToNetWorkByte()
* 功能描述: 根据系统大小端模式，16位数据从主机字节顺序转换为网络字节顺序
* 入口参数: 需要转换的数据
* 出口参数: 
* 返回值  : 
* 其它    : 
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
* 函数名称：GetByte_BitOneCount()
* 功能描述：获取字节中bit1的个数
* 入口参数：
* 返回值  ：
* 其它    ：
* 生成时间：2016-07-08 11:07
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
* 函数名称: cmpbuf()
* 功能描述: 字符串比较
* 入口参数: 需要比较的两个字符串，长度
* 出口参数: 
* 返回值  : 相等or不相等
* 其它    : 
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

/*******************************版权所有(c)**************END OF FILE********************/
