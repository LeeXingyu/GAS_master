/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXXXXXX, 保留所有权  
* 文件名      ：StringOperate.c
* 作者        ：XXXX
* 版本        ：V 1.0
* 日期        ：2014/01/09
* 功能描述    ：字符串比较
* 备注        ：
* 主要函数列表：
* 修改记录    ：
  日      期          版本          修改人          修改内容  
  2014/01/09          1.0           XXXX            创建文件
****************************************************************************************/

/* 调试部分开关 -----------------------------------------------------------------------*/

/* 包含文件 ---------------------------------------------------------------------------*/
#include "utilities.h"

/* 宏定义 -----------------------------------------------------------------------------*/
#define BASE            10

/* 结构类型定义 -----------------------------------------------------------------------*/

/* 原型声明部分 -----------------------------------------------------------------------*/

/* 全局变量声明 -----------------------------------------------------------------------*/

/* 静态变量定义 -----------------------------------------------------------------------*/

/****************************************************************************************
* 函数名称: cmpbuf()
* 功能描述: 字符串比较
* 入口参数: 需要比较的两个字符串，长度
* 出口参数: 
* 返回值  : 相等or不相等
* 其它    : 
****************************************************************************************/
BOOL cmpbuf(const uint8 *soc, const uint8 *dest, uint32 len)
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

/****************************************************************************************
* 函数名称: ReStrcmp()
* 功能描述: 字符串比较函数重写
* 入口参数: 需要比较的两个字符串
* 出口参数: 
* 返回值  : 相等or不相等
* 其它    : 
****************************************************************************************/
BOOL ReStrcmp(const uint8 *soc, const uint8 *dest)
{
    uint16 count = 0;
    BOOL ret = FALSE;
	
    if (!soc || !dest)
        return ret;
    
    do
    {
        ret = (soc[count] == dest[count]) ? TRUE : FALSE;
        count++;
    }while(ret && dest[count - 1]);

    return ret;
}

/****************************************************************************************
* 函数名称: ReStrlen()
* 功能描述: 字符串长度
* 入口参数: 
* 出口参数: 
* 返回值  : 长度
* 其它    : 
****************************************************************************************/
int ReStrlen(const uint8 *str)
{
    int count;
	
    if ((!str) || (!(*str)))
        return 0;
    
    count = 0;
    while(str[count])
    {
        count++;
    }
    
    return count;
}

/****************************************************************************************
* 函数名称: ReMemcpy()
* 功能描述: 重写memcpy函数
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void *ReMemcpy(const void *dest, const void *source, int len)
{
    char *ret       = (char*)dest;
    char *dest_t    = ret;
    char *source_t  = (char*)source;
    
    while(len--)
    {
        *dest_t++ = *source_t++;
    }
    
    return ret;
}

/****************************************************************************************
* 函数名称: hexalpha_to_int()
* 功能描述: 十六进制字符包含在 a-f 或 A-F中启用这个方法
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
static int hexalpha_to_int(int c)
{
    char hexalpha[] = "aAbBcCdDeEfF";
    int i;
    int answer = 0;

    for(i = 0; answer == 0 && hexalpha[i] != '\0'; i++)
    {
        if(hexalpha[i] == c)
        {
            answer = 10 + (i / 2);
        }
    }

    return answer;
}

/****************************************************************************************
* 函数名称: htoi()
* 功能描述: 将十六进制字符串转换为整形数
* 入口参数: 字符串
* 出口参数: 
* 返回值  : 转换后的整型数
* 其它    : 
****************************************************************************************/
unsigned int htoi(const char s[])
{
    unsigned int answer = 0;
    int i = 0;
    int valid = 1;
    int hexit;

    if(s[i] == '0')
    {
        ++i;
        if(s[i] == 'x' || s[i] == 'X')
        {
            ++i;
        }
    }
    
    while(valid && s[i] != '\0')
    {
        answer = answer * 16;
        if(s[i] >= '0' && s[i] <= '9')
        {
            answer = answer + (s[i] - '0');
        }
        else
        {
            hexit = hexalpha_to_int(s[i]);
            if(hexit == 0)
            {
                valid = 0;
            }
            else
            {
                answer = answer + hexit;
            }
        }
        ++i;
    }
    
    if(!valid)
    {
        answer = 0;
    }

    return answer;
}

/****************************************************************************************
* 函数名称: Is_NumDigit()
* 功能描述: 
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
static unsigned int Is_NumDigit(unsigned int c)
{
	int             result  = -1;
	unsigned int    base    = BASE;

	// Limit the base
	if (base < 2)
	{
		base = 2;
	}
	if (base > 36)
	{
		base = 36;
	}

	// Find which range of digits to test
	if (base <= 10)
	{
		// Check for the range of 0 to 9, limited by the base
		if ((c >= '0') && (c < ('0' + base)))
		{
			result = c - '0';
		}
	}
	else
	{
		// Check for the range 0 to 9
		if ((c >= '0') && (c <= '9'))
		{
			result = c - '0';
		}
		else
		{
			// Mask off the bit to convert lower case to upper case!!!!!!!!!!
			c &= 0xFFDF;

			// check for the range A to Z, limited by base
			if ((c >= 'A') && (c < ('A' - 10 + base)))
			{
				result = c - 'A' + 10;
			}
		}
	}

	return result;
}

/****************************************************************************************
* 函数名称: ISStringNumber()
* 功能描述: 查看字符串是否是数字
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
BOOL ISStringNumber(int *const result, char const *const buffer)
{
    int     Local_result = 0;
	int     Local_digit;
	BOOL    bValidResult = TRUE;
	BOOL    negate = FALSE;
    uint8   count = 0;

	if(buffer[count] == '-')
	{
		count++;
		negate = TRUE;
	}

	while(bValidResult && buffer[count])
	{
		Local_result *= BASE;
		Local_digit = Is_NumDigit((unsigned int)buffer[count]);
		if (Local_digit == -1)
		{
			bValidResult = FALSE;
		}
		else
		{
			Local_result += Local_digit;
		}
        
		count++;
	}

	if (negate)
	{
		*result = -(int)Local_result;
	}
	else
	{
		*result = Local_result;
	}

	return bValidResult;
}

/****************************************************************************************
* 函数名称: Test_Toupper()
* 功能描述: 小写转换为大写
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
//char Test_Toupper(char cConvert)
//{
//    char ret = cConvert;
//    
//    if (cConvert >= 'a' && (cConvert <= 'z'))
//    {
//        ret = cConvert - 0x20;
//    }
//    
//    return ret;
//}


/*******************************版权所有(c)**************END OF FILE********************/
