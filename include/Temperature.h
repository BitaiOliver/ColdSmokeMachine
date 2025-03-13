#ifndef TEMPERATURE_H
#define TEMPERATURE_H 1
/**************** Includes ************/
#include <Arduino.h>



/**************** Defines ************/

#define TEMPERATURE_HANDLE_READING_ID       0U
#define TEMPERATURE_FIRST_READ_DONE_ID      1U

#define TEMPERATURE_READ_TEMP_FACTOR   10U /* Multiplication factor of the sensor data */

#define TEMPERATURE_MIN_TEMP            0U 
#define TEMPERATURE_MAX_TEMP            100U 


/**************** Varables ************/


/**************** Functions ************/
void Temperature_Init(void);
void Temperature_Main(void);



#endif