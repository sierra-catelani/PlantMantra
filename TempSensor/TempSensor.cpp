/********************************************
  TempSensor.cc - Enables I2C communication between an Arduino and MCP9808 Temp Sensor
  Uncluden register read and write functions, as well as register bit manipulations.
  Also enables setting of specific sensor functions and reading specific registers.
  Created by Sierra Catelani, March 21, 2020.
*********************************************/
#include <Arduino.h>
#include "TempSensor.h"


/************************************
RegWrite() - Writes two byes of data to a specified register using I2C.
Inputs: reg = Target Register; data = two bytes of data to write.
return: none
*************************************/

void TempSensor::RegWrite(uint16_t reg, uint16_t data){

  //Access ambient temperature register of temperature sensor
  Wire.beginTransmission(TempSenseI2CAdd);
  
  //Set register offset to the Temp Sensor for writing
  Wire.write(reg);

  //Write two bytes of data to Temp Sensor (one at a time) and end transmission
  Wire.write(data>>8);
  Wire.write(data & 0xFF);
  Wire.endTransmission();

  //complete
  return;
}

/************************************
SetTargetReg() - Begins I2C transmission with Specific register for further reading.
Inputs: reg = Target Register.
return: none
*************************************/
void TempSensor::SetTargetReg(uint16_t reg){

  //Access ambient temperature register of temperature sensor
  Wire.beginTransmission(TempSenseI2CAdd);
  
  //Set register offset to the Temp Sensor for writing
  Wire.write(reg);
  Wire.endTransmission();
}

/************************************
RegSetBit() - Sets a bit of a specific register.
Inputs: reg = Target Register; bit = Target bit to set (0-15).
return: none
*************************************/
void TempSensor::RegSetBit(uint16_t reg, uint8_t bit0){

  //Read current data from register
  uint16_t registerContents = 0;
  registerContents = RegRead(reg);

  //Create bitmask
  uint16_t bitmask = 1;
  bitmask <<= bit0;
  bitmask = (bitmask) | registerContents;

  //Write bitmask
  RegWrite(reg,bitmask);
  
  //complete
  return;
}


/************************************
RegClearBit() - Clears a bit of a specific register.
Inputs: reg = Target Register; bit = Target bit to clear (0-15).
return: none
*************************************/
void TempSensor::RegClearBit(uint16_t reg, uint8_t bit0){

  //Read current data from register
  uint16_t registerContents = 0;
  registerContents = RegRead(reg);

  //Create bitmask
  uint16_t bitmask = 1;
  bitmask <<= bit0;
  bitmask = (~bitmask) & registerContents;
  
  //Write bitmask
  RegWrite(reg,bitmask);

  //complete
  return;
}

/************************************
RegCheckBit() - Checks and returns a bit of a specific register.
Inputs: reg = Target Register; bit = Target bit to check (0-15).
return: value of bit
*************************************/
bool TempSensor::RegCheckBit(uint16_t reg, uint8_t bit0){

  //Initialize variables
  uint16_t bitmask = 1;
  uint16_t regReturn, bitcheck = 0;

  regReturn = RegRead(reg);

  //Set bitmask to corresponding bit and check against register data
  bitmask <<= bit0;
  bitcheck = regReturn & bitmask;

  //return 1 if bitcheck != 0
  if(bitcheck == 0){ return 0;}
  else{return 1;}

}

/************************************
SetShutdownMode() - enable/disable shutdown(low-power) mode.
Inputs: 1 enables ShutdownMode, 0 disables ShutdownMode
return: none
*************************************/
void TempSensor::SetShutdownMode(bool enable){
  uint8_t shutdownBit = 8; 
  
  //Set or clear shutdown bit depending on enable value
  if(enable){RegSetBit(ConfigREG, shutdownBit);}
  else{RegClearBit(ConfigREG, shutdownBit);}
}



/************************************
RegRead_SingleByte() - Reads the least significan byte of a Register.
Inputs: reg = Target Register.
return: single byte read from register
*************************************/
uint8_t TempSensor::RegRead_SingleByte(uint16_t reg){

  //Begin I2C Transmission with Temperature Sensor
  SetTargetReg(reg);

  //request data from TempSense device
  Wire.requestFrom(TempSenseI2CAdd,1);

  //return byte from data read (returns one byte of data)
  return Wire.read();
  
}


/************************************
RegRead() - Reads full data of a specified register (two bytes).
Inputs: reg = target Register to be read from.
return: both bytes read from register.
*************************************/
uint16_t TempSensor::RegRead(uint16_t reg){

  uint16_t returndata;
  
  //Begin I2C Transmission with Temperature Sensor
  SetTargetReg(reg);

  //request data from TempSense device
  Wire.requestFrom(TempSenseI2CAdd,2);

  //create return data
  returndata = (Wire.read()<<8);
  returndata |= Wire.read();
    
  return returndata;
  
}


/************************************
ReadManufactID() - Reads manufacturing ID register
return: manufacturing ID.
*************************************/
uint16_t TempSensor::ReadManufactID(void){ return RegRead(ManufIDREG); }

/************************************
ReadDeviceIDREV() - Reads Device ID/Rev register
return: Device ID/Rev.
*************************************/
uint16_t TempSensor::ReadDeviceIDREV(void){ return RegRead(DevIDREG); }


/************************************
ReadTempValue() - Reads Device ID/Rev register
return: Reads temperature sensor and converts to Farenheit
*************************************/
float TempSensor::ReadTempValue(void){

  float temperature;
  uint8_t upperByte, lowerByte;

  //Access ambient temperature register of temperature sensor and clear flag bits
  SetTargetReg(T_TempReadREG);
  Wire.requestFrom(TempSenseI2CAdd,2);
  upperByte = Wire.read();
  lowerByte = Wire.read();
  upperByte &= 0x0F;

  //computer Temperature and convert to Fareinheit (upperbyte * 16 + lowerbyte/16 - from MCP9808 datasheet)
  temperature = (float(upperByte)*16 + (float(lowerByte)/16))*9/5 + 32;

  return temperature;
}

