/********************************************
  SunlightSensor.h - Reads data from an SI1145 Sunlight Sensor
  Created by Sierra Catelani, March 21, 2020.
*********************************************/

#ifndef SunlightSensor_h
#define SunlightSensor_h

#include <Wire.h>
#include <Arduino.h>

/******** I2C Targets ********/
#define PhotoDetI2CAdd 0x60

/******** Command Register CMDs ********/
#define CMD_NOP 0x00
#define CMD_RESET 0x01
#define CMD_GETCAL 0x12
#define CMD_ALSFORCE 0x06
#define CMD_PARAM_QUERY 0x80
#define CMD_PARAM_SET 0xA0

/******** Response Register Error Codes ********/
#define ALS_VIS_ADC_OVERFLOW 0x8C
#define ALS_IR_ADC_OVERFLOW 0x8D

/******** I2C Registers ********/
#define REG_HW_KEY 0x07
#define REG_MEAS_RATE0 0x08
#define REG_MEAS_RATE1 0x09
#define REG_COMMAND 0x18
#define REG_RESPONSE 0x20
#define REG_ALS_VIS_DATA0 0x22
#define REG_ALS_VIS_DATA1 0x23
#define REG_ALS_IR_DATA0 0x24
#define REG_ALS_IR_DATA1 0x25
#define REG_PARAM_WR 0x17
#define REG_PARAM_RD 0x2E

/******** RAM Offset ********/
#define RAM_CHLIST 0x01
#define RAM_ALS_IR_ADC_MUX 0x0E
#define RAM_ALS_VIS_ADC_COUNTER 0x10
#define RAM_ALS_VIS_ADC_GAIN 0x11
#define RAM_ALS_VIS_ADC_MISC 0x12
#define RAM_ALS_IR_ADC_COUNTER 0x1D
#define RAM_ALS_IR_ADC_GAIN 0x1E
#define RAM_ALS_IR_ADC_MISC 0x1F


class SunlightSensor{
  public:
    void RegWrite(uint8_t reg, uint8_t data);
    uint8_t RegRead(uint8_t reg);
    void DumpI2CRegs(void);
    void RegSetBit(uint8_t reg, uint8_t bit0);
    void RegClearBit(uint8_t reg, uint8_t bit0);
    void ClearResponseCMD(void);
    void SWResetCMD(void);
    void GetCalDataCMD(void);
    void MeasureALSCMD(void);
    void SetHWKEY(uint8_t value = 0x17);
    void SetMeasRate(uint8_t byte0, uint8_t byte1);
    void RAMSET(uint8_t offset, uint8_t data);
    uint8_t RAMQUERY(uint8_t offset);
    void EnProximatySensors(bool enable);
    void EnALSSensors(bool enable);
    void EnUVSensor(bool enable);
    void EnAUXSensor(bool enable);
    uint16_t ReadAmbVisData(void);
    uint16_t ReadAmbIRData(void);
};

#endif
