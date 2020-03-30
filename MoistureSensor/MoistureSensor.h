/********************************************
	MoistureDet.h - Reads data from an NA555 capacitive moisture sensor
	Created by Sierra Catelani, March 21, 2020.
*********************************************/

#ifndef MoistureSensor_h
#define MoistureSensor_h

#include <Arduino.h>


#define NA555_PIN A1

class MoistureSensor
{
	public:
		MoistureSensor(uint8_t pin);
		uint16_t readRaw(void);
		uint16_t readAndAve(void);

	private:
		int _pin;

};



#endif