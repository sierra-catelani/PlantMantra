/********************************************
	MoistureDet.cc - Reads data from an NA555 capacitive moisture sensor
	Created by Sierra Catelani, March 21, 2020.
*********************************************/


#include <Arduino.h>
#include "MoistureSensor.h"


//MoistureSensor.MoistureSensor -> Initializes instance of MoistureSensor Class
//and sets up the pin that will be used in this MoistureSensor instance.
//Inputs: Pin to be used for sensor input.
MoistureSensor::MoistureSensor(uint8_t pin){
	_pin = pin;
}

//MoistureSensor.readRaw() Function will read the raw data of the Moisture Sensor.
//Inputs: None
//Return: Raw Sensor Data (between 0  - 1023)
uint16_t MoistureSensor::readRaw(){

  return analogRead(_pin);
}

//MoistureSensor.readAndAve() will read 10 samples raw data of the Moisture Sensor and average them.
//Inputs: None
//Return: Averaged sensor data (between 0  - 1023)
uint16_t MoistureSensor::readAndAve(){

  uint16_t runningSum = 0;
  for(int i=0; i<10; i++){ runningSum += readRaw(); }

  return runningSum/10;
  
}