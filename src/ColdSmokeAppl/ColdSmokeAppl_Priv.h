#ifndef COLDSMOKEAPPL_PRIV_H
#define COLDSMOKEAPPL_PRIV_H 1
/**************** Includes ************/
#include <Arduino.h>
#include <Rte_Types.h>
#include <DIO.h>

#include "CompilerSwitch.h"


/**************** Defines ************/
#define COLDSMOKEAPPL_STATE_INIT                      1U
#define COLDSMOKEAPPL_STATE_PREHEAT                   2U
#define COLDSMOKEAPPL_STATE_READY                     3U

#define COLDSMOKEAPPL_INIT_WAIT_THRESH                5U /* wait in init 5 seconds before start */


#define COLDSMOKEAPPL_RED_LED_MASK                      0U
#define COLDSMOKEAPPL_YELLOW_LED_MASK                   1U
#define COLDSMOKEAPPL_GREEN_LED_MASK                    2U

#define COLDSMOKEAPPL_LED_STATUS_OFF                    0U
#define COLDSMOKEAPPL_LED_STATUS_ON                     1U

#define COLDSMOKEAPPL_HEATING_STATUS_OFF                0U
#define COLDSMOKEAPPL_HEATING_STATUS_ON                 1U

#define COLDSMOKEAPPL_DEFAULT_TARGET_TEMP               60U /* 60 Celsius*/
#define COLDSMOKEAPPL_TEMP_SCALE                        10U /* Internal temperature scaling */
#define COLDSMOKEAPPL_TEMP_TOLERANCE                    5U /* Internal temperature tolerance 0.5 Celsius */


/**************** Varables ************/
typedef struct ColdSmokeAppl_Data_Type_Tag
{
    uint8       State;
    uint16      u16TargetTemp;    
    uint8       u8WaitInInit; 
    uint8       u8LedsStatus; 
    uint8       u8HeatingStatus; 
}ColdSmokeAppl_Data_Type;

extern uint8 Rte_ColdSmokeAppl_TargetTemp;
#define Rte_IWrite_ColdSmokeAppl_TargetTemp    Rte_ColdSmokeAppl_TargetTemp

extern Rte_Temperature_Reading_Type Rte_Temperature_ReadedTemp;
#define Rte_IRead_ColdSmokeAppl_Temperature_ReadedTemperature    Rte_Temperature_ReadedTemp

extern uint8 Rte_Temperature_ActionFlags;
#define Rte_IRead_ColdSmokeAppl_Temperature_ActionFlags    Rte_Temperature_ActionFlags
#define Rte_IWrite_ColdSmokeAppl_Temperature_ActionFlags    Rte_Temperature_ActionFlags


/**************** Functions ************/
void ColdSmokeAppl_ToggleRedLed(void);
void ColdSmokeAppl_ToggleYellowLed(void);
void ColdSmokeAppl_CheckTemp(void);
void ColdSmokeAppl_HandleHeating(void);


#endif