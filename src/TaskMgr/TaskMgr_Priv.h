#ifndef TASKMGR_PRIV_H
#define TASKMGR_PRIV_H 1
/**************** Includes ************/
#include <Arduino.h>
#include <Rte_Types.h>


/**************** Defines ************/


/**************** Varables ************/
/// OS ///
extern uint8 Rte_TaskMgr_Handler_Flags;
#define Rte_IRead_TaskMgr_Handler_Flags    Rte_TaskMgr_Handler_Flags
//-----//

/// Temperature ///
extern uint8 Rte_Temperature_ActionFlags;
#define Rte_IWrite_TaskMgr_Temperature_ActionFlags    Rte_Temperature_ActionFlags
//---------------//

/// Temperature ///
extern uint8 Rte_WiFiAccPoint_Status;
#define Rte_IRead_TaskMgr_WebServer_Status    Rte_WiFiAccPoint_Status
//---------------//

/**************** Functions ************/
void Handle_100ms_Task(void);
void Handle_200ms_Task(void);
void Handle_1s_Task(void);
void Handle_5s_Task(void);
void Handle_10s_Task(void);
void Handle_30s_Task(void);
void Handle_1m_Task(void);

void TaskMgr_InRunPrint(void);


#endif