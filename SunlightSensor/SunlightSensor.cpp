
/********************************************
  SunlightSensor.cc - Enables I2C communication between an Arduino and SI1145 Sunlight Sensor
  Includes register read and write functions, as well as register bit manipulations.
  Also enables configuring hardware, and accessing on-chip RAM for full sensor access.
  Created by Sierra Catelani, March 21, 2020.
*********************************************/

#include <Arduino.h>
#include "SunlightSensor.h"

/**************************************************************/
/*--------------- I2C Transaction Functions ------------------*/
/**************************************************************/

/************************************
RegWrite() - Writes one byte of data to a specified register using I2C.
Inputs: reg = Target Register; data = byte of data to write.
return: none
*************************************/
void SunlightSensor::RegWrite(uint8_t reg, uint8_t data){

  //Access ambient temperature register of temperature sensor
  Wire.beginTransmission(PhotoDetI2CAdd);
  
  //Set register offset to the Temp Sensor for writing
  Wire.write(reg);

  //Write two bytes of data to Temp Sensor and end transmission
  Wire.write(data);
  Wire.endTransmission();

  //complete
  return;
}

/************************************
RegRead() - Reads one byte of data to a specified register using I2C.
Inputs: reg = Target Register.
return: none
*************************************/
uint8_t SunlightSensor::RegRead(uint8_t reg){
  
  uint8_t returnData;

  //Begin I2C Transmission with Temperature Sensor
  Wire.beginTransmission(PhotoDetI2CAdd);
  
  //Set register offset to the Sunlight Sensor for reading (bitmask enables non-)
  Wire.write(reg | 0b01000000);
  Wire.endTransmission();

  //Wire.beginTransmission(PhotoDetI2CAdd);
  //request data from TempSense device
  Wire.requestFrom(PhotoDetI2CAdd,1);

  //return byte from data read (returns one byte of data)
  returnData = Wire.read();
    //Wire.endTransmission();

  
  return returnData;
}

/************************************
DumpI2CRegs() - Dumps full register map for debugging purposes.
Inputs: none
return: none
*************************************/
void SunlightSensor::DumpI2CRegs(void){
  
  //Begin I2C Transmission with Sunlight Sensor
  Wire.beginTransmission(PhotoDetI2CAdd);
  Wire.write(0x00);
  Wire.endTransmission();

  //Loop through full register map
  for(int i=0;i<0x3F;i++){

    Serial.print("REGISTER ");
    Serial.print(i, HEX);
    Serial.print(":   ");
    
    
    //read and print data each Sunlight register
    Wire.requestFrom(PhotoDetI2CAdd,1);
    Serial.print(Wire.read(), HEX);
    Serial.print("\n");
    delay(500);
  }
   
  //complete
  return;
}

/************************************
RegSetBit() - Sets a bit of a specific register.
Inputs: reg = Target Register; bit = Target bit to set (0-7).
return: none
*************************************/
void SunlightSensor::RegSetBit(uint8_t reg, uint8_t bit0){

  //Read current data from register
  uint8_t registerContents = 0;
  registerContents = RegRead(reg);

  //Create bitmask
  uint8_t bitmask = 1;
  bitmask <<= bit0;
  bitmask = (bitmask) | registerContents;

  RegWrite(reg,bitmask);
  
  //complete
  return;
}

/************************************
RegClearBit() - Clears a bit of a specific register.
Inputs: reg = Target Register; bit0 = Target bit to set (0-7).
return: none
*************************************/
void SunlightSensor::RegClearBit(uint8_t reg, uint8_t bit0){

  //Read current data from register
  uint8_t registerContents = 0;
  registerContents = RegRead(reg);

  //Create bitmask
  uint8_t bitmask = 1;
  bitmask <<= bit0;
  bitmask = (~bitmask) & registerContents;
  
  //Access ambient temperature register of temperature sensor
  RegWrite(reg,bitmask);

  //complete
  return;
}

/*************************************************************/
/*--------------- Command Register Functions ----------------*/
/*************************************************************/

//These functions write commands to the COMMAND register
//Which initiate specific hardware functions

/************************************
ClearResponseCMD() - Send NOP command - clears the Response register.
Inputs: none
return: none
*************************************/
void SunlightSensor::ClearResponseCMD(void){
  RegWrite(REG_COMMAND,CMD_NOP);
  return;
}

/************************************
ClearResponseCMD() - Send RESET Command - resets sensor firmware.
Inputs: none
return: none
*************************************/
void SunlightSensor::SWResetCMD(void){
  RegWrite(REG_COMMAND,CMD_RESET);
  return;
}

/************************************
ClearResponseCMD() - Send GETCAL Command - reports calibration data to registers 0x22-0x2D
Inputs: none
return: none
*************************************/
void SunlightSensor::GetCalDataCMD(void){
  RegWrite(REG_COMMAND,CMD_GETCAL);
  return;
}

/************************************
ClearResponseCMD() - Send CMD_ALSFORCE Command - forces a visible and IR light reading
Inputs: none
return: none
*************************************/
void SunlightSensor::MeasureALSCMD(void){
  RegWrite(REG_COMMAND,CMD_ALSFORCE);
  return;
}



/************ Response Register Functions ***********/


/***************************************************************/
/*--------------- Specific I2C Reg Functions ------------------*/
/***************************************************************/


/************************************
SetHWKey() - Required to set HW_KEY REG to 0x17 for proper operation
Inputs: HW_KEY
return: none
*************************************/
void SunlightSensor::SetHWKEY(uint8_t value){ 
  RegWrite(REG_HW_KEY,value);
  return;
}

/************************************
SetHWKey() - Set MEAS_RATE0 and MEAS_RATE1 registers. For forced mode(polling mode), both need to be set to 0x00.
Inputs: byte0 -> written to REG_MEAS_RATE0; byte1 -> written to REG_MEAS_RATE1
return: none
*************************************/
void SunlightSensor::SetMeasRate(uint8_t byte0, uint8_t byte1){
  RegWrite(REG_MEAS_RATE0,byte0);
  RegWrite(REG_MEAS_RATE1,byte1);
  return;
}

/**************************************************************/
/*------------------ RAM Access Functions --------------------*/
/**************************************************************/


/************************************
RAMSET() - Write to the on-chip RAM, which is used to configuring the sensor ADC and other sensor read features.
Inputs: offset -> RAM target offset (sets location to write to); data -> data to write to the RAM target address.
return: none
*************************************/
void SunlightSensor::RAMSET(uint8_t offset, uint8_t data){

  //Set RAMTarget (command = CMD_PARAM_SET | offset)
  uint8_t RAMTarget = (CMD_PARAM_SET | offset);
  
  //Write data to be written to RAMTarget to REG_PARAM_WR
  RegWrite(REG_PARAM_WR, data);

  //Write RAMTarget to COMMAND register
  RegWrite(REG_COMMAND,RAMTarget);

  return;
  
}

/************************************
RAMQUERY() - Read from the on-chip RAM, which is used to configuring the sensor ADC and other sensor read features.
Inputs: offset -> RAM target offset (sets location to read from)
return: data read from the RAM target address.
*************************************/
uint8_t SunlightSensor::RAMQUERY(uint8_t offset){

  uint8_t returnValue;
  
  //Set RAMTarget (command = CMD_PARAM_QUERY | offset)
  uint8_t RAMTarget = (CMD_PARAM_QUERY | offset);
  
  //Write RAMTarget to COMMAND register
  RegWrite(REG_COMMAND,RAMTarget);
  
  //Read data output from REG_PARAM_RD
  returnValue = RegRead(REG_PARAM_RD);

  return returnValue;
  
}


/**********************************************************/
/*------------ Specific RAM Access Functions -------------*/
/**********************************************************/

/************************************
EnProximatySensors() - Enables/Disables the on-chip proximatey sensors.
Inputs: 1 enables sensors; 0 disables sensors
return: none
*************************************/
void SunlightSensor::EnProximatySensors(bool enable){
  
  uint8_t RAMContents = 0;
  uint8_t regSetData = 0;

  //Read CHLIST (channel list) RAM Data
  RAMContents = RAMQUERY(RAM_CHLIST);

  //Set or clear the proximatey sensor control bits
  if (enable == 0){ regSetData = RAMContents & 0xF8;}
  else { regSetData = RAMContents | 0x07;}

  //Write new CHLIST to RAM memory
  RAMSET(RAM_CHLIST,regSetData);

  return;
}


/************************************
EnALSSensors() - Enables/Disables the on-chip Ambient Light sensors.
Inputs: 1 enables sensors; 0 disables sensors
return: none
*************************************/
void SunlightSensor::EnALSSensors(bool enable){
  
  uint8_t RAMContents = 0;
  uint8_t regSetData = 0;

  //Read CHLIST (channel list) RAM Data
  RAMContents = RAMQUERY(RAM_CHLIST);

  //Set or clear the ALS sensor control bits
  if (enable == 0){ regSetData = RAMContents & 0xCF;}
  else { regSetData = RAMContents | 0x30;}

  //Write new CHLIST to RAM memory
  RAMSET(RAM_CHLIST,regSetData);

  return;
}

/************************************
EnUVSensors() - Enables/Disables the on-chip UV Light sensors.
Inputs: 1 enables sensors; 0 disables sensors
return: none
*************************************/
void SunlightSensor::EnUVSensor(bool enable){
  
  uint8_t RAMContents = 0;
  uint8_t regSetData = 0;

  //Read CHLIST (channel list) RAM Data
  RAMContents = RAMQUERY(RAM_CHLIST);

  //Set or clear the UV sensor control bit
  if (enable == 0){ regSetData = RAMContents & 0x7F;}
  else { regSetData = RAMContents | 0x80;}

  //Write new CHLIST to RAM memory
  RAMSET(RAM_CHLIST,regSetData);

  return;
}

/************************************
EnUVSensors() - Enables/Disables the on-chip AUX Light sensors.
Inputs: 1 enables sensors; 0 disables sensors
return: none
*************************************/
void SunlightSensor::EnAUXSensor(bool enable){
  
  uint8_t RAMContents = 0;
  uint8_t regSetData = 0;

  //Read CHLIST (channel list) RAM Data
  RAMContents = RAMQUERY(RAM_CHLIST);

  //Set or clear the AUX sensor control bit
  if (enable == 0){ regSetData = RAMContents & 0xBF;}
  else { regSetData = RAMContents | 0x40;}

  //Write new CHLIST to RAM memory
  RAMSET(RAM_CHLIST,regSetData);

  return;
}



/**********************************************************/
/*-------------- Measurement Read Functions --------------*/
/**********************************************************/


/************************************
ReadAmbVisData() - Read data from Ambient Visible light Registers.
return: uint16_t -> two byes of data, one from each Amb Vis reg.
*************************************/
uint16_t SunlightSensor::ReadAmbVisData(void){
  uint16_t measReturn = 0;

  //read data from registers and create return data
  measReturn = (RegRead(REG_ALS_VIS_DATA0) );
  measReturn |= (RegRead(REG_ALS_VIS_DATA1)<< 8);

  //complete and return
  return measReturn;
}

/************************************
ReadAmbIRData() - Read data from Ambient IR Registers.
return: uint16_t -> two byes of data, one from each Amb IR reg.
*************************************/

uint16_t SunlightSensor::ReadAmbIRData(void){
  uint16_t measReturn = 0;

  //read data from registers and create return data
  measReturn = RegRead(REG_ALS_IR_DATA0);
  measReturn |= (RegRead(REG_ALS_IR_DATA1) << 8);

  //complete and return
  return measReturn;
}

