#ifndef RTE_TYPES_H
#define RTE_TYPES_H 1
/**************** Includes ************/
#include <Arduino.h>



/**************** Data Types ************/
typedef struct Rte_Temperature_Reading_Type_Tag
{
    float      Data;
    uint8       Qf; 
}Rte_Temperature_Reading_Type;


/**************** Varables ************/



/**************** Functions ************/
void Rte_Init(void);


#endif