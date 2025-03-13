#include <OneWire.h>
#include <DallasTemperature.h>
#include <Temperature.h>

#include "Temperature_Priv.h"
#include "CompilerSwitch.h"

/* Global Variables */

Temperature_Data_Type Temperature_Data;


OneWire oneWire(TEMPERATURE_SENSOR_BUS_PIN);
DallasTemperature sensors(&oneWire);


String Temperature_PrintString;

uint8 Temperature_OneTimeCounter = 1U;
String Temperature_SerialPrint = "";


/* Functions */
/********************************************************
* Temperature Init function. 
********************************************************/
void Temperature_Init(void)
{
#ifdef SERIAL_PRINT_TEMP
  Temperature_SerialPrint = "Temperature: Init Start...";
  Serial.print(Temperature_SerialPrint);
#endif

    Temperature_PrintString = "";

    Rte_IRead_Temperature_ActionFlags = 0U;
    Rte_IWrite_Temperature_ReadedTemperature.Data = 0U;
    Rte_IWrite_Temperature_ReadedTemperature.Qf = TEMPERATURE_QF_INIT;

    Temperature_Data.ReadingTimer = 1U; /* initialize with 1 to manage start offset */
    Temperature_Data.State = TEMPERATURE_STATE_WAIT;
    Temperature_Data.ReadingState = TEMPERATURE_READING_STEP_1;
    Temperature_Data.NbOfReadingRetry = 0U;

	/* Set to FALSE in order to process in asyncronous mode */
    sensors.setWaitForConversion(FALSE);
    /* start one wire */
    sensors.begin();
#ifdef SERIAL_PRINT_TEMP
  Temperature_SerialPrint = "...Init Stop.";
  Serial.println(Temperature_SerialPrint);
#endif
}

/********************************************************
* Temperature Main function. 
********************************************************/
void Temperature_Main(void)
{
    if (0U < Temperature_OneTimeCounter)
    {
        Temperature_OneTimeCounter = 0U;
#ifdef SERIAL_PRINT_TEMP
        Temperature_SerialPrint = "Temperature: Loop Start.";
        Serial.println(Temperature_SerialPrint);
#endif
    }
    else
    {
        /* do nothing */
    }

    if(FALSE != (Rte_IRead_Temperature_ActionFlags &(1 << TEMPERATURE_HANDLE_READING_ID)))
    {
        /* Reset handle reading flag */
        Rte_IRead_Temperature_ActionFlags &= ~(1 << TEMPERATURE_HANDLE_READING_ID);
        /* Increment reading timer */
        Temperature_Data.ReadingTimer++;
    }
    else
    {
        /* code */
    }

    switch (Temperature_Data.State)
    {
    case TEMPERATURE_STATE_WAIT:
    {
        if (TEMPERATURE_READ_TEMP_THRSH <= Temperature_Data.ReadingTimer)
        {
            /* Reset reading timer */
            Temperature_Data.ReadingTimer = 0U;
            Temperature_Data.State = TEMPERATURE_STATE_READ;
        }
        else
        {
            /* code */
        } 
#ifdef SERIAL_PRINT_TEMP
            Temperature_PrintString = "Temperature: Wait to flag ";
            //Serial.println(Temperature_PrintString);
#endif

        break;
    }
    case TEMPERATURE_STATE_READ:
    {
        temperature_ReadTemperature();
        break;
    }
    
    default:
        break;
    }

}

/********************************************************
* Read temperature handler function. 
********************************************************/
void temperature_ReadTemperature()
{
    switch (Temperature_Data.ReadingState)
    {
    case TEMPERATURE_READING_STEP_1:
    {
#ifdef SERIAL_PRINT_TEMP
        Temperature_PrintString = "Temperature: Read temp step 1 ";
        Serial.println(Temperature_PrintString);
#endif
        /* Request a temperature reading */
        temperature_RequestSensorRead();
        break;
    }
    case TEMPERATURE_READING_STEP_2:
    {
#ifdef SERIAL_PRINT_TEMP
        Temperature_PrintString = "Temperature: Read temp step 2 ";
        Serial.println(Temperature_PrintString);
#endif
        /* Read data from sensor */
        temperature_ReadSensorData();
        break;
    }
    case TEMPERATURE_READING_STEP_3:
    {
#ifdef SERIAL_PRINT_TEMP
        Temperature_PrintString = "Temperature: Read temp step 3 ";
        Serial.println(Temperature_PrintString);
#endif
        /* Check the readed temperature */
        temperature_ComputeTemperature();
        break;
    }
    
    default:
        break;
    }
}



/********************************************************
* Request sensor read. 
********************************************************/
void temperature_RequestSensorRead()
{

    sensors.requestTemperatures();  

    Temperature_Data.ReadingState = TEMPERATURE_READING_STEP_2;

}

/********************************************************
* Request sensor read. 
********************************************************/
void temperature_ReadSensorData()
{
    float Temperature = 0;

    Temperature = sensors.getTempCByIndex(0);

#ifdef SERIAL_PRINT_TEMP
    Temperature_PrintString = "Temperature: Sensor Data: " + String(Temperature);
    Serial.println(Temperature_PrintString);
#endif
    /* Temperature valid -> save reading */
     if ((TEMPERATURE_MIN_TEMP <= Temperature) && (TEMPERATURE_MAX_TEMP >= Temperature))
    {
        /* Temperature valid -> save reading */
        Temperature_Data.CurrentReading[TEMPERATURE_READING_STEP_1].Data = Temperature; 
        Temperature_Data.CurrentReading[TEMPERATURE_READING_STEP_1].Qf = TEMPERATURE_QF_GOOD;

        /* Reset retry counter */
        Temperature_Data.NbOfReadingRetry = 0U;

        /* Jump to the next state */
        Temperature_Data.ReadingState = TEMPERATURE_READING_STEP_3; 
    }
    else
    {
        /* Failed to read sensor or negative value */
        Temperature_Data.CurrentReading[TEMPERATURE_READING_STEP_1].Qf = TEMPERATURE_QF_BAD; 
        /* Increment nuber of retry counter */
        Temperature_Data.NbOfReadingRetry++;

        if (TEMPERATURE_NB_OF_RETRY <= Temperature_Data.NbOfReadingRetry)
        {
            /* Reset retry counter */
            Temperature_Data.NbOfReadingRetry = 0U;
            /* Max retry reached -> Jump to the next state */
            Temperature_Data.ReadingState = TEMPERATURE_READING_STEP_3; 
        }
        else
        {
            /* Max retry not reached -> Jump to the first state state */
            Temperature_Data.ReadingState = TEMPERATURE_READING_STEP_1; 
        }   
    }

}


/********************************************************
* Compute final temperature value. 
********************************************************/
void temperature_ComputeTemperature()
{

    /* Set reading status back to step 1 and Main status back to wait */
    Temperature_Data.State = TEMPERATURE_STATE_WAIT;
    Temperature_Data.ReadingState = TEMPERATURE_READING_STEP_1;

    /* Set flag that the first reading of the temperature has been performed */
    Rte_IWrite_Temperature_ActionFlags |= (1 << TEMPERATURE_FIRST_READ_DONE_ID);

    if (TEMPERATURE_QF_GOOD == Temperature_Data.CurrentReading[TEMPERATURE_READING_STEP_1].Qf)
    {
        /* QF reading are good */
        Rte_IWrite_Temperature_ReadedTemperature.Qf = TEMPERATURE_QF_GOOD;
        Rte_IWrite_Temperature_ReadedTemperature.Data =  Temperature_Data.CurrentReading[TEMPERATURE_READING_STEP_1].Data;
    }
    else
    {
        /* No reading reading is good, no need to modify the actual value. */
        Rte_IWrite_Temperature_ReadedTemperature.Qf = TEMPERATURE_QF_BAD;
    }
}