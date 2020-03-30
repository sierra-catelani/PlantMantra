PlantMantra allows user to read the Temperature, Moisture, and Sunlight profiles of a houseplant.
An Arduino with WiFi chip and sensors are needed to successfully setup this project.

The sensors used for this project are:
NA555 - Moisture Sensor
SI1145 - Sunlight Sensor
MCP9808 - Temperature sensor sensor

These sensors can all be purchased on the Seeed website.


This project includes custom written libraries for each sensor.  To include them in your main code, include the following header files:

MoistureSensor.h - reads NA555
SunlightSensor.h - reads SI1145
TempSensor.h - reads MCP9808

To include each sensor, include the header files in the header of the main program

Note that the SunlightSensor and MoistureSensor are accessible via I2C.  Connect their SCL/SDA pins to the correct pinout of the Arduino board.  The Moisture sensor is an analog sensor and its signal pin can be connected to any of the Arduino's Analog In pins.  During datareading, the moisture sensor input pin is defined by the "" variable.  This variable will need to be changed to the pin connected to the Moisture sensor output signal.

Main program operates as follows:

The WiFi chip must connect to the users LAN network.  To do this, edit the SSID information in the included PasscodeInfo.h file.  Then the main code will take care of WiFi connection.

Data is collected at a specified time interval and is then sent to the Thingspeak cloud server where further processing can be done. 

To use the ThingSpeak server, the user must set up a ThingSpeak account and set up the channel to enable the three sensors.  Your Thingspeak Key must be included in the PasscodeInfo.h file. An example channel can be found here:


