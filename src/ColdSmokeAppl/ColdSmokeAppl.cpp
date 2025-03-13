#include <ColdSmokeAppl.h>
#include <Temperature.h>

#include "ColdSmokeAppl_Priv.h"

/* Global Variables */
ColdSmokeAppl_Data_Type ColdSmokeAppl_Data;

uint8 ColdSmokeAppl_OneTimeCounter = 1U;
String ColdSmokeAppl_SerialPrint = "";

/* Functions */
/********************************************************
* ColdSmokeAppl Init function. 
********************************************************/
void ColdSmokeAppl_Init(void)
{
#ifdef SERIAL_PRINT_COLDSMOKEAPPL
    ColdSmokeAppl_SerialPrint = "ColdSmokeAppl: Init Start...";
    Serial.print(ColdSmokeAppl_SerialPrint);
#endif


    Rte_IWrite_ColdSmokeAppl_TargetTemp = COLDSMOKEAPPL_DEFAULT_TARGET_TEMP;

    ColdSmokeAppl_Data.u16TargetTemp = COLDSMOKEAPPL_DEFAULT_TARGET_TEMP * COLDSMOKEAPPL_TEMP_SCALE;
    ColdSmokeAppl_Data.u8WaitInInit = 0U;
    ColdSmokeAppl_Data.u8LedsStatus = 0U;
    ColdSmokeAppl_Data.u8HeatingStatus = COLDSMOKEAPPL_HEATING_STATUS_OFF;


    ColdSmokeAppl_Data.State = COLDSMOKEAPPL_STATE_INIT;

#ifdef SERIAL_PRINT_COLDSMOKEAPPL
    ColdSmokeAppl_SerialPrint = "  ...Stop.";
    Serial.println(ColdSmokeAppl_SerialPrint);
#endif
}


/********************************************************
* ColdSmokeAppl Main function. 
********************************************************/
void ColdSmokeAppl_Main(void)
{
    if (0U < ColdSmokeAppl_OneTimeCounter)
    {
        ColdSmokeAppl_OneTimeCounter = 0U;
#ifdef SERIAL_PRINT_COLDSMOKEAPPL
        ColdSmokeAppl_SerialPrint = "ColdSmokeAppl: Loop Start.";
        Serial.println(ColdSmokeAppl_SerialPrint);
#endif
    }
    else
    {
        /* do nothing */
    }    

    switch (ColdSmokeAppl_Data.State)
    {
        case COLDSMOKEAPPL_STATE_INIT:
        {
            if (COLDSMOKEAPPL_INIT_WAIT_THRESH > ColdSmokeAppl_Data.u8WaitInInit)
            {
                ColdSmokeAppl_Data.u8WaitInInit++;
                if (COLDSMOKEAPPL_LED_STATUS_OFF == (ColdSmokeAppl_Data.u8LedsStatus & (1U << COLDSMOKEAPPL_RED_LED_MASK)))
                {
                    /* Set RED led ON */
                    ColdSmokeAppl_Data.u8LedsStatus |= (1U << COLDSMOKEAPPL_RED_LED_MASK);
                    DIO_SetPinValue(COLDSMOKEAPPL_RED_LED_PIN, DIO_PIN_ON);
                }
                else
                {
                    /* Led is already on -> do nothing */
                }
            }
            else
            {
                /* Set RED led OFF */
                ColdSmokeAppl_Data.u8LedsStatus &= ~(1U << COLDSMOKEAPPL_RED_LED_MASK);
                DIO_SetPinValue(COLDSMOKEAPPL_RED_LED_PIN, DIO_PIN_OFF);

                /* Waiting time expired -> start heating */
                ColdSmokeAppl_Data.State = COLDSMOKEAPPL_STATE_PREHEAT;
            }
            
            break;
        }
        case COLDSMOKEAPPL_STATE_PREHEAT:
        {
            ColdSmokeAppl_ToggleYellowLed();
            ColdSmokeAppl_CheckTemp();
            break;
        }
        case COLDSMOKEAPPL_STATE_READY:
        {
            if (COLDSMOKEAPPL_LED_STATUS_OFF == (ColdSmokeAppl_Data.u8LedsStatus & (1U << COLDSMOKEAPPL_GREEN_LED_MASK)))
            {
                /* Set GREEN led ON */
                ColdSmokeAppl_Data.u8LedsStatus |= (1U << COLDSMOKEAPPL_GREEN_LED_MASK);
                DIO_SetPinValue(COLDSMOKEAPPL_GREEN_LED_PIN, DIO_PIN_ON);
            }
            else
            {
                /* Led is already on -> do nothing */
            }
            if (COLDSMOKEAPPL_LED_STATUS_OFF != (ColdSmokeAppl_Data.u8LedsStatus & (1U << COLDSMOKEAPPL_YELLOW_LED_MASK)))
            {
                /* Set YELLOW led OFF */
                ColdSmokeAppl_Data.u8LedsStatus &= ~(1U << COLDSMOKEAPPL_YELLOW_LED_MASK);
                DIO_SetPinValue(COLDSMOKEAPPL_YELLOW_LED_PIN, DIO_PIN_OFF);
            }
            else
            {
                /* Led is already off -> do nothing */
            }

            ColdSmokeAppl_CheckTemp();
            break;
        }
        default:
            break;
    }
}

void ColdSmokeAppl_CheckTemp(void)
{
    /* check quality status of the Temperature */
    if (TEMPERATURE_QF_GOOD == Rte_IRead_ColdSmokeAppl_Temperature_ReadedTemperature.Qf)
    {
        /* Quality status good -> call heating handling */
        ColdSmokeAppl_HandleHeating();
    }
    else
    {
        if(FALSE != (Rte_IRead_ColdSmokeAppl_Temperature_ActionFlags &(1 << TEMPERATURE_FIRST_READ_DONE_ID)))
        {
            /* No need to Reset handle reading flag */
            //Rte_IWrite_ColdSmokeAppl_Temperature_ActionFlags &= ~(1 << TEMPERATURE_FIRST_READ_DONE_ID);

            /* QF is not good -> start Red Led blinking as error state */
            ColdSmokeAppl_ToggleRedLed();
        }
        else
        {
            /* Wait till forst reading is performed */
        }
    }
}

void ColdSmokeAppl_HandleHeating(void)
{
    uint16 u16TargetTempLocal, u16CurrentTempLocal;

    u16TargetTempLocal = Rte_IWrite_ColdSmokeAppl_TargetTemp * COLDSMOKEAPPL_TEMP_SCALE;
    u16CurrentTempLocal = (uint16)(Rte_IRead_ColdSmokeAppl_Temperature_ReadedTemperature.Data * COLDSMOKEAPPL_TEMP_SCALE);
    /* check current temperature */
    if ((u16TargetTempLocal - COLDSMOKEAPPL_TEMP_TOLERANCE) > u16CurrentTempLocal)
    {
        if (COLDSMOKEAPPL_HEATING_STATUS_ON != ColdSmokeAppl_Data.u8HeatingStatus)
        {
            /* Set heating on */
            ColdSmokeAppl_Data.u8HeatingStatus = COLDSMOKEAPPL_HEATING_STATUS_ON;
            DIO_SetRelayPinValue(COLDSMOKEAPPL_RELAY_PIN, DIO_PIN_ON);
        }
        else
        {
            /* Heating is alredy on -> do nothing */
        }
    }
    else if (u16TargetTempLocal <= u16CurrentTempLocal)
    {
        if (COLDSMOKEAPPL_HEATING_STATUS_OFF != ColdSmokeAppl_Data.u8HeatingStatus)
        {
            /* Set heating off */
            ColdSmokeAppl_Data.u8HeatingStatus = COLDSMOKEAPPL_HEATING_STATUS_OFF;
            DIO_SetRelayPinValue(COLDSMOKEAPPL_RELAY_PIN, DIO_PIN_OFF);

            /* jump to READY state */
            ColdSmokeAppl_Data.State = COLDSMOKEAPPL_STATE_READY;
        }
        else
        {
            /* Heating is alredy off -> do nothing */
        }
    }
    else
    {
        /* between tolerance -> do nothing */
    }
}

void ColdSmokeAppl_ToggleRedLed(void)
{
    if (COLDSMOKEAPPL_LED_STATUS_OFF != (ColdSmokeAppl_Data.u8LedsStatus & (1U << COLDSMOKEAPPL_RED_LED_MASK)))
    {
        /* Led is already on -> set it off */
        ColdSmokeAppl_Data.u8LedsStatus &= ~(1U << COLDSMOKEAPPL_RED_LED_MASK);
        DIO_SetPinValue(COLDSMOKEAPPL_RED_LED_PIN, DIO_PIN_OFF);
    }
    else
    {
        /* Set RED led ON */
        ColdSmokeAppl_Data.u8LedsStatus |= (1U << COLDSMOKEAPPL_RED_LED_MASK);
        DIO_SetPinValue(COLDSMOKEAPPL_RED_LED_PIN, DIO_PIN_ON);

    }

    /* Error status -> shut down heating */
    if (COLDSMOKEAPPL_HEATING_STATUS_OFF != ColdSmokeAppl_Data.u8HeatingStatus)
    {
        /* Set heating off */
        ColdSmokeAppl_Data.u8HeatingStatus = COLDSMOKEAPPL_HEATING_STATUS_OFF;
        DIO_SetRelayPinValue(COLDSMOKEAPPL_RELAY_PIN, DIO_PIN_OFF);
    }
    else
    {
        /* Heating is alredy off -> do nothing */
    }

}
void ColdSmokeAppl_ToggleYellowLed(void)
{
    if (COLDSMOKEAPPL_LED_STATUS_OFF != (ColdSmokeAppl_Data.u8LedsStatus & (1U << COLDSMOKEAPPL_YELLOW_LED_MASK)))
    {
        /* Led is already on -> set it off */
        ColdSmokeAppl_Data.u8LedsStatus &= ~(1U << COLDSMOKEAPPL_YELLOW_LED_MASK);
        DIO_SetPinValue(COLDSMOKEAPPL_YELLOW_LED_PIN, DIO_PIN_OFF);
    }
    else
    {
        /* Set RED led ON */
        ColdSmokeAppl_Data.u8LedsStatus |= (1U << COLDSMOKEAPPL_YELLOW_LED_MASK);
        DIO_SetPinValue(COLDSMOKEAPPL_YELLOW_LED_PIN, DIO_PIN_ON);
    }
}
