/************************************版权所有(c)****************************************
* 版权所有(c), 2012-2013, XXXXXXXXXXXX, 保留所有权  
* 文件名      ：STM32Fxxx_Flash.c
* 作者        ：XXXX
* 版本        ：V 1.0
* 日期        ：2013/12/10
* 功能描述    ：STM32的flash读写
* 备注        ：
* 主要函数列表：
* 修改记录    ：
  日      期          版本          修改人          修改内容  
  2013/12/10          1.0           XXXX            创建文件
****************************************************************************************/

/* 调试部分开关 -----------------------------------------------------------------------*/

/* 包含文件 ---------------------------------------------------------------------------*/
#include "CoreInclude.h"
#include "NewType.h"

#include <string.h>

/* 宏定义 -----------------------------------------------------------------------------*/

/* 结构类型定义 -----------------------------------------------------------------------*/

/* 原型声明部分 -----------------------------------------------------------------------*/

/* 全局变量声明 -----------------------------------------------------------------------*/

/* 静态变量定义 -----------------------------------------------------------------------*/

/****************************************************************************************
* 函数名称: STM32Fxx_FlashInit()
* 功能描述: STM32的flash初始化
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
void STM32Fxxx_FlashInit(void)
{
    FLASH_Unlock();                                                                             //Unlock the Program memory

#ifdef STM32F10X_HD
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    FLASH_GetStatus();
#endif  //STM32F10X_HD

#ifdef STM32F0XX
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR | FLASH_FLAG_BSY);    //Clear all FLASH flags
#endif  //STM32F0XX
}

/****************************************************************************************
* 函数名称: STM32Fxx_FlashErase()
* 功能描述: STM32的flash擦除
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
BOOL STM32Fxxx_FlashErase(uint32 addr, uint32 len)
{
	BOOL    ret 	    = TRUE;
	uint32  end_addr    = addr + len;
    uint32  addr_count  = addr;
    uint32  page_size   = 1024;
	
#ifdef STM32F10X_HD
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    FLASH_GetStatus();
#endif  //STM32F10X_HD

#ifdef STM32F0XX
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR | FLASH_FLAG_BSY);    //Clear all FLASH flags
#endif  //STM32F0XX
	FLASH_Unlock();
    
	while(ret && (addr_count < end_addr))
	{
		if(FLASH_COMPLETE == FLASH_ErasePage(addr))
			addr_count += page_size;
		else
			ret = FALSE;			
	}

	FLASH_Lock();

	return ret;
}

/****************************************************************************************
* 函数名称: STM32Fxx_FlashWrite()
* 功能描述: STM32的flash写入
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
BOOL STM32Fxxx_FlashWrite(uint8 *write_data, uint32 addr, uint32 len)
{
    BOOL   ret = TRUE;
    uint32 count;
    uint32 addr_count = addr;

    if (len & 0x03)
        return FALSE;

#ifdef STM32F10X_HD
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    FLASH_GetStatus();
#endif  //STM32F10X_HD

#ifdef STM32F0XX
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_WRPERR | FLASH_FLAG_PGERR | FLASH_FLAG_BSY);    //Clear all FLASH flags
#endif  //STM32F0XX
    FLASH_Unlock();

    count = 0;
    while ((TRUE == ret) && (count < len))
    {
        if (FLASH_COMPLETE == FLASH_ProgramWord(addr_count, *(uint32 *)&write_data[count]))
        {
            if (*(uint32 *)(addr_count) != *(uint32 *)&write_data[count])
                ret = FALSE;
        }
        else
        {
            ret = FALSE;
        }
        
        addr_count  += 4;
        count       += 4;
    }

    FLASH_Lock();
    
    return ret;
}

/****************************************************************************************
* 函数名称: STM32Fxxx_FlashRead()
* 功能描述: STM32的flash读取
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
BOOL STM32Fxxx_FlashRead(uint8 *read_data, uint32 addr, uint32 len)
{
	uint32 	count;
    uint32  addr_count = addr;

    if ((len & 0x03) || (addr & 0x03))
		return FALSE;
        
	count = 0;
	while(count < len)
	{
		*(uint16*)&read_data[count] = *(volatile uint16*)(addr_count);
		count       += 2;
		addr_count  += 2;
	}
	return TRUE;
}

static uint16 FLASH_PAGE_SIZE = 1024;
/****************************************************************************************
* 函数名称: STM32Fxx_FlashRead()
* 功能描述: STM32的flash读取
* 入口参数: 
* 出口参数: 
* 返回值  : 
* 其它    : 
****************************************************************************************/
BOOL STM32Fxx_FlashPageWrite(uint8 *data, uint32 addr, uint32 len)
{
	uint8 buf[1024];
	uint32 	page_index 	= addr&(~(FLASH_PAGE_SIZE-1));
	uint32	page_addr  	= addr&(FLASH_PAGE_SIZE-1);

	STM32Fxxx_FlashRead(buf, page_index, FLASH_PAGE_SIZE);
	memcpy((char*)&buf[page_addr], data, len);

	if(STM32Fxxx_FlashErase(page_index, len))
	{
		if(STM32Fxxx_FlashWrite(buf, page_index, FLASH_PAGE_SIZE))
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
}

/*******************************版权所有(c)**************END OF FILE********************/
