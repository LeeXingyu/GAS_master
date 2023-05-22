/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXXXXXX, 保留所有权  
* 文件名      ：crc.c
* 作者        ：XXXX
* 版本        ：V 1.0
* 日期        ：2013/04/17
* 功能描述    ：crc校验
* 备注        ：
* 主要函数列表：
* 修改记录    ：
  日      期          版本          修改人          修改内容  
  2013/04/17          1.0           XXXX            创建文件
****************************************************************************************/

/* 调试部分开关 -----------------------------------------------------------------------*/

/* 包含文件 ---------------------------------------------------------------------------*/
#include "utilities.h"

/* 宏定义 -----------------------------------------------------------------------------*/

/* 结构类型定义 -----------------------------------------------------------------------*/

/* 原型声明部分 -----------------------------------------------------------------------*/

/* 全局变量声明 -----------------------------------------------------------------------*/

/* 静态变量定义 -----------------------------------------------------------------------*/
static const uint8 NMEA_CheckArray[] = 
{	
	0x30, 0x31, 0x32, 0x33,             //'0','1','2','3','4','5','6','7',
	0x34, 0x35, 0x36, 0x37,
	0x38, 0x39, 0x41, 0x42,				//'8','9','A','B','C','D','E','F'
	0x43, 0x44, 0x45, 0x46
};

const static uint16 crc16_ccitt_table[256] =
{
	0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
	0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
	0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
	0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
	0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
	0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
	0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
	0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
	0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
	0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
	0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
	0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
	0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
	0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
	0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
	0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
	0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
	0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
	0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
	0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
	0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
	0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
	0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
	0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
	0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
	0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
	0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
	0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
	0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
	0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
	0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
	0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78,
};

/****************************************************************************************
* 函数名称: crc16_ccitt_byte()
* 功能描述: crc16单字节校验
* 入口参数: 校准初始值，需要校准的数据
* 出口参数: 
* 返回值  : 
* 其它    : 校准结果
****************************************************************************************/
uint16 crc16_ccitt_byte(uint16 check, uint8 dat)
{
    //uint8 i;

    //check ^= dat;
    //for (i= 0; i < 8; i++ )
    //{
    //    check = (check & 1) ? ((check >> 1) ^ 0x8408) : (check >> 1);
    //}

    //return (check);

	check = (check >> 8) ^ crc16_ccitt_table[(check ^ dat) & 0xFF];

	return (check);
}

/****************************************************************************************
* 函数名称: crc16_ccitt_block()
* 功能描述: crc16多字节校验
* 入口参数: 校准数据堆，校准初始值，校准数据长度
* 出口参数: 
* 返回值  : 
* 其它    : 校准结果
****************************************************************************************/
uint16 crc16_ccitt_block(uint8 *buf, uint16 check, uint32 len)
{
	//uint32  j;
	//uint8   i;
 // 
 // 	for(j = 0; j < len; j++)
 // 	{
 //   	check ^= buf[j];
 //   	for(i = 0; i < 8; i++)
 //  		   	check = (check & 1) ? ((check >> 1) ^ 0x8408) : (check >> 1);
 // 	}
 // 	return (check);
 // 	

	uint8 *ptr = buf;

	while (len-- > 0)
	{
		check = (check >> 8) ^ crc16_ccitt_table[(check ^ (*ptr++)) & 0xFF];
	}

	return (check);
}

/****************************************************************************************
* 函数名称: crc16_byte()
* 功能描述: crc16单字节校验
* 入口参数: 校准初始值，需要校准的数据
* 出口参数: 
* 返回值  : 
* 其它    : 校准结果
****************************************************************************************/
uint16 crc16_byte(uint16 check, uint8 dat)
{
    uint8 i;

    check ^= dat;
    for (i= 0; i < 8; i++ )
    {
        check = (check & 1)? ((check >> 1) ^ 0xA001) : (check >> 1);
    }

    return(check);
}

/****************************************************************************************
* 函数名称: crc16_ccitt_block()
* 功能描述: crc16多字节校验
* 入口参数: 校准数据堆，校准初始值，校准数据长度
* 出口参数: 
* 返回值  : 
* 其它    : 校准结果
****************************************************************************************/
uint16 crc16_block(uint8 *buf, uint16 check, uint32 len)
{
	uint32  j;
	uint8   i;
  
  	for(j = 0; j < len; j++)
  	{
    	check ^= buf[j];
    	for(i = 0; i < 8; i++)
   		   	check = (check & 1) ? ((check >> 1) ^ 0xA001) : (check >> 1);
  	}
  	return(check);
}

/****************************************************************************************
* 函数名称: crc_block_ethernet()
* 功能描述: 以太网CRC32校验(多字节)
* 入口参数: 校准数据堆，校准初始值，校准数据长度
* 出口参数: 
* 返回值  : 
* 其它    : 校准结果
****************************************************************************************/
uint32  crc_block_ethernet(uint8 *buf, uint32 check, uint32 len)
{    
//    uint32    *ptr = (uint32 *)buf;
//    uint32    xbit;
//    uint32    data;
//    uint32    CRC = check;    // init
//    
//    int bits;
//    
//    while (len--)
//    {
//        xbit = 1UL << 31;

//        data = *ptr++;
//        for (bits = 0; bits < 32; bits++)
//        {
//            if (CRC & 0x80000000) 
//            {
//                CRC <<= 1;
//                CRC ^= 0x04C11DB7;
//            }
//            else
//                CRC <<= 1;
//            
//            if (data & xbit)
//                CRC ^= 0x04C11DB7;

//            xbit >>= 1;
//        }
//    }
//    
//    return CRC;

    
    BOOL	has_remainder;
    char	remainder;
	char	complement[4] = {0, 0, 0, 0};
    uint32  i, count;
	uint32	cursor;

	has_remainder = (len & 0x03) ? TRUE : FALSE;

	if (TRUE == has_remainder)
	{
		count 		= 0;
		remainder 	= len & 0x03;
		cursor 		= len & 0xFFFFFFFC;

		while(count < remainder)
		 	complement[count++] = buf[cursor++];
	}
    
  	for(count = 0; (count < len) && (count + 4 <= len); count += 4)
  	{        
    	for(i = 0; i < 32; i++)
        {
   		   	check = (check & 0x80000000) ? ((check << 1) ^ 0x04C11DB7) : (check << 1);

			check = (*((uint32 *)&buf[count]) & (0x80000000 >> i)) ? (check ^ 0x04C11DB7) : check;
        }
  	}
    
    if (TRUE == has_remainder)
    {
        for(i = 0; i < 32; i++)
        {
   		   	check = (check & 0x80000000) ? ((check << 1) ^ 0x04C11DB7) : (check << 1);

			check = (*((uint32 *)complement) & (0x80000000 >> i)) ? (check ^ 0x04C11DB7) : check;
        }
    }
    
    return check;
}
#if 0
/****************************************************************************************
* 函数名称: crc32_ethernet_block()
* 功能描述: 文件下载CRC校验
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 校准结果
****************************************************************************************/
uint32 crc32_ethernet_block(uint8 *data, uint32 length)
{
	uint32 	count;
	uint32 	check;
	char	complement[4] = {0, 0, 0, 0};
	BOOL	has_remainder;
	char	remainder;
	uint32	cursor;
	
	has_remainder = length&0x03?TRUE:FALSE;

	if(has_remainder)
	{
		count 		= 0;
		remainder 	= length&0x03;
		cursor 		= length&0xFFFFFFFC;

		while(count < remainder)
		 	complement[count++] = data[cursor++];
	} 

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	CRC_ResetDR();

	check = CRC_CalcBlockCRC((uint32_t*)data, length>>2);
	if(has_remainder)
		check = CRC_CalcCRC(*(uint32*)complement);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, DISABLE); 

	return check;	 
}
#endif

/****************************************************************************************
* 函数名称: DS18B20_CRC8Check()
* 功能描述: DS18B20数据CRC校验
* 入口参数: 要校验的数据，校验数据长度
* 出口参数: 
* 返回值  : 校验码
* 其它    : 
****************************************************************************************/
BOOL DS18B20_CRC8Check(unsigned char *p_CRCCheckData, unsigned char ucCRCDataLength)
{
	unsigned char ucCRCRegisterData = 0x00;                 //对于模除余数的初始化，ISO/IEC 13239标准规定各位（8、16、32）
                                                            //而对于DS18B20器件和一些控制仪表的通信CRC码却是清0.
	unsigned char tmp = 0;                                      
	unsigned char i,j;
	for (j = 0; j < ucCRCDataLength; j++)                   //处理完全部数据
	{
		tmp = p_CRCCheckData[j];
		for (i = 0; i < 8; i++)
		{
			if((ucCRCRegisterData ^ tmp) & 0x01)            //逆序算法，如果移出去的最低位为1，就是和“逆序去最高位多项式”异或
			{                                               //此算法，相当于数学除法中，如果高位为0，则对齐下一位。
				ucCRCRegisterData >>= 1;
				ucCRCRegisterData ^= 0x8c;                  //与去高位逆序多项式相异或
			}
			else
			{
				ucCRCRegisterData >>= 1;                    //逆序算法，若遇到最低位为0，则右移一位，直到遇到1为止，在与多项式异或
			}
			tmp >>= 1;
		}
	}
	return (ucCRCRegisterData ? FALSE : TRUE);
}

/****************************************************************************************
* 函数名称: NMEA_CheckSum()
* 功能描述: NMEA协议校验
* 入口参数: 要校验的数据
* 出口参数: 
* 返回值  : 校验码
* 其它    : 
****************************************************************************************/
uint16 NMEA_CheckSum(uint8 check)
{
    uint16 usCheckSum;
    
    usCheckSum = NMEA_CheckArray[check & 0x0F];
    usCheckSum = usCheckSum << 8;
    usCheckSum = usCheckSum | NMEA_CheckArray[check >> 4];
    
    return  usCheckSum;
}

/****************************************************************************************
* 函数名称: SumCheck()
* 功能描述: 和校验
* 入口参数: 校验初始数据，要校验的数据
* 出口参数: 
* 返回值  : 校验码
* 其它    : 
****************************************************************************************/
uint8 SumCheck(uint8 check, uint8 check_data)
{
    return (check + check_data);
}

/****************************************************************************************
* 函数名称: HalfWordSumCheck()
* 功能描述: 半字和校验
* 入口参数: 校验初始数据，要校验的数据
* 出口参数: 
* 返回值  : 校验码
* 其它    : 
****************************************************************************************/
uint16 HalfWordSumCheck(uint16 check, uint8 check_data)
{
    uint16 tmp;
    
    tmp = check + check_data;
    
    return tmp;
}


/*******************************版权所有(c)**************END OF FILE********************/
