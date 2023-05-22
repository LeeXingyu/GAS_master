/************************************��Ȩ����(c)****************************************
* ��Ȩ����(c), 2012-2013, �ɶ������Ƽ����޹�˾, ��������Ȩ  
* �ļ���      ��CoreInclude.h
* ����        ���� ��
* �汾        ��V 1.0
* ����        ��2014/11/08
* ��������    ��CPU��Ҫ�ں�ͷ�ļ�
* ��ע        ��
* ��Ҫ�����б���
* �޸ļ�¼    ��
  ��      ��          �汾          �޸���          �޸�����  
  2014/11/08          1.0           ����            �����ļ�
****************************************************************************************/

/* ��ֹͷ�ļ��ظ����� -----------------------------------------------------------------*/
#ifndef __CoreInclude_H__
#define __CoreInclude_H__

/* ���Բ��ֿ��� -----------------------------------------------------------------------*/

/* �ⲿ���� ---------------------------------------------------------------------------*/
#ifdef    CORE_INCLUDE_MODULE
#define   CORE_INCLUDE_EXT
#else
#define   CORE_INCLUDE_EXT  extern
#endif

/* �����ļ� ---------------------------------------------------------------------------*/
#ifdef STM32F1XX
#include "stm32f10x.h"
#include "core_cm3.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_it.h"
#endif //:~STM32F1XX

#ifdef STM32F0XX
#include "stm32f0xx.h"
#include "core_cm0.h"
#include "stm32f0xx_conf.h"
#include "stm32f0xx_it.h"
#endif //:~STM32F0XX

#ifdef STM32F4XX
#include "stm32f4xx.h"
#include "core_cm4.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_it.h"
#endif //:~STM32F4XX

/* �궨�� -----------------------------------------------------------------------------*/

/* �ṹ���Ͷ��� -----------------------------------------------------------------------*/

/* ԭ���������� -----------------------------------------------------------------------*/

#endif /* __CoreInclude_H__ */
/*******************************��Ȩ����(c)**************END OF FILE********************/