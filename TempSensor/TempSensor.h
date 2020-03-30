#ifndef TempSensor_h
#define TempSensor_h

#include <Wire.h>


#define TempSenseI2CAdd 0x18

#define ConfigREG 0x1
#define T_UpperBoundREG 0x2
#define T_LowerBoundREG 0x3
#define T_CriticalREG 0x4
#define T_TempReadREG 0x5
#define ManufIDREG 0x6
#define DevIDREG 0x7
#define ResolutionREG 0x8



class TempSensor{
public:
  void RegWrite(uint16_t reg, uint16_t data);
  void SetTargetReg(uint16_t reg);
  void RegSetBit(uint16_t reg, uint8_t bit0);
  void RegClearBit(uint16_t reg, uint8_t bit0);
  bool RegCheckBit(uint16_t reg, uint8_t bit0);
  void SetShutdownMode(bool enable);
  uint8_t RegRead_SingleByte(uint16_t reg);
  uint16_t RegRead(uint16_t reg);
  uint16_t ReadManufactID(void);
  uint16_t ReadDeviceIDREV(void);
  float ReadTempValue(void);
};

#endif