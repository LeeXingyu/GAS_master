/************************************????(c)****************************************
* ????(c), 2012-2013, XXXXXXXX??, ?????
* ???      :GasStove_Service.c
* ??        :XXXXXX
* ????    :XXXXXX
* ??        :V1.0.0.0
* ??        :2017-10-06
* ????    :
* ??        :
* ??????:
* ????    :
  ?      ?          ??          ???          ????
  2017-10-06          1.0           XXXXXXXX        ????
****************************************************************************************/

/* ?????? -----------------------------------------------------------------------*/

/* ???? ---------------------------------------------------------------------------*/
#include "rtc.h"
#include "KeyBoard.h"
#include "STM32Fxxx_Misc.h"
#include "ChordTone.h"
#include "HT1621.h"

#include "utilities.h"

/* ??? -----------------------------------------------------------------------------*/
#define GAS_STOVE_DIS_RTC 0x00
#define GAS_STOVE_DIS_TIMING 0x01
#define GAS_STOVE_DIS_SET 0x02

#define GAS_STOVE_EDIT_ENSURE_TIME 4000 //????,??????,??????

#define GAS_STOVE_DIS_SWITCH_SHORT_TIME (2000u)
#define GAS_STOVE_DIS_SWITCH_TIME (5000u)

#define GAS_STOVE_TIMING_MAX_TIME (180)

#define GAS_STOVE_BLINK_RTC_HOUR 0x03       //????
#define GAS_STOVE_BLINK_RTC_TEN_MINUTE 0x04 //????:??
#define GAS_STOVE_BLINK_RTC_MINUTE 0x08     //????:??

#define GAS_STOVE_BLINK_SET_RTC 0x10 //??????

#define GAS_STOVE_BLINK_SET_START 0x17 //????????
#define GAS_STOVE_BLINK_SET_END 0x17   //????????

#define GAS_STOVE_BLINK_TIMING_HOUR 0x02       //????
#define GAS_STOVE_BLINK_TIMING_TEN_MINUTE 0x04 //????:??
#define GAS_STOVE_BLINK_TIMING_MINUTE 0x08     //????:??

#define GAS_STOVE_BLINK_NULL 0x00 //???

#define LED_SEG_BLINK_DOT (0x10)         //??????
#define LED_SEG_BLINK_DOT_AND_ONE (0x11) //??????????

#define GAS_STOVE_MAX_SET_FUN (4) //?????????

#define GAS_STOVE_DEF_START_TIME (1u) //??????
#define GAS_STOVE_DEF_END_TIME (5u)   //??????,???

#define GAS_STOVE_GAS_SENSOR_ALARM_INTERVAL (180000) //??????

#define GAS_STOVE_STORAGE_PARA_ADDR (0x0800A000)

#if (1000 == FACTOR_1_S)
#define GAS_STOVE_TIMING_1_MINUTE (60000u)
#else
#define GAS_STOVE_TIMING_1_MINUTE (60000u)
#endif // #if (1000 == FACTOR_1_S)

#define GAS_STOVE_ALARM_ENSURE_TIME (10000)

/* ?????? -----------------------------------------------------------------------*/
typedef struct _GasStove_Para_t
{
    uint16 usStartTime;
    uint16 usEndTime;

    //??????????

    uint32 check;
} GasStove_Para_t;

/* ?????? -----------------------------------------------------------------------*/
static void GasStove_DisRTC(void);
static void GasStove_DisTiming(void);
static void GasStove_DisSet(void);

extern BOOL GasStove_FireState(void);   //??????
extern BOOL GasStove_FanRotation(void); //??????
extern void GasStove_Flameout(void);    //????
extern BOOL GasStove_GasSensor(void);   //????

extern void Cooker_SysIdMatch(void);
extern BOOL Cooker_SysIdMatchResult(void);

static void SettingTiming_Ensure(void);
static void GasStove_ExtEnsure(void);

static void Setting_SysIdMatch(void);
static void Setting_SysIdMatchStart(void);
static void Setting_SysIdMatchEnsure(void);

extern void LED_SEG_Update(uint8 *pVaulue);
extern void LED_SEGBlinkSet(uint8 value);

/* ?????? -----------------------------------------------------------------------*/

/* ?????? -----------------------------------------------------------------------*/
static uint8 a_ucGasStove_LEDRTCBuffer[4] = {0, 0, 0, 0};
static uint8 a_ucGasStove_LEDTimeBuffer[4] = {0, 0, 0, 0};
static uint8 a_ucGasStove_LEDParaTimeBuffer[4] = {10, 10, 0, 0};
static uint8 a_ucGasStove_LEDIdMatchBuffer[4] = {10, 10, 10, 0};

static uint8 ucGasStove_DisplayState = GAS_STOVE_DIS_RTC;

static uint32 uiGasStove_DisDly = 0;

static uint32 uiGasStove_Timing = 0;
static uint32 uiTimingCnt = 0;

static BOOL bGasStove_TimingEnable = FALSE;

static BOOL bGasStove_GasSensorState = FALSE;
static uint32 uiGasStove_GasSensorDly;

static GasStove_Para_t tGasStove_Para;

static uint32 uiCooker_MatchIdWaitDly;
static BOOL bSetting_SysIdMatchState = FALSE;

static uint8 ucPressCnt = 0; //???????????

static uint32 uiGasStove_TimingEnDly; //?????,3S?????
static BOOL bGasStove_TimingSetEn = FALSE;

static void (*const a_pfnGasStove_Service[])(void) =
    {
        GasStove_DisRTC,
        GasStove_DisTiming,
        GasStove_DisSet,
};

/****************************************************************************************
 * ????:GasStove_DisRTC()
 * ????: ???????
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void GasStove_TimingIRQHandler(void)
{
    if (++uiTimingCnt >= GAS_STOVE_TIMING_1_MINUTE)
    {
        uiTimingCnt = 0;
        uiGasStove_Timing--;
        if (!uiGasStove_Timing)
            SysTick_FunReg(NULL);
    }
}

/****************************************************************************************
 * ????:GasStove_DisRTC()
 * ????: ??????,?5S???????
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void GasStove_DisRTC(void)
{
    static uint8 hour, minute;

    // if ((!uiGasStove_Timing) && (BSP_OS_Timeout(uiGasStove_DisDly, GAS_STOVE_DIS_SWITCH_TIME)))
    if (BSP_OS_Timeout(uiGasStove_DisDly, GAS_STOVE_DIS_SWITCH_TIME))
    {
        uiGasStove_DisDly = BSP_OS_TimeGet();

        if (uiGasStove_Timing > 0) //??????
        {
            ucGasStove_DisplayState = GAS_STOVE_DIS_TIMING;

            LED_SEG_Update(a_ucGasStove_LEDTimeBuffer);
            LED_SEGBlinkSet(LED_SEG_BLINK_DOT_AND_ONE);
        }
    }
    else
    {
        RTCTime now;

        get_rtc(&now);
        if ((now.hour != hour) || (now.minute != minute))
        {
            uint8 tmp;

            tmp = now.hour / 10;
            a_ucGasStove_LEDRTCBuffer[0] = (tmp > 9) ? 9 : tmp;
            tmp = now.hour % 10;
            a_ucGasStove_LEDRTCBuffer[1] = (tmp > 9) ? 9 : tmp;

            tmp = now.minute / 10;
            a_ucGasStove_LEDRTCBuffer[2] = (tmp > 9) ? 9 : tmp;
            tmp = now.minute % 10;
            a_ucGasStove_LEDRTCBuffer[3] = (tmp > 9) ? 9 : tmp;

            hour = now.hour;
            minute = now.minute;
        }

        //		LED_SEGBlinkSet(LED_SEG_BLINK_DOT);
    }
}

/****************************************************************************************
 * ????:GasStove_DisTiming()
 * ????: ????????,?5S???????
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void GasStove_DisTiming(void)
{
    BOOL convert = TRUE;
    uint32 delay = GAS_STOVE_DIS_SWITCH_TIME;
    static uint32 uiLastTiming = 0xFF;

    if (!uiGasStove_Timing)
    {
        delay = GAS_STOVE_DIS_SWITCH_SHORT_TIME;

        a_ucGasStove_LEDTimeBuffer[0] = 10;
        a_ucGasStove_LEDTimeBuffer[1] = 0;
        a_ucGasStove_LEDTimeBuffer[2] = 0;
        a_ucGasStove_LEDTimeBuffer[3] = 0;

        uiLastTiming = uiGasStove_Timing;
    }

    if (!BSP_OS_Timeout(uiGasStove_DisDly, delay))
    {
        convert = FALSE;

        if (uiGasStove_Timing >= 180)
            uiGasStove_Timing = 179;

        if (uiGasStove_Timing != uiLastTiming)
        {
            a_ucGasStove_LEDTimeBuffer[0] = 10;
            a_ucGasStove_LEDTimeBuffer[1] = uiGasStove_Timing / 60;
            a_ucGasStove_LEDTimeBuffer[2] = (uiGasStove_Timing % 60) / 10;
            a_ucGasStove_LEDTimeBuffer[3] = (uiGasStove_Timing % 60) % 10;

            uiLastTiming = uiGasStove_Timing;
        }
    }

    if (convert)
    {
        uiGasStove_DisDly = BSP_OS_TimeGet();
        ucGasStove_DisplayState = GAS_STOVE_DIS_RTC;

        LED_SEG_Update(a_ucGasStove_LEDRTCBuffer);
        LED_SEGBlinkSet(LED_SEG_BLINK_DOT);
    }
}

/****************************************************************************************
 * ????:GasStove_DisTiming()
 * ????: ??????????
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void GasStove_DisSet(void)
{
    if (bGasStove_TimingSetEn)
    {
        if (BSP_OS_Timeout(uiGasStove_TimingEnDly, GAS_STOVE_EDIT_ENSURE_TIME))
            SettingTiming_Ensure();
    }

    // 匹配ID 匹配一次a_ucGasStove_LEDIdMatchBuffer[3] = 1
    if (bSetting_SysIdMatchState)
    {
        if (Cooker_SysIdMatchResult())
        {
            a_ucGasStove_LEDIdMatchBuffer[0] = 10;
            a_ucGasStove_LEDIdMatchBuffer[1] = 10;
            a_ucGasStove_LEDIdMatchBuffer[2] = 10;
            a_ucGasStove_LEDIdMatchBuffer[3] = 1;

            bSetting_SysIdMatchState = FALSE;
        }
        else
        {
            if (BSP_OS_Timeout(uiCooker_MatchIdWaitDly, 5000))
            {
                a_ucGasStove_LEDIdMatchBuffer[0] = 10;
                a_ucGasStove_LEDIdMatchBuffer[1] = 10;
                a_ucGasStove_LEDIdMatchBuffer[2] = 10;
                a_ucGasStove_LEDIdMatchBuffer[3] = 10;

                bSetting_SysIdMatchState = FALSE;
            }
        }
    }
}

/****************************************************************************************
 * ????:GasStove_DisSettingRTC()
 * ????: ??????,???????,??????
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void GasStove_DisSettingRTC(void)
{
    LED_SEGBlinkSet(GAS_STOVE_BLINK_SET_RTC);
    LED_SEG_Update(a_ucGasStove_LEDRTCBuffer);
}

/****************************************************************************************
 * ????:GasStove_DisSettingRTC()
 * ????:
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void SettingRTC_AddHour(void)
{
    uint8 hour;

    LED_SEGBlinkSet(GAS_STOVE_BLINK_SET_RTC);

    hour = a_ucGasStove_LEDRTCBuffer[0] * 10;
    hour += a_ucGasStove_LEDRTCBuffer[1];

    if (++hour >= 24)
        hour = 0;

    a_ucGasStove_LEDRTCBuffer[0] = hour / 10;
    a_ucGasStove_LEDRTCBuffer[1] = hour % 10;
}

/****************************************************************************************
 * ????:SettingRTC_AddTenMinute()
 * ????:
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void SettingRTC_AddTenMinute(void)
{
    LED_SEGBlinkSet(GAS_STOVE_BLINK_SET_RTC);

    if (++a_ucGasStove_LEDRTCBuffer[2] > 5)
        a_ucGasStove_LEDRTCBuffer[2] = 0;
}

/****************************************************************************************
 * ????:SettingRTC_AddMinute()
 * ????:
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void SettingRTC_AddMinute(void)
{
    LED_SEGBlinkSet(GAS_STOVE_BLINK_SET_RTC);

    if (++a_ucGasStove_LEDRTCBuffer[3] > 9)
        a_ucGasStove_LEDRTCBuffer[3] = 0;
}

/****************************************************************************************
 * ????:SettingRTC_Ensure()
 * ????:
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void SettingRTC_Ensure(void)
{
    RTCTime rtc;

    get_rtc(&rtc);
    if (!CheckRTCRight(&rtc))
    {
        rtc.year = 17;
        rtc.month = 10;
        rtc.year = 22;

        rtc.second = 0;
    }

    rtc.hour = a_ucGasStove_LEDRTCBuffer[0] * 10 + a_ucGasStove_LEDRTCBuffer[1];
    rtc.minute = a_ucGasStove_LEDRTCBuffer[2] * 10 + a_ucGasStove_LEDRTCBuffer[3];

    rtc.hour = (rtc.hour > 23) ? 23 : rtc.hour;
    rtc.minute = (rtc.minute > 59) ? 59 : rtc.minute;

    set_rtc(&rtc);

    // LED_SEGBlinkSet(GAS_STOVE_BLINK_NULL);
    // LED_SEG_Update(a_ucGasStove_LEDRTCBuffer);
    //
    // ucGasStove_DisplayState = GAS_STOVE_DIS_RTC;
    // uiGasStove_DisDly       = BSP_OS_TimeGet();
    //
    // ucPressCnt = 0;
    // Led_DisplayOff(LED_DIS_SET_RTC | LED_DIS_SET_START_TIME | LED_DIS_SET_END_TIME | LED_DIS_SET_SLAVE_CODE);

    GasStove_ExtEnsure();
}

/****************************************************************************************
 * ????:GasStove_DisSettingStartTime()
 * ????: ??????
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void GasStove_DisSettingStartTime(void)
{
    uint8 hour, ten_minute, minute;

    hour = tGasStove_Para.usStartTime / 60;
    ten_minute = (tGasStove_Para.usStartTime % 60) / 10;
    minute = tGasStove_Para.usStartTime % 10;

    a_ucGasStove_LEDParaTimeBuffer[0] = 10;
    a_ucGasStove_LEDParaTimeBuffer[1] = hour;
    a_ucGasStove_LEDParaTimeBuffer[2] = ten_minute;
    a_ucGasStove_LEDParaTimeBuffer[3] = minute;

    LED_SEGBlinkSet(GAS_STOVE_BLINK_SET_START);
    LED_SEG_Update(a_ucGasStove_LEDParaTimeBuffer);
}

/****************************************************************************************
 * ????:GasStove_DisSettingEndTime()
 * ????: ??????
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void GasStove_DisSettingEndTime(void)
{
    uint8 hour, ten_minute, minute;

    hour = tGasStove_Para.usEndTime / 60;
    ten_minute = (tGasStove_Para.usEndTime % 60) / 10;
    minute = tGasStove_Para.usEndTime % 10;

    a_ucGasStove_LEDParaTimeBuffer[0] = 10;
    a_ucGasStove_LEDParaTimeBuffer[1] = hour;
    a_ucGasStove_LEDParaTimeBuffer[2] = ten_minute;
    a_ucGasStove_LEDParaTimeBuffer[3] = minute;

    LED_SEGBlinkSet(GAS_STOVE_BLINK_SET_END);
    LED_SEG_Update(a_ucGasStove_LEDParaTimeBuffer);
}

/****************************************************************************************
 * ????:SettingStartTime_Hour()
 * ????: ????????
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void SettingStartTime_Hour(void)
{
    LED_SEGBlinkSet(GAS_STOVE_BLINK_SET_START);
    LED_SEG_Update(a_ucGasStove_LEDParaTimeBuffer);

    if (++a_ucGasStove_LEDParaTimeBuffer[1] > 2)
        a_ucGasStove_LEDParaTimeBuffer[1] = 0;
}

/****************************************************************************************
 * ????:SettingStartTime_TenMinute()
 * ????: ????????
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void SettingStartTime_TenMinute(void)
{
    LED_SEGBlinkSet(GAS_STOVE_BLINK_SET_START);
    LED_SEG_Update(a_ucGasStove_LEDParaTimeBuffer);

    if (++a_ucGasStove_LEDParaTimeBuffer[2] > 5)
        a_ucGasStove_LEDParaTimeBuffer[2] = 0;
}

/****************************************************************************************
 * ????:SettingStartTime_Minute()
 * ????: ?????????
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void SettingStartTime_Minute(void)
{
    LED_SEGBlinkSet(GAS_STOVE_BLINK_SET_START);
    LED_SEG_Update(a_ucGasStove_LEDParaTimeBuffer);

    if (++a_ucGasStove_LEDParaTimeBuffer[3] > 9)
        a_ucGasStove_LEDParaTimeBuffer[3] = 0;
}

/****************************************************************************************
 * ????:SettingStartTime_Ensure()
 * ????: ????????
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void SettingStartTime_Ensure(void)
{
    GasStove_Para_t para;

    para.usStartTime = a_ucGasStove_LEDParaTimeBuffer[1] * 60;
    para.usStartTime += a_ucGasStove_LEDParaTimeBuffer[2] * 10;
    para.usStartTime += a_ucGasStove_LEDParaTimeBuffer[3];

    if (tGasStove_Para.usStartTime != para.usStartTime)
    {
        para.usEndTime = tGasStove_Para.usEndTime;
        para.check = crc32_ethernet_block((uint8 *)&para, sizeof(GasStove_Para_t) - 4);

        if (STM32Fxx_FlashPageWrite((uint8 *)&para, GAS_STOVE_STORAGE_PARA_ADDR, sizeof(GasStove_Para_t)))
        {
            tGasStove_Para.usStartTime = para.usStartTime;
        }
    }

    // LED_SEGBlinkSet(GAS_STOVE_BLINK_NULL);
    // LED_SEG_Update(a_ucGasStove_LEDRTCBuffer);
    //
    // ucGasStove_DisplayState = GAS_STOVE_DIS_RTC;
    // uiGasStove_DisDly       = BSP_OS_TimeGet();
    //
    // ucPressCnt = 0;
    // Led_DisplayOff(LED_DIS_SET_RTC | LED_DIS_SET_START_TIME | LED_DIS_SET_END_TIME | LED_DIS_SET_SLAVE_CODE);

    GasStove_ExtEnsure();
}

/****************************************************************************************
 * ????:SettingEndTime_Ensure()
 * ????: ????????
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void SettingEndTime_Ensure(void)
{
    GasStove_Para_t para;

    para.usEndTime = a_ucGasStove_LEDParaTimeBuffer[1] * 60;
    para.usEndTime += a_ucGasStove_LEDParaTimeBuffer[2] * 10;
    para.usEndTime += a_ucGasStove_LEDParaTimeBuffer[3];

    if (tGasStove_Para.usEndTime != para.usEndTime)
    {
        para.usStartTime = tGasStove_Para.usStartTime;
        para.check = crc32_ethernet_block((uint8 *)&para, sizeof(GasStove_Para_t) - 4);

        if (STM32Fxx_FlashPageWrite((uint8 *)&para, GAS_STOVE_STORAGE_PARA_ADDR, sizeof(GasStove_Para_t)))
        {
            tGasStove_Para.usEndTime = para.usEndTime;
        }
    }

    // LED_SEGBlinkSet(GAS_STOVE_BLINK_NULL);
    // LED_SEG_Update(a_ucGasStove_LEDRTCBuffer);

    // ucGasStove_DisplayState = GAS_STOVE_DIS_RTC;
    // uiGasStove_DisDly       = BSP_OS_TimeGet();
    //
    // ucPressCnt = 0;
    // Led_DisplayOff(LED_DIS_SET_RTC | LED_DIS_SET_START_TIME | LED_DIS_SET_END_TIME | LED_DIS_SET_SLAVE_CODE);

    GasStove_ExtEnsure();
}

/****************************************************************************************
 * ????:SettingTiming_Cancle()
 * ????:
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void SettingTiming_Cancle(void)
{
    uiGasStove_Timing = 0;

    SysTick_FunReg(NULL);
    bGasStove_TimingEnable = FALSE;

    LED_SEGBlinkSet(GAS_STOVE_BLINK_NULL);
    LED_SEG_Update(a_ucGasStove_LEDTimeBuffer);

    ucGasStove_DisplayState = GAS_STOVE_DIS_TIMING;
    uiGasStove_DisDly = BSP_OS_TimeGet();
}

/****************************************************************************************
 * ????:SettingTiming_Ensure()
 * ????:
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void SettingTiming_Ensure(void)
{
    uiGasStove_Timing = a_ucGasStove_LEDTimeBuffer[1] * 60;
    uiGasStove_Timing += a_ucGasStove_LEDTimeBuffer[2] * 10;
    uiGasStove_Timing += a_ucGasStove_LEDTimeBuffer[3];

    if (uiGasStove_Timing)
    {
        SysTick_FunReg(GasStove_TimingIRQHandler);
        bGasStove_TimingEnable = TRUE;
    }

    LED_SEGBlinkSet(GAS_STOVE_BLINK_NULL);
    LED_SEG_Update(a_ucGasStove_LEDTimeBuffer);

    ucGasStove_DisplayState = GAS_STOVE_DIS_TIMING;
    uiGasStove_DisDly = BSP_OS_TimeGet();

    KeyBoard_FunReg(3, SettingTiming_Cancle);

    bGasStove_TimingSetEn = FALSE;
}

/****************************************************************************************
 * ????:SettingTiming_Hour()
 * ????:
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void SettingTiming_Hour(void)
{
    a_ucGasStove_LEDTimeBuffer[0] = 10;

    LED_SEGBlinkSet(LED_SEG_BLINK_DOT_AND_ONE);
    LED_SEG_Update(a_ucGasStove_LEDTimeBuffer);

    if (++a_ucGasStove_LEDTimeBuffer[1] > 2)
        a_ucGasStove_LEDTimeBuffer[1] = 0;

    bGasStove_TimingEnable = FALSE;
    KeyBoard_FunReg(3, SettingTiming_Ensure);

    ucGasStove_DisplayState = GAS_STOVE_DIS_SET;

    bGasStove_TimingSetEn = TRUE;
    uiGasStove_TimingEnDly = BSP_OS_TimeGet();
}

/****************************************************************************************
 * ????:SettingTiming_TenMinute()
 * ????:
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void SettingTiming_TenMinute(void)
{
    a_ucGasStove_LEDTimeBuffer[0] = 10;

    LED_SEGBlinkSet(LED_SEG_BLINK_DOT_AND_ONE);
    LED_SEG_Update(a_ucGasStove_LEDTimeBuffer);

    if (++a_ucGasStove_LEDTimeBuffer[2] > 5)
        a_ucGasStove_LEDTimeBuffer[2] = 0;

    bGasStove_TimingEnable = FALSE;
    KeyBoard_FunReg(3, SettingTiming_Ensure);

    ucGasStove_DisplayState = GAS_STOVE_DIS_SET;

    bGasStove_TimingSetEn = TRUE;
    uiGasStove_TimingEnDly = BSP_OS_TimeGet();
}

/****************************************************************************************
 * ????:SettingTiming_Minute()
 * ????:
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void SettingTiming_Minute(void)
{
    a_ucGasStove_LEDTimeBuffer[0] = 10;

    LED_SEGBlinkSet(LED_SEG_BLINK_DOT_AND_ONE);
    LED_SEG_Update(a_ucGasStove_LEDTimeBuffer);

    if (++a_ucGasStove_LEDTimeBuffer[3] > 9)
        a_ucGasStove_LEDTimeBuffer[3] = 0;

    bGasStove_TimingEnable = FALSE;
    KeyBoard_FunReg(3, SettingTiming_Ensure);

    ucGasStove_DisplayState = GAS_STOVE_DIS_SET;

    bGasStove_TimingSetEn = TRUE;
    uiGasStove_TimingEnDly = BSP_OS_TimeGet();
}

/****************************************************************************************
 * ????:
 * ????:
 * ????:
 * ???  :
 * ??    :
 * ????:2018-1-8 23:11
 ****************************************************************************************/
static void GasStove_ExtEnsure(void)
{
    if (1 == ucPressCnt)
    {
        GasStove_DisSettingStartTime();

        KeyBoard_FunReg(0, SettingStartTime_Hour);
        KeyBoard_FunReg(1, SettingStartTime_TenMinute);
        KeyBoard_FunReg(2, SettingStartTime_Minute);
        KeyBoard_FunReg(3, SettingStartTime_Ensure);

        ucGasStove_DisplayState = GAS_STOVE_DIS_SET;

        Led_DisplayOff(LED_DIS_SET_RTC | LED_DIS_SET_END_TIME | LED_DIS_SET_SLAVE_CODE);
        Led_DisplayOn(LED_DIS_SET_START_TIME);
    }
    else if (2 == ucPressCnt)
    {
        GasStove_DisSettingEndTime();

        KeyBoard_FunReg(0, SettingStartTime_Hour);
        KeyBoard_FunReg(1, SettingStartTime_TenMinute);
        KeyBoard_FunReg(2, SettingStartTime_Minute);
        KeyBoard_FunReg(3, SettingEndTime_Ensure);

        ucGasStove_DisplayState = GAS_STOVE_DIS_SET;

        Led_DisplayOff(LED_DIS_SET_RTC | LED_DIS_SET_START_TIME | LED_DIS_SET_SLAVE_CODE);
        Led_DisplayOn(LED_DIS_SET_END_TIME);
    }
    else if (3 == ucPressCnt)
    {
        Setting_SysIdMatch();

        KeyBoard_FunReg(0, NULL);
        KeyBoard_FunReg(1, NULL);
        KeyBoard_FunReg(2, Setting_SysIdMatchStart);
        KeyBoard_FunReg(3, Setting_SysIdMatchEnsure);

        ucGasStove_DisplayState = GAS_STOVE_DIS_SET;

        Led_DisplayOff(LED_DIS_SET_RTC | LED_DIS_SET_START_TIME | LED_DIS_SET_END_TIME);
        Led_DisplayOn(LED_DIS_SET_SLAVE_CODE);
    }
    else
    {
        LED_SEGBlinkSet(LED_SEG_BLINK_DOT);
        LED_SEG_Update(a_ucGasStove_LEDRTCBuffer);

        ucGasStove_DisplayState = GAS_STOVE_DIS_RTC;

        KeyBoard_FunReg(0, SettingTiming_Hour);
        KeyBoard_FunReg(1, SettingTiming_TenMinute);
        KeyBoard_FunReg(2, SettingTiming_Minute);
        KeyBoard_FunReg(3, SettingTiming_Cancle);

        Led_DisplayOff(LED_DIS_SET_RTC | LED_DIS_SET_START_TIME | LED_DIS_SET_END_TIME | LED_DIS_SET_SLAVE_CODE);
    }

    ucPressCnt++;
}

/****************************************************************************************
 * ????:Setting_SysIdMatch()
 * ????: ??????????
 * ????:
 * ???  :
 * ??    :
 * ????:2017-11-19 23:9
 ****************************************************************************************/
static void Setting_SysIdMatch(void)
{
    a_ucGasStove_LEDIdMatchBuffer[0] = 10;
    a_ucGasStove_LEDIdMatchBuffer[1] = 10;
    a_ucGasStove_LEDIdMatchBuffer[2] = 10;
    a_ucGasStove_LEDIdMatchBuffer[3] = 0;

    bSetting_SysIdMatchState = FALSE; //?????????

    LED_SEGBlinkSet(GAS_STOVE_BLINK_RTC_MINUTE);
    LED_SEG_Update(a_ucGasStove_LEDIdMatchBuffer);
}

/****************************************************************************************
 * ????:Setting_SysIdMatchEnsure()
 * ????: ?????????????,?????????
 * ????:
 * ???  :
 * ??    :
 * ????:2017-11-19 23:9
 ****************************************************************************************/
static void Setting_SysIdMatchStart(void)
{
    if (!bSetting_SysIdMatchState)
    {
        // 只允许匹配一次
        if (1 != a_ucGasStove_LEDIdMatchBuffer[3])
        {
            a_ucGasStove_LEDIdMatchBuffer[0] = 10;
            a_ucGasStove_LEDIdMatchBuffer[1] = 10;
            a_ucGasStove_LEDIdMatchBuffer[2] = 10;
            a_ucGasStove_LEDIdMatchBuffer[3] = 0;

            bSetting_SysIdMatchState = TRUE;

            Cooker_SysIdMatch();

            uiCooker_MatchIdWaitDly = BSP_OS_TimeGet();
            LED_SEGBlinkSet(GAS_STOVE_BLINK_NULL);
        }
    }
}

/****************************************************************************************
 * ????:Setting_SysIdMatchEnsure()
 * ????: ?????????????,?????????
 * ????:
 * ???  :
 * ??    :
 * ????:2017-11-19 23:9
 ****************************************************************************************/
static void Setting_SysIdMatchEnsure(void)
{
    if (!bSetting_SysIdMatchState)
    {
        // if (1 != a_ucGasStove_LEDIdMatchBuffer[3])
        //{
        //	a_ucGasStove_LEDIdMatchBuffer[0] = 10;
        //	a_ucGasStove_LEDIdMatchBuffer[1] = 10;
        //	a_ucGasStove_LEDIdMatchBuffer[2] = 10;
        //	a_ucGasStove_LEDIdMatchBuffer[3] = 0;

        //	bSetting_SysIdMatchState = TRUE;

        //	Cooker_SysIdMatch();

        //	uiCooker_MatchIdWaitDly = BSP_OS_TimeGet();
        //	LED_SEGBlinkSet(GAS_STOVE_BLINK_NULL);
        //}
        // else
        GasStove_ExtEnsure();
    }
}

/****************************************************************************************
 * ????:GasStove_DisSetButton()
 * ????: ????(???????)
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void GasStove_DisSetButton(void)
{
    if (ucPressCnt > GAS_STOVE_MAX_SET_FUN)
        ucPressCnt = 0;

    if (0 == ucPressCnt)
    {
        GasStove_DisSettingRTC();

        KeyBoard_FunReg(0, SettingRTC_AddHour);
        KeyBoard_FunReg(1, SettingRTC_AddTenMinute);
        KeyBoard_FunReg(2, SettingRTC_AddMinute);
        KeyBoard_FunReg(3, SettingRTC_Ensure);

        ucGasStove_DisplayState = GAS_STOVE_DIS_SET;

        Led_DisplayOff(LED_DIS_SET_START_TIME | LED_DIS_SET_END_TIME | LED_DIS_SET_SLAVE_CODE);
        Led_DisplayOn(LED_DIS_SET_RTC);
    }
    else if (1 == ucPressCnt)
    {
        GasStove_DisSettingStartTime();

        KeyBoard_FunReg(0, SettingStartTime_Hour);
        KeyBoard_FunReg(1, SettingStartTime_TenMinute);
        KeyBoard_FunReg(2, SettingStartTime_Minute);
        KeyBoard_FunReg(3, SettingStartTime_Ensure);

        ucGasStove_DisplayState = GAS_STOVE_DIS_SET;

        Led_DisplayOff(LED_DIS_SET_RTC | LED_DIS_SET_END_TIME | LED_DIS_SET_SLAVE_CODE);
        Led_DisplayOn(LED_DIS_SET_START_TIME);
    }
    else if (2 == ucPressCnt)
    {
        GasStove_DisSettingEndTime();

        KeyBoard_FunReg(0, SettingStartTime_Hour);
        KeyBoard_FunReg(1, SettingStartTime_TenMinute);
        KeyBoard_FunReg(2, SettingStartTime_Minute);
        KeyBoard_FunReg(3, SettingEndTime_Ensure);

        ucGasStove_DisplayState = GAS_STOVE_DIS_SET;

        Led_DisplayOff(LED_DIS_SET_RTC | LED_DIS_SET_START_TIME | LED_DIS_SET_SLAVE_CODE);
        Led_DisplayOn(LED_DIS_SET_END_TIME);
    }
    else if (3 == ucPressCnt)
    {
        Setting_SysIdMatch();

        KeyBoard_FunReg(0, NULL);
        KeyBoard_FunReg(1, NULL);
        // 对码：相当于将主机芯片ID发给从机  且只匹配成功一次
        KeyBoard_FunReg(2, Setting_SysIdMatchStart);
        KeyBoard_FunReg(3, Setting_SysIdMatchEnsure);

        ucGasStove_DisplayState = GAS_STOVE_DIS_SET;

        Led_DisplayOff(LED_DIS_SET_RTC | LED_DIS_SET_START_TIME | LED_DIS_SET_END_TIME);
        Led_DisplayOn(LED_DIS_SET_SLAVE_CODE);
    }
    //??????????

    else
    {
        LED_SEGBlinkSet(LED_SEG_BLINK_DOT);
        LED_SEG_Update(a_ucGasStove_LEDRTCBuffer);

        ucGasStove_DisplayState = GAS_STOVE_DIS_RTC;

        KeyBoard_FunReg(0, SettingTiming_Hour);
        KeyBoard_FunReg(1, SettingTiming_TenMinute);
        KeyBoard_FunReg(2, SettingTiming_Minute);
        KeyBoard_FunReg(3, SettingTiming_Cancle);

        Led_DisplayOff(LED_DIS_SET_RTC | LED_DIS_SET_START_TIME | LED_DIS_SET_END_TIME | LED_DIS_SET_SLAVE_CODE);
    }

    ucPressCnt++;
}

__weak BOOL GasStove_FireState(void) //??????
{
    return FALSE;
}

__weak BOOL GasStove_FanRotation(void) //??????
{
    return FALSE;
}

__weak void GasStove_Flameout(void) //????
{
}

__weak BOOL GasStove_GasSensor(void) //????
{
    return FALSE;
}

/****************************************************************************************
 * ????:GasStove_AlarmChk()
 * ????: ????
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void GasStove_AlarmChk(void)
{
    static BOOL bGasStove_FireStart = FALSE;
    static uint32 uiGasStove_FireStartDly = 0;
    static uint32 uiGasStove_EveryMinuteDly = 0;
    static uint16 usGasStove_EndMinuteCnt = 0;
    static uint16 usGasStove_LastMinuteCnt = 0xFFFF;

    static BOOL bGasAlarmingState = FALSE;
    static uint32 usGasAlarmingDly = 0;

    const ChordTone_Music_e a_eSingleMusicCnt[] =
        {
            eCHORD_TONE_MUSIC_1_SINGLE, //????
            eCHORD_TONE_MUSIC_1_SINGLE,
            eCHORD_TONE_MUSIC_2_SINGLE,
            eCHORD_TONE_MUSIC_3_SINGLE,
            eCHORD_TONE_MUSIC_4_SINGLE,
            eCHORD_TONE_MUSIC_5_SINGLE,
            eCHORD_TONE_MUSIC_6_SINGLE,
            eCHORD_TONE_MUSIC_7_SINGLE,
            eCHORD_TONE_MUSIC_8_SINGLE,
            eCHORD_TONE_MUSIC_9_SINGLE,
            eCHORD_TONE_MUSIC_10_SINGLE,
            eCHORD_TONE_MUSIC_11_SINGLE,
        };
    const uint16 usSingMusicMaxCnt = sizeof(a_eSingleMusicCnt) / sizeof(ChordTone_Music_e);
    uint16 minute;
    // 是否存在人的状态改变
    // 根据状态量进行改变
    if (GasStove_FanRotation())
        Led_DisplayOn(LED_DIS_FAN_ROTATION);
    else
        Led_DisplayOff(LED_DIS_FAN_ROTATION);

    if (uiGasStove_Timing)
        return;

    if (!bGasStove_FireStart)
    {
        if (!GasStove_FanRotation())
        {
            if (bGasAlarmingState)
            {
                if (GasStove_FireState())
                {
                    // 煤气报警时间
                    if (BSP_OS_Timeout(usGasAlarmingDly, GAS_STOVE_ALARM_ENSURE_TIME)) //??20S????????,??
                    {
                        bGasAlarmingState = FALSE;

                        GasStove_Flameout();
                    }
                }
                else
                    bGasAlarmingState = FALSE; //??????,???????
            }
            else
            {
                bGasStove_FireStart = TRUE;
                uiGasStove_FireStartDly = BSP_OS_TimeGet();

                usGasStove_EndMinuteCnt = 0;
                usGasStove_LastMinuteCnt = 0xFFFF;
            }
        }
        else
        {
            if (bGasAlarmingState) //?????????,??????
                bGasAlarmingState = FALSE;
        }
    }
    else
    {
        if ((!GasStove_FireState()) || (GasStove_FanRotation()))
        {
            bGasStove_FireStart = FALSE;
            return;
        }

        if (BSP_OS_Timeout(uiGasStove_FireStartDly, tGasStove_Para.usStartTime * GAS_STOVE_TIMING_1_MINUTE))
        {
            if (usGasStove_LastMinuteCnt != usGasStove_EndMinuteCnt)
            {
                if (0xFFFF == usGasStove_LastMinuteCnt)
                {
                    //????
                    //                    ChordTone_Start(eCHORD_TONE_MUSIC_1_SINGLE);

                    minute = (tGasStove_Para.usStartTime >= usSingMusicMaxCnt) ? usSingMusicMaxCnt : tGasStove_Para.usStartTime;
                    ChordTone_Start(a_eSingleMusicCnt[minute]);

                    usGasStove_EndMinuteCnt = tGasStove_Para.usStartTime;
                    uiGasStove_EveryMinuteDly = BSP_OS_TimeGet();
                }

                usGasStove_LastMinuteCnt = usGasStove_EndMinuteCnt;
            }

            if (BSP_OS_Timeout(uiGasStove_EveryMinuteDly, GAS_STOVE_TIMING_1_MINUTE))
            {
                usGasStove_EndMinuteCnt++;

                if (usGasStove_EndMinuteCnt < tGasStove_Para.usEndTime)
                {
                    minute = (usGasStove_EndMinuteCnt >= usSingMusicMaxCnt) ? usSingMusicMaxCnt : usGasStove_EndMinuteCnt;
                    ChordTone_Start(a_eSingleMusicCnt[minute]);
                }

                uiGasStove_EveryMinuteDly = BSP_OS_TimeGet();
            }

            if (usGasStove_EndMinuteCnt >= tGasStove_Para.usEndTime)
            {
                //???20S
                ChordTone_Start(eCHORD_TONE_MUSIC_LONG_SINGLE);

                bGasStove_FireStart = FALSE;

                bGasAlarmingState = TRUE;
                usGasAlarmingDly = BSP_OS_TimeGet();
            }
        }
    }
}

/****************************************************************************************
 * ????:GasStove_GasSensorAlarmChk()
 * ????:
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void GasStove_GasSensorAlarmCancle(void)
{
    ChordTone_Start(eCHORD_TONE_MUSIC_OFF);
    KeyBoard_FunReg(3, SettingTiming_Ensure);
}

/****************************************************************************************
 * ????:GasStove_GasSensorAlarmChk()
 * ????:
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
static void GasStove_GasSensorAlarmChk(void)
{
    //    GAS_STOVE_ALARM_ENSURE_TIME
    //    static uint32 uiGasStove_AlarmDly;
    //    static BOOL   bGasStove_State = FALSE;

    if (bGasStove_GasSensorState)
    {
        if (GasStove_GasSensor())
        {
            if (BSP_OS_Timeout(uiGasStove_GasSensorDly, GAS_STOVE_GAS_SENSOR_ALARM_INTERVAL))
            {
                GasStove_Flameout();

                //???20S
                ChordTone_Start(eCHORD_TONE_MUSIC_LONG_SINGLE);
                Led_DisplayOn(LED_DIS_GAS_LEAK);

                uiGasStove_GasSensorDly = BSP_OS_TimeGet();

                KeyBoard_FunReg(3, GasStove_GasSensorAlarmCancle);
            }
        }
        else
        {
            //			ChordTone_Start(eCHORD_TONE_MUSIC_OFF);
            Led_DisplayOff(LED_DIS_GAS_LEAK);
            bGasStove_GasSensorState = FALSE;
        }
    }
    else
    {
        if (GasStove_GasSensor())
        {
            GasStove_Flameout();
            //???20S
            ChordTone_Start(eCHORD_TONE_MUSIC_LONG_SINGLE);
            Led_DisplayOn(LED_DIS_GAS_LEAK);

            uiGasStove_GasSensorDly = BSP_OS_TimeGet();

            bGasStove_GasSensorState = TRUE;

            KeyBoard_FunReg(3, GasStove_GasSensorAlarmCancle);
        }
        else
        {
            //			ChordTone_Start(eCHORD_TONE_MUSIC_OFF);
            Led_DisplayOff(LED_DIS_GAS_LEAK);
        }
    }
}

/****************************************************************************************
 * ????:GasStove_DisplayService()
 * ????:
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
void GasStove_DisplayService(void)
{
    static BOOL bTimingOutState = FALSE;
    static uint32 usTimingOutDly;

    if (ucGasStove_DisplayState > GAS_STOVE_DIS_SET)
        ucGasStove_DisplayState = GAS_STOVE_DIS_RTC;
    // 显示时间设置
    a_pfnGasStove_Service[ucGasStove_DisplayState]();

    if (bGasStove_TimingEnable)
    {
        if (!uiGasStove_Timing)
        {

            //???20S
            ChordTone_Start(eCHORD_TONE_MUSIC_LONG_SINGLE);

            bGasStove_TimingEnable = FALSE;
            SysTick_FunReg(NULL);

            bTimingOutState = TRUE;
            usTimingOutDly = BSP_OS_TimeGet();
        }
    }
    else
        SysTick_FunReg(NULL);

    if (bTimingOutState)
    {
        if (BSP_OS_Timeout(usTimingOutDly, GAS_STOVE_ALARM_ENSURE_TIME))
        {
            bTimingOutState = FALSE;
            GasStove_Flameout();
        }
//        else
//        {
//            if (GasStove_FanRotation())
//						{
//                bTimingOutState = FALSE;
//						}
//        }
    }
		

    GasStove_AlarmChk();

    GasStove_GasSensorAlarmChk();
}

/****************************************************************************************
 * ????:GasStove_Init()
 * ????:
 * ????:
 * ???  :
 * ??    :
 * ????:2017-10-6 21:48
 ****************************************************************************************/
void GasStove_Init(void)
{
    BOOL ret = FALSE;

    KeyBoard_FunReg(0, SettingTiming_Hour);
    KeyBoard_FunReg(1, SettingTiming_TenMinute);
    KeyBoard_FunReg(2, SettingTiming_Minute);
    KeyBoard_FunReg(3, SettingTiming_Cancle);

    KeyBoard_FunReg(4, GasStove_DisSetButton);

    if (STM32Fxxx_FlashRead((uint8 *)&tGasStove_Para, GAS_STOVE_STORAGE_PARA_ADDR, sizeof(GasStove_Para_t)))
    {
        if (tGasStove_Para.check == crc32_ethernet_block((uint8 *)&tGasStove_Para, sizeof(GasStove_Para_t) - 4))
            ret = TRUE;
    }

    if (!ret)
    {
        tGasStove_Para.usStartTime = GAS_STOVE_DEF_START_TIME;
        tGasStove_Para.usEndTime = GAS_STOVE_DEF_END_TIME;
    }

    uiGasStove_DisDly = BSP_OS_TimeGet();
    ucGasStove_DisplayState = GAS_STOVE_DIS_RTC;
    LED_SEGBlinkSet(LED_SEG_BLINK_DOT);

    LED_SEG_Update(a_ucGasStove_LEDRTCBuffer);
}

/*******************************????(c)**************END OF FILE********************/
