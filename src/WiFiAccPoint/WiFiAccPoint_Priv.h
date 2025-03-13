#ifndef WIFIACCPOINT_PRIV_H
#define WIFIACCPOINT_PRIV_H 1

#include <Arduino.h>
#include <Rte_Types.h>
#include <CompilerSwitch.h>

/**************** Defines ************/
#define WIFIACCPOINT_STATE_INIT                      1U
#define WIFIACCPOINT_STATE_RUN                       2U

/**************** Varables ************/
typedef struct WiFiAccPoint_Data_Type_Tag
{
    uint8       State; 
}WiFiAccPoint_Data_Type;

extern uint8 Rte_WiFiAccPoint_Status;
#define Rte_IWrite_WiFiAccPoint_Status    Rte_WiFiAccPoint_Status

extern Rte_Temperature_Reading_Type Rte_Temperature_ReadedTemp;
#define Rte_IRead_WiFiAccPoint_Temperature_ReadedTemperature    Rte_Temperature_ReadedTemp

extern uint8 Rte_ColdSmokeAppl_TargetTemp;
#define Rte_IRead_WiFiAccPoint_ColdSmokeAppl_TargetTemp    Rte_ColdSmokeAppl_TargetTemp
#define Rte_IWrite_WiFiAccPoint_ColdSmokeAppl_TargetTemp    Rte_ColdSmokeAppl_TargetTemp

/**************** Functions ************/
String WiFiAccPoint_Processor(const String& var);
void WiFiAccPoint_HandleUP();
void WiFiAccPoint_HandleDOWN();


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>EuroSteaks</title>
	<style>
	html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
	body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 20px;}
  .button {
  background-color: #4CAF50; /* Green */
  border: none;
  color: white;
  padding: 15px 32px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 16px;
  margin: 4px 2px;
  cursor: pointer;
}

.button1 {
  background-color: white; 
  color: black; 
  border: 2px solid #4CAF50;
}

.button2 {
  background-color: white; 
  color: black; 
  border: 2px solid #f44336;
}
	</style>
</head>
<body>
  <h2>Norbi's Cold Smoke Machine</h2>

  <p>
    <a href="/up"><button class="button button1">Increase</button></a>
  </p>
  <p>
    <a href="/down"><button class="button button2">Decrease</button></a>
  </p>

  <p>
    <h3><span class="dht-labels">Target Temperature:</span></h3>
    <span id="temperature"><font color=green size="8">%TARTEMP%</font></span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <h3><span class="dht-labels">Current Temperature:</span></h3>
    <font color=red size="8">
      <span id="curtemp">%CURTEMP%</span>
    </font>
    <sup class="units">&deg;C</sup>
  </p>
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("curtemp").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/curtemp", true);
  xhttp.send();
}, 3000);

</script>
</html>)rawliteral";

#endif
