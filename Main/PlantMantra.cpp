/**********************************************
PlantMantra allows user to monitor Temperature, Moisture, and Sunlight data of Houseplant.
Sensors used are as follows:


Written for an Arduino Nano 33 IOT device.

Written by Sierra Catelani - March 29, 2020
***********************************************/


//************* LIBRARIES HERE ****************//
#include <WiFiNINA.h>
#include "MoistureSensor.h"
#include "TempSensor.h"
#include "SunlightSensor.h"
#include "NetworkCodes.h" 

//************* DEFINITIONS HERE **************//
#define TIMEOUT  5000  // Timeout for server response.

//******** SETUP LOCAL NETWORK DETAILS ********//
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the Wifi radio's status

//*********** SETUP CLIENT + SERVER ***********//

//Setup Arduino Nano IOT Client
WiFiClient sensorClient;


//Setup ThinkSpeak Server
char ThingSpeakServer[] = "api.thingspeak.com";
String writeAPIKey = SECRET_KEY;


//******** SET UP SENSOR INSTANCES  ************//


MoistureSensor sensorNA555(NA555_PIN);
SunlightSensor sensorSI1145;
TempSensor sensorMCP9808;

uint8_t hwKey = 0x17;

uint16_t moistureData;
uint16_t visLightData;
uint16_t temperatureData;

unsigned long dataLogDelta = 60000;
unsigned long previousDataLog = 600000;

/**************** SETUP FUNCTION - CONNECT TO WIFI ******************/
void setup() {

  Wire.begin();

  //SETUP SENSOR HARDWARE
  sensorSI1145.SetHWKEY(0x17);
  sensorSI1145.SetMeasRate(0x00,0x00);

  sensorSI1145.EnProximatySensors(0);
  sensorSI1145.EnALSSensors(1);
  sensorSI1145.EnUVSensor(0);

  //CONNECT TO WIFI
  
  // check for the WiFi module - system will hang if WiFi module not found.
  while (WiFi.status() == WL_NO_MODULE) {
    // don't continue
  }
  
  //Connect device to LAN Network
  wifiNetworkConnect();

}


/***************************  PROGRAM LOOP  **********************************/
/**************** COLLECT DATA AND SEND TO THINGSPEAK CLOUD ******************/
void loop() {


  //Wait for 5 minutes to pass between datalogs
  if(millis() - previousDataLog > dataLogDelta){

      //Reset datalog timestamp
      previousDataLog = millis();

      //reconnect to LAN network if connection was lost
      if(WiFi.status != WL_CONNECTED){
        wifiNetworkConnect();
      } 

      //Check that finicky sunlight sensor is ready to run a measurement
      hwKey = sensorSI1145.RegRead(REG_HW_KEY);
  
      while(hwKey != 0x17){
        //Serial.print("\nSI1143 not ready!\n");
        delay(1000);
      }

      //Force sunlight sensor measurement
      sensorSI1145.MeasureALSCMD();
      delay(5000);


      //Sample all sensors
      moistureData = sensorNA555.readAndAve();
      visLightData = sensorSI1145.ReadAmbVisData();
      temperatureData = sensorMCP9808.ReadTempValue();

      //Post sensor readings to Serial Monitor
      /*
      Serial.println("Moisture Reading = " + String(moistureData));
      Serial.println("Light Reading = " + String(visLightData));
      Serial.println("Temperature Reading = " + String(temperatureData));
      Serial.println();
      */
      
      //Send HTTP Command to post data from sensors to ThingSpeak Cloud database
      if(!mHttpRequest(moistureData,visLightData,temperatureData)){
        //Serial.print("Error: Could not connect to ThingSpeakServer!");
      }   
   }
}


/***************************  WIFI FUNCTIONS **********************************/
/**************** *************************************************************/

// This function runs the WiFi connect sequence to connect IOT device to local network.
// Note: program will hang if device does not connect.
// returns:  none
void wifiNetworkConnect(void){
  
    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
      //Connect to WPA/WPA2 network:
      status = WiFi.begin(ssid, pass);
  
      //wait 10 seconds for connection:
      delay(10000);
      }
      
    return;
}


// This function writes an Http message to the Server to write data to the ThingSpeak cloud database.
// returns:  1 on success, -1 on fail.
int mHttpRequest(uint16_t moistData, uint16_t lightData, uint16_t tempData){

    // Create data string to send to ThingSpeak.
    String data = "field1=" + String(moistData) + "&field2=" + String(lightData) + "&field3=" + String(tempData);
    
    // POST data to ThingSpeak.
    if (sensorClient.connect(ThingSpeakServer, 80)) {
        
        sensorClient.println("POST /update HTTP/1.1");
        sensorClient.println("Host: api.thingspeak.com");
        sensorClient.println("Connection: close");
        sensorClient.println("X-THINGSPEAKAPIKEY: "+writeAPIKey);
        sensorClient.println("Content-Type: application/x-www-form-urlencoded");
        sensorClient.print("Content-Length: ");
        sensorClient.print(data.length());
        sensorClient.print("\n\n");
        sensorClient.print(data);     
        delay(200);
        
        getResponse();
    
        return 1;
      }
  else{return -1;}

}

// This function waits for the server to respond, and then builds a string with response message
// Returns: Response string
String getResponse(){
  
  String serverResponse;
  unsigned long startTime = millis();

  //Wait for server to indicate availability
  while (sensorClient.available() == 0 && ((millis() - startTime) < TIMEOUT)){
        delay(5);
  }

  //Create message string from chars received by server.
  if(sensorClient.available() > 0){
     char serverChar;
     // Read chars from buffer and build a string
     while (sensorClient.available() > 0){
         serverChar = sensorClient.read();
         serverResponse += serverChar;        
         }
    }

  //close session with Server.
  sensorClient.stop();

  return response;
}

  