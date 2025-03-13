
/**************** Includes ************/
#include <Arduino.h>
#include <Rte_Types.h>

#include "CompilerSwitch.h"

/**************** Defines ************/
#define RTE_DELAY_MS_FACTOR         16000U

/**************** Varables ************/
/// OS ///
uint8 Rte_TaskMgr_Handler_Flags;
//------//

/// Temperature ///
Rte_Temperature_Reading_Type Rte_Temperature_ReadedTemp;
uint8 Rte_Temperature_ActionFlags;
//---------------//

/// WebServer ///
uint8 Rte_WiFiAccPoint_Status;
//---------------//

/// ColdSmokeAppl ///
uint8 Rte_ColdSmokeAppl_TargetTemp;
//---------------//

/**************** Functions ************/
void Rte_Init(void)
{
    /// OS ///
    Rte_TaskMgr_Handler_Flags = 0U;
    //------//

    /// WebServer ///
    Rte_WiFiAccPoint_Status = 0U;
    //------//

    /// ColdSmokeAppl ///
    Rte_ColdSmokeAppl_TargetTemp = 0U;
    //------//

    
    /// Temperature ///
    Rte_Temperature_ActionFlags = 0U;
    Rte_Temperature_ReadedTemp.Data = 0U;
    Rte_Temperature_ReadedTemp.Qf = 0U;
    //---------------//

}

void Rte_DelayMs(unsigned int d)
{
    unsigned int i;
    d = d * RTE_DELAY_MS_FACTOR;
    for (i = 0; i < d; i++)  // loop time is 5 machine cycles: 31.25ns @ 160MHz, 62.5ns @ 80MHz
    {
        asm("nop"); // minimum element to keep GCC from optimizing this function out.
    }

}