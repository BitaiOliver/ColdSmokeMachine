#include <TaskMgr.h>
#include <DIO.h>
#include <Temperature.h>
#include <WiFiAccPoint.h>
#include <ColdSmokeAppl.h>

#include "TaskMgr_Priv.h"
#include "CompilerSwitch.h"

/* Global shared variables */
uint8 TaskMgr_InRunPrintCounter = 0U;
uint8 TaskMgr_OneTimeCounter = 1U;
String TaskMgr_SerialPrint = "";


void TaskMgr_Init(void)
{
#ifdef SERIAL_PRINT_TASKMGR
  TaskMgr_SerialPrint = "TaskMgr: Init Start....";
  Serial.print(TaskMgr_SerialPrint);
#endif

    /// OS ///
    /* Initialize timer handlers flag */
    Rte_IRead_TaskMgr_Handler_Flags = 0U;
    /* Call Rte_Init() */
    Rte_Init();
    //------//

    /// DIO ///
    DIO_Init();
    //-------//
	
    /// WiFiAccPoint ///
    WiFiAccPoint_Init();
    //---------------//
    
	/// Temperature ///
    Temperature_Init();
    //---------------//

  

    /// ColdSmokeAppl ///
    ColdSmokeAppl_Init();
    //---------------//

    

#ifdef SERIAL_PRINT_TASKMGR
  TaskMgr_SerialPrint = "TaskMgr.............Stop.";
  Serial.println(TaskMgr_SerialPrint);
#endif

}

void TaskMgr_Main(void)
{
    if (0U < TaskMgr_OneTimeCounter)
    {
        TaskMgr_OneTimeCounter = 0U;
#ifdef SERIAL_PRINT_TASKMGR
        TaskMgr_SerialPrint = "TaskMgr: Loop Start.";
        Serial.println(TaskMgr_SerialPrint);
#endif
    }
    else
    {
        /* do nothing */
    }

    if(FALSE != (Rte_IRead_TaskMgr_Handler_Flags & (1 << TIMER_100MS_ID)))
    {
        /* Reset flag */
        Rte_IRead_TaskMgr_Handler_Flags &= ~(1 << TIMER_100MS_ID);
        /* Call handler */
        Handle_100ms_Task();
    }
    else
    {
        /* code */
    }
    if(FALSE != (Rte_IRead_TaskMgr_Handler_Flags & (1 << TIMER_200MS_ID)))
    {
        /* Reset flag */
        Rte_IRead_TaskMgr_Handler_Flags &= ~(1 << TIMER_200MS_ID);
        /* Call handler */
        Handle_200ms_Task();
    }
    else
    {
        /* code */
    }
    if(FALSE != (Rte_IRead_TaskMgr_Handler_Flags & (1 << TIMER_1S_ID)))
    {
        /* Reset flag */
        Rte_IRead_TaskMgr_Handler_Flags &= ~(1 << TIMER_1S_ID);
        /* Call handler */
        Handle_1s_Task();
    }
    else
    {
        /* code */
    }
    if(FALSE != (Rte_IRead_TaskMgr_Handler_Flags & (1 << TIMER_5S_ID)))
    {
        /* Reset flag */
        Rte_IRead_TaskMgr_Handler_Flags &= ~(1 << TIMER_5S_ID);
        /* Call handler */
        Handle_5s_Task();
    }
    else
    {
        /* code */
    }
    if(FALSE != (Rte_IRead_TaskMgr_Handler_Flags & (1 << TIMER_10S_ID)))
    {
        /* Reset flag */
        Rte_IRead_TaskMgr_Handler_Flags &= ~(1 << TIMER_10S_ID);
        /* Call handler */
        Handle_10s_Task();
    }
    else
    {
        /* code */
    }
    if(FALSE != (Rte_IRead_TaskMgr_Handler_Flags & (1 << TIMER_30S_ID)))
    {
        /* Reset flag */
        Rte_IRead_TaskMgr_Handler_Flags &= ~(1 << TIMER_30S_ID);
        /* Call handler */
        Handle_30s_Task();
    }
    else
    {
        /* code */
    }
    if(FALSE != (Rte_IRead_TaskMgr_Handler_Flags & (1 << TIMER_1M_ID)))
    {
        /* Reset flag */
        Rte_IRead_TaskMgr_Handler_Flags &= ~(1 << TIMER_1M_ID);
        /* Call handler */
        Handle_1m_Task();
    }
    else
    {
        /* code */
    }
}

void TaskMgr_InRunPrint(void)
{
#ifdef SERIAL_PRINT_TASKMGR
    switch (TaskMgr_InRunPrintCounter)
    {
        case 0:
        {
            TaskMgr_InRunPrintCounter = 1U;
            TaskMgr_SerialPrint = "TaskMgr: Running 1s.";
            Serial.println(TaskMgr_SerialPrint);
            break;
        }
        case 1:
        {
            TaskMgr_InRunPrintCounter = 2U;
            //TaskMgr_SerialPrint = "TaskMgr: Run: ";
            //Serial.print(TaskMgr_SerialPrint);
            break;
        }
        case 2:
        {
            TaskMgr_InRunPrintCounter = 3U;
            //TaskMgr_SerialPrint = " .";
            //TaskMgr_SerialPrint = " W: " + String(Rte_IRead_TaskMgr_WifiStatus) + " B: " + String(Rte_IRead_TaskMgr_ServerConnStatus);
            //Serial.print(TaskMgr_SerialPrint);
            break;
        }
        case 3:
        {
            TaskMgr_InRunPrintCounter = 1U;
            TaskMgr_SerialPrint = "TaskMgr: Run:  W: " + String(Rte_IRead_TaskMgr_WebServer_Status);
            Serial.println(TaskMgr_SerialPrint);
            break;
        }
        default:
            break;
    }
#endif
}

void Handle_100ms_Task(void)
{
    //digitalWrite(COLDSMOKEAPPL_RED_LED_PIN, TaskMgr_PinToggle);
    //TaskMgr_PinToggle ^=1U;
}
void Handle_200ms_Task(void)
{
    //digitalWrite(COLDSMOKEAPPL_YELLOW_LED_PIN, TaskMgr_PinToggle2);
    //TaskMgr_PinToggle2 ^=1U;
}

void Handle_1s_Task(void)
{
    //digitalWrite(COLDSMOKEAPPL_YELLOW_LED_PIN, 1u);

    DIO_Main();
        
    WiFiAccPoint_Main();
	Temperature_Main();

    ColdSmokeAppl_Main();



    TaskMgr_InRunPrint();
	
    //digitalWrite(COLDSMOKEAPPL_YELLOW_LED_PIN, 0u);
}
void Handle_5s_Task(void)
{
}
void Handle_10s_Task(void)
{
    /* Set flag for Temperature to handle reading */
    Rte_IWrite_TaskMgr_Temperature_ActionFlags |= (1 << TEMPERATURE_HANDLE_READING_ID);
}
void Handle_30s_Task(void)
{  
}
void Handle_1m_Task(void)
{
}

/*
void ICACHE_RAM_ATTR Twi::busywait(unsigned int v)
{
    unsigned int i;
    for (i = 0; i < v; i++)  // loop time is 5 machine cycles: 31.25ns @ 160MHz, 62.5ns @ 80MHz
    {
        asm("nop"); // minimum element to keep GCC from optimizing this function out.
    }
}
*/