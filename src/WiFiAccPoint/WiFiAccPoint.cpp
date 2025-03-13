#include <WiFiAccPoint.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Temperature.h>

#include "WiFiAccPoint_Priv.h"

/* Global Variables */
WiFiAccPoint_Data_Type WiFiAccPoint_Data;

uint8 WiFiAccPoint_OneTimeCounter = 1U;
String WiFiAccPoint_SerialPrint = "";

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

AsyncWebServer server(80);



/* Functions */
/********************************************************
* WiFiAccPoint Init function. 
********************************************************/
void WiFiAccPoint_Init(void)
{
#ifdef SERIAL_PRINT_WIFIACCPOINT
    WiFiAccPoint_SerialPrint = "WiFiAccPoint: Init Start...";
    Serial.print(WiFiAccPoint_SerialPrint);
#endif

    WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);
    WiFi.softAP(WiFi_Name, WiFi_Password);
    delay(100);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html, WiFiAccPoint_Processor);
    });
    server.on("/tartemp", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", String(Rte_IRead_WiFiAccPoint_ColdSmokeAppl_TargetTemp).c_str());
    });
    server.on("/curtemp", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", String(Rte_IRead_WiFiAccPoint_Temperature_ReadedTemperature.Data).c_str());
    });
    server.on("/up", HTTP_GET, [](AsyncWebServerRequest *request){
        WiFiAccPoint_HandleUP();
        request->send_P(200, "text/html", index_html,  WiFiAccPoint_Processor);
    });
    server.on("/down", HTTP_GET, [](AsyncWebServerRequest *request){
        WiFiAccPoint_HandleDOWN();
        request->send_P(200, "text/html", index_html,  WiFiAccPoint_Processor);
    });

    // Start server
    server.begin();


    WiFiAccPoint_Data.State = WIFIACCPOINT_STATE_INIT;

#ifdef SERIAL_PRINT_WIFIACCPOINT
    WiFiAccPoint_SerialPrint = "  ...Stop.";
    Serial.println(WiFiAccPoint_SerialPrint);
#endif
}


/********************************************************
* WiFiAccPoint Main function. 
********************************************************/
void WiFiAccPoint_Main(void)
{
    if (0U < WiFiAccPoint_OneTimeCounter)
    {
        WiFiAccPoint_OneTimeCounter = 0U;
#ifdef SERIAL_PRINT_WIFIACCPOINT
        WiFiAccPoint_SerialPrint = "WiFiAccPoint: Loop Start.";
        Serial.println(WiFiAccPoint_SerialPrint);
#endif
    }
    else
    {
        /* do nothing */
    }

    switch (WiFiAccPoint_Data.State)
    {
        case WIFIACCPOINT_STATE_INIT:
        {
            WiFiAccPoint_Data.State = WIFIACCPOINT_STATE_RUN;
            break;
        }
        case WIFIACCPOINT_STATE_RUN:
        {
            /* Do nothing */
            break;
        }        
        default:
            break;
    }
}

String WiFiAccPoint_Processor(const String& var)
{
  //Serial.println("------");
  //Serial.println(var);
 //Serial.println("------");
  String retval = "";
 if (var == "TARTEMP"){
    retval = String(Rte_IRead_WiFiAccPoint_ColdSmokeAppl_TargetTemp).c_str();
  }
  else if (var == "CURTEMP"){
    retval = String(Rte_IRead_WiFiAccPoint_Temperature_ReadedTemperature.Data).c_str();
  }
  else
  {
    /* code */
  }
  //Serial.println(retval);
  return retval;
}

void  WiFiAccPoint_HandleUP() 
{
    if (TEMPERATURE_MAX_TEMP > Rte_IRead_WiFiAccPoint_ColdSmokeAppl_TargetTemp)
    {
        Rte_IWrite_WiFiAccPoint_ColdSmokeAppl_TargetTemp++;
    }
}
void  WiFiAccPoint_HandleDOWN() 
{
    if(TEMPERATURE_MIN_TEMP < Rte_IRead_WiFiAccPoint_ColdSmokeAppl_TargetTemp)
    {
        Rte_IWrite_WiFiAccPoint_ColdSmokeAppl_TargetTemp--;
    }
}
