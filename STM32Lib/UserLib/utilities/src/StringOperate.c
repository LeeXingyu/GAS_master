/************************************��Ȩ����(c)****************************************
* ��Ȩ����(c), 2012-2013, XXXXXXXXXXXX, ��������Ȩ  
* �ļ���      ��StringOperate.c
* ����        ��XXXX
* �汾        ��V 1.0
* ����        ��2014/01/09
* ��������    ���ַ����Ƚ�
* ��ע        ��
* ��Ҫ�����б�
* �޸ļ�¼    ��
  ��      ��          �汾          �޸���          �޸�����  
  2014/01/09          1.0           XXXX            �����ļ�
****************************************************************************************/

/* ���Բ��ֿ��� -----------------------------------------------------------------------*/

/* �����ļ� ---------------------------------------------------------------------------*/
#include "utilities.h"

/* �궨�� -----------------------------------------------------------------------------*/
#define BASE            10

/* �ṹ���Ͷ��� -----------------------------------------------------------------------*/

/* ԭ���������� -----------------------------------------------------------------------*/

/* ȫ�ֱ������� -----------------------------------------------------------------------*/

/* ��̬�������� -----------------------------------------------------------------------*/

/****************************************************************************************
* ��������: cmpbuf()
* ��������: �ַ����Ƚ�
* ��ڲ���: ��Ҫ�Ƚϵ������ַ���������
* ���ڲ���: 
* ����ֵ  : ���or�����
* ����    : 
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
* ��������: ReStrcmp()
* ��������: �ַ����ȽϺ�����д
* ��ڲ���: ��Ҫ�Ƚϵ������ַ���
* ���ڲ���: 
* ����ֵ  : ���or�����
* ����    : 
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
* ��������: ReStrlen()
* ��������: �ַ�������
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : ����
* ����    : 
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
* ��������: ReMemcpy()
* ��������: ��дmemcpy����
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
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
* ��������: hexalpha_to_int()
* ��������: ʮ�������ַ������� a-f �� A-F�������������
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
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
* ��������: htoi()
* ��������: ��ʮ�������ַ���ת��Ϊ������
* ��ڲ���: �ַ���
* ���ڲ���: 
* ����ֵ  : ת�����������
* ����    : 
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
* ��������: Is_NumDigit()
* ��������: 
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
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
* ��������: ISStringNumber()
* ��������: �鿴�ַ����Ƿ�������
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
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
* ��������: Test_Toupper()
* ��������: Сдת��Ϊ��д
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
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


/*******************************��Ȩ����(c)**************END OF FILE********************/
