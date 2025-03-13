#ifndef COMPILER_SWITCH_H
#define COMPILER_SWITCH_H 1
/* This header shall be included in everi software component.
    This header includes defines which are used as compiler switches. */


// #define SERIAL_PRINT_MAIN                       1U /* If this define exist serial prints are working for Main module */
// #define SERIAL_PRINT_TASKMGR                    1U /* If this define exist serial prints are working for TaskManager module */
// #define SERIAL_PRINT_TEMP                       1U /* If this define exist serial prints are working for Temperature module */
// #define SERIAL_PRINT_DIO                        1U /* If this define exist serial prints are working for Dio module */
// #define SERIAL_PRINT_WIFIACCPOINT               1U /* If this define exist serial prints are working for WebServer module */
// #define SERIAL_PRINT_COLDSMOKEAPPL              1U /* If this define exist serial prints are working for Cold Smoke Application module */


/* If this define exist then IO pin is seted to LOW in order to activate relay/heating. */
/* Comment/delete define for reqular active on HIGH relay*/
//#define RELAY_TYPE_ACTIVE_ON_LOW            1U

#ifdef RELAY_TYPE_ACTIVE_ON_LOW
#define RELAY_DEFAULT_INIT_VALUE            HIGH
#else
#define RELAY_DEFAULT_INIT_VALUE            LOW
#endif

/* Some other defines used by all the modules */
#define TRUE                    1U
#define FALSE                   0U


#define TEMPERATURE_QF_INIT             0U
#define TEMPERATURE_QF_GOOD             1U
#define TEMPERATURE_QF_FAILING          2U
#define TEMPERATURE_QF_BAD              3U


const char WiFi_Name[] = "ColdSmoke";          // WiFi name (SSID)   
const char WiFi_Password[] = "Cold1234";       // WiFi password

/* Application pin configuration */
#define COLDSMOKEAPPL_TEMP_SENSOR_PIN                   4U /* D02 */
#define COLDSMOKEAPPL_RELAY_PIN                         14U /* D05 */
#define COLDSMOKEAPPL_RED_LED_PIN                       12U /* D06 */
#define COLDSMOKEAPPL_YELLOW_LED_PIN                    13U /* D07 */
#define COLDSMOKEAPPL_GREEN_LED_PIN                     15U /* D08 */


#endif