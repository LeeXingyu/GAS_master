/************************************��Ȩ����(c)****************************************
* ��Ȩ����(c), 2012-2013, XXXXXXXXXXXX, ��������Ȩ  
* �ļ���      ��STM32Fxxx_Flash.c
* ����        ��XXXX
* �汾        ��V 1.0
* ����        ��2013/12/10
* ��������    ��STM32��flash��д
* ��ע        ��
* ��Ҫ�����б�
* �޸ļ�¼    ��
  ��      ��          �汾          �޸���          �޸�����  
  2013/12/10          1.0           XXXX            �����ļ�
****************************************************************************************/

/* ���Բ��ֿ��� -----------------------------------------------------------------------*/

/* �����ļ� ---------------------------------------------------------------------------*/
#include "CoreInclude.h"
#include "NewType.h"

#include <string.h>

/* �궨�� -----------------------------------------------------------------------------*/

/* �ṹ���Ͷ��� -----------------------------------------------------------------------*/

/* ԭ���������� -----------------------------------------------------------------------*/

/* ȫ�ֱ������� -----------------------------------------------------------------------*/

/* ��̬�������� -----------------------------------------------------------------------*/

/****************************************************************************************
* ��������: STM32Fxx_FlashInit()
* ��������: STM32��flash��ʼ��
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
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
* ��������: STM32Fxx_FlashErase()
* ��������: STM32��flash����
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
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
* ��������: STM32Fxx_FlashWrite()
* ��������: STM32��flashд��
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
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
* ��������: STM32Fxxx_FlashRead()
* ��������: STM32��flash��ȡ
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
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
* ��������: STM32Fxx_FlashRead()
* ��������: STM32��flash��ȡ
* ��ڲ���: 
* ���ڲ���: 
* ����ֵ  : 
* ����    : 
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

/*******************************��Ȩ����(c)**************END OF FILE********************/
