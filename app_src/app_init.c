/*
 * Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!=================================================================================================
 \file       app_init.c
 \brief      This is a public source file for the initial system startup module. It contains
 the implementation of the interface functions.
 ==================================================================================================*/

/*==================================================================================================
 Include Files
 ==================================================================================================*/
#include "fsl_device_registers.h"
#include "fsl_os_abstraction.h"
#include "fsl_port.h"
#include "pin_mux.h"

#if WDOG_ENABLE
    #include "fsl_wdog.h"
#endif

#ifndef FRDM_K64F_KW41Z
    #include "PhyInterface.h"
    #include "MacInterface.h"
#endif

/* FSL Framework */
#include "LED.h"
#include "TimersManager.h"
#include "Keyboard.h"
#include "SerialManager.h"
#include "Panic.h"

#include "fsl_os_abstraction.h"
#include "app_init.h"
#include "app_config.h"
#include "thread_network.h"
#include "thread_cfg.h"
#include "debug_log.h"

#include "ble_mk64f.h"
#include "thread_volansys.h"

#if THREAD_USE_THCI
    #include "thci.h"
    #include "FsciInterface.h"
#endif


#ifdef FRDM_K64F_KW41Z
    #include "FsciInterface.h"
    #include "cmd_threadip.h"

    #if gHybridApp_d
        #include "cmd_ble.h"
    #endif

#endif

/* Display */
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "GUI.h"
#include "MULTIPAGE.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "emwin_support.h"

#include "core.h"
#include "battery_mk64f.h"

extern void APP_Init(void);
extern void APP_Handler(void);

bool isInitComplete = false;

#if gLpmIncluded_d
    extern void App_SedWakeUpFromKeyBoard(void);
#endif

#if THCI_USB_ENABLE && THR_SOFTWARE_RESET_ENABLE
    extern void THCI_ResetCpuEvent(resetCpuStatus_t resetStatus, uint32_t timeoutUs);
#endif

/*==================================================================================================
 Private macros
 ==================================================================================================*/
#define gFSCI_IpStackOpGCnf_c                   0xCFU
/*! FSCI utility Confirmations/Indications              */
#define gFSCI_CnfOpcodeGroup_c                  0xA4
/*! FSCI operation group for GATT Database (application) */
#define gFsciBleL2capOpcodeGroup_c              0x41
/*! FSCI operation group for GATT */
#define gFsciBleGattOpcodeGroup_c               0x44
/*! FSCI operation group for GATT Database (application) */
#define gFsciBleGattDbAppOpcodeGroup_c          0x45
/*! FSCI operation group for GAP */
#define gFsciBleGapOpcodeGroup_c                0x47

/* ATTENTION: the following static configuration is not used, as DHCP is used instead */
/* IP address configuration. */
#define configIP_ADDR0 192
#define configIP_ADDR1 168
#define configIP_ADDR2 0
#define configIP_ADDR3 142

/* Netmask configuration. */
#define configNET_MASK0 255
#define configNET_MASK1 255
#define configNET_MASK2 255
#define configNET_MASK3 0

/* Gateway address configuration. */
#define configGW_ADDR0 192
#define configGW_ADDR1 168
#define configGW_ADDR2 0
#define configGW_ADDR3 1

#define TEST_VERSION				"5"


/*==================================================================================================
 Private type definitions
 ==================================================================================================*/

/*==================================================================================================
 Private prototypes
 ==================================================================================================*/

static void APP_HandleMcuResetOnIdle(void);
#if gLpmIncluded_d
    static void APP_HandleLowPowerOnIdle(void);
#endif
#if WDOG_ENABLE
    static void APP_WDOG_Init(void);
    static void APP_WDOG_Refresh(void);
#endif
/*==================================================================================================
 Private global variables declarations
 ==================================================================================================*/

void (*pfAppKeyboardHandler)(void *) = NULL;

#if WDOG_ENABLE
/* Configure watchdog. */
const wdog_config_t wdogConfig =
{
    .enableWdog = TRUE, /* Watchdog mode */
    .timeoutValue = 0x4096U, /* Watchdog overflow time is about 4s*/
    .enableWindowMode = FALSE, /* Disable window function */
    .windowValue = 0, /* Watchdog window value */
    .prescaler = kWDOG_ClockPrescalerDivide1, /* Watchdog clock prescaler */
    .enableUpdate = TRUE, /* Update register enabled */
    .clockSource = kWDOG_LpoClockSource, /* Watchdog clock source is LPO 1KHz */
#if defined(FSL_FEATURE_WDOG_HAS_WAITEN) && FSL_FEATURE_WDOG_HAS_WAITEN
    .workMode.enableWait = TRUE, /* Enable watchdog in wait mode */
#endif
    .workMode.enableStop = FALSE, /* Enable watchdog in stop mode */
    .workMode.enableDebug = FALSE, /* Disable watchdog in debug mode */
};

static WDOG_Type *wdog_base = WDOG;
#endif

/*!< reset MCU timestamp <microseconds> */
static uint64_t gSwResetTimestamp = 0;
/*!< boolean -  nvm format */
static bool_t gResetToFactory = FALSE;

/*==================================================================================================
 Public global variables declarations
 ==================================================================================================*/

taskMsgQueue_t appThreadMsgQueue;

/*==================================================================================================
 Public functions
 ==================================================================================================*/
/*==================================================================================================
 ==================================================================================================*/


/* Functions required for emwin / Display */
uint32_t DSPI2_GetFreq(void)
{
    return CLOCK_GetBusClkFreq();
}

EventGroupHandle_t executeTickEvent;

// One minute
const TickType_t executeTickWaitTime = 60000 / portTICK_PERIOD_MS;

//void aFunction( EventGroupHandle_t xEventGroup )
//{
//EventBits_t uxBits;
//const TickType_t xTicksToWait = 100 / portTICK_PERIOD_MS;
//
//  /* Wait a maximum of 100ms for either bit 0 or bit 4 to be set within
//  the event group.  Clear the bits before exiting. */
//  uxBits = xEventGroupWaitBits(
//            xEventGroup,   /* The event group being tested. */
//            BIT_0 | BIT_4, /* The bits within the event group to wait for. */
//            pdTRUE,        /* BIT_0 & BIT_4 should be cleared before returning. */
//            pdFALSE,       /* Don't wait for both bits, either bit will do. */
//            xTicksToWait );/* Wait a maximum of 100ms for either bit to be set. */
//
//  if( ( uxBits & ( BIT_0 | BIT_4 ) ) == ( BIT_0 | BIT_4 ) )
//  {
//      /* xEventGroupWaitBits() returned because both bits were set. */
//  }
//  else if( ( uxBits & BIT_0 ) != 0 )
//  {
//      /* xEventGroupWaitBits() returned because just BIT_0 was set. */
//  }
//  else if( ( uxBits & BIT_4 ) != 0 )
//  {
//      /* xEventGroupWaitBits() returned because just BIT_4 was set. */
//  }
//  else
//  {
//      /* xEventGroupWaitBits() returned because xTicksToWait ticks passed
//      without either BIT_0 or BIT_4 becoming set. */
//  }
//}

void main_task(uint32_t param)
{
    static uint8_t mainInitialized = FALSE;

    if (!mainInitialized)
    {
        mainInitialized = TRUE;
#if WDOG_ENABLE
        /* Init watchdog module */
        APP_WDOG_Init();
#endif
        /* Init memory blocks manager */
        MEM_Init();

        /* Init  timers module */
        TMR_Init();
        TMR_TimeStampInit();
        /* Init Led module */
        LED_Init();

        executeTickEvent = xEventGroupCreate();

        ATMO_Init();
    }


    /* Main Application Loop (idle state) */
    while (1)
    {
        /* Debug Checks, Leader LED restore check */
        DBG_Check();

        /* Reset MCU */
        APP_HandleMcuResetOnIdle();

        /* For BareMetal break the while(1) after 1 run */
        if (gUseRtos_c == 0)
        {
            break;
        }

        ATMO_Tick();

//        if(!isInitComplete)
//        {
//        	ATMO_Tick();
//        }
//
//        EventBits_t uxBits = xEventGroupWaitBits(executeTickEvent, 0x1, pdTRUE, pdFALSE, executeTickWaitTime);
//
//        if(uxBits & 0x1)
//        {
//        	while(ATMO_Tick() > 0){};
//        }

        ATMO_BATTERY_MK64F_Update();

        isInitComplete = true;
    }
}


/*!*************************************************************************************************
 \fn     APP_ResetMcuOnTimeout
 \brief  Reset the MCU on timeout
 \param  [in]    timeoutMs  timeout in milliseconds
 \param  [in]    resetToFactory
 \return         None
 ***************************************************************************************************/
void APP_ResetMcuOnTimeout(uint32_t timeoutMs, bool_t resetToFactory)
{
    gResetToFactory = resetToFactory;
    gSwResetTimestamp = TMR_GetTimestamp();
    gSwResetTimestamp += (timeoutMs * 1000); /* microseconds*/
}

/*!*************************************************************************************************
 \fn     APP_GetResetMcuTimeout
 \brief  Return the interval time until a MCU reset occurs
 \return  the time interval; 0 means that no Mcu reset was programmed
 ***************************************************************************************************/
uint32_t APP_GetResetMcuTimeout(void)
{
    uint32_t timeInterval = 0;

    if (gSwResetTimestamp > TMR_GetTimestamp())
    {
        timeInterval = (uint32_t)((gSwResetTimestamp - TMR_GetTimestamp())
                                  / 1000);
    }

    return timeInterval;
}



/*!*************************************************************************************************
 \fn     APP_HandleMcuResetOnIdle
 \brief  Reset the MCU on idle
 \param  [in]
 \return         None
 ***************************************************************************************************/
static void APP_HandleMcuResetOnIdle(void)
{
    if ((gSwResetTimestamp) && (gSwResetTimestamp < TMR_GetTimestamp()))
    {
        gSwResetTimestamp = 0;
        /* disable interrupts */
        OSA_InterruptDisable();

#if THCI_USB_ENABLE && THR_SOFTWARE_RESET_ENABLE
        THR_SoftwareReset(0, gResetToFactory);
        /* inform application */
        THCI_ResetCpuEvent(gResetCpuSuccess_c, 0);
#else

        if (gResetToFactory)
        {
            /* Erase NVM Datasets */
            //NvFormat();
        }

        ResetMCU();
#endif
        /* Enable interrupts */
        OSA_InterruptEnable();
    }
}

/*!*************************************************************************************************
 \fn     APP_HandleLowPowerOnIdle
 \brief  Handle low power on idle
 \param  [in]
 \return         None
 ***************************************************************************************************/
#if gLpmIncluded_d
static void APP_HandleLowPowerOnIdle(void)
{
    if (PWR_CheckIfDeviceCanGoToSleep())
    {
        PWRLib_WakeupReason_t wakeupReason;
        wakeupReason = PWR_EnterLowPower();

        if (wakeupReason.Bits.FromKeyBoard)
        {
            /* Protection to the LLWD pin enabled on both edges */
            static bool_t wakeUpFlag = FALSE;

            if (TRUE == wakeUpFlag)
            {
                wakeUpFlag = FALSE;
                App_SedWakeUpFromKeyBoard();
            }
            else
            {
                wakeUpFlag = TRUE;
            }

            PWR_AllowDeviceToSleep();
        }
    }
}
#endif

/*!*************************************************************************************************
 \fn     static void APP_WDOG_Init(void)
 \brief  Init watch dog if enabled
 ***************************************************************************************************/
#if WDOG_ENABLE
static void APP_WDOG_Init(void)
{

    uint32_t i = 0;

    WDOG_Init(wdog_base, &wdogConfig);

    /* Accessing register by bus clock */
    for (i = 0; i < 256; i++)
    {
        (void)WDOG->RSTCNT;
    }
}

/*!*************************************************************************************************
 \fn     static void APP_WDOG_Refresh(void)
 \brief  Refresh watch dog if enabled
 ***************************************************************************************************/

static void APP_WDOG_Refresh(void)
{
    uint32_t wdogTimer = (uint32_t)((((uint32_t)wdog_base->TMROUTH) << 16U) | (wdog_base->TMROUTL));

    /* Restart the watchdog so it doesn't reset */
    if (wdogTimer > (wdogConfig.timeoutValue >> 3U))
    {
        WDOG_Refresh(wdog_base);
    }
}
#endif
/*==================================================================================================
 Private debug functions
 ==================================================================================================*/
