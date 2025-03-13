#ifndef TASKMGR_H
#define TASKMGR_H 1
/**************** Includes ************/
#include <Arduino.h>



/**************** Defines ************/
#define TIMER_100MS             1U
#define TIMER_200MS             2U
#define TIMER_1S                10U
#define TIMER_5S                50U
#define TIMER_10S               100U
#define TIMER_30S               300U
#define TIMER_1M                600U /* put back to 120 */


#define TIMER_100MS_ID          0U
#define TIMER_200MS_ID          1U
#define TIMER_1S_ID             2U
#define TIMER_5S_ID             3U
#define TIMER_10S_ID            4U
#define TIMER_30S_ID            5U
#define TIMER_1M_ID             6U

#define NUMBER_OF_TIMERS        5U


/**************** Varables ************/


/**************** Functions ************/

void TaskMgr_Init(void);
void TaskMgr_Main(void);



#endif