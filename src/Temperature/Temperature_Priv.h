#ifndef TEMPERATURE_PRIV_H
#define TEMPERATURE_PRIV_H 1
/**************** Includes ************/
#include <Arduino.h>
#include <Rte_Types.h>
#include "CompilerSwitch.h"


/**************** Defines ************/
#define TEMPERATURE_READ_TEMP_THRSH    2U /* 20s read. */


#define TEMPERATURE_SENSOR_BUS_PIN      COLDSMOKEAPPL_TEMP_SENSOR_PIN  /*D02*/


/* Main function states */
#define TEMPERATURE_STATE_WAIT         1U 
#define TEMPERATURE_STATE_READ         2U 


/* Reading states */
#define TEMPERATURE_READING_STEP_1     0U 
#define TEMPERATURE_READING_STEP_2     1U 
#define TEMPERATURE_READING_STEP_3     2U 

#define TEMPERATURE_NUMBER_OF_READINGS 2U 


#define TEMPERATURE_NB_OF_RETRY         2U

/**************** Varables ************/

typedef struct Temperature_Data_Type_Tag
{
    uint8 ReadingTimer;    
    uint8 State;
    uint8 ReadingState;
    uint8 NbOfReadingRetry;
    Rte_Temperature_Reading_Type CurrentReading[TEMPERATURE_NUMBER_OF_READINGS];
}Temperature_Data_Type;

extern Rte_Temperature_Reading_Type Rte_Temperature_ReadedTemp;
#define Rte_IWrite_Temperature_ReadedTemperature    Rte_Temperature_ReadedTemp

extern uint8 Rte_Temperature_ActionFlags;
#define Rte_IRead_Temperature_ActionFlags    Rte_Temperature_ActionFlags
#define Rte_IWrite_Temperature_ActionFlags    Rte_Temperature_ActionFlags
/**************** Functions ************/
void temperature_ReadTemperature();
void temperature_ComputeTemperature();
void temperature_RequestSensorRead();
void temperature_ReadSensorData();



#endif