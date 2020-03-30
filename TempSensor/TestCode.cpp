/***
Test Script for Temp Sensor
***/
/*
int TS_TestScript(void){

  int testValue1 = 0x0004;
  int testValue2 = 0x0000;
  int testValue3 = 0x0010;
  int testValue4 = 0x0054;
  int testValue5 = 0x0400;

  int registerValue;
  int bitReturn;
  int failFlag = 0;

  //Begin Test Script
  Serial.print("*****Running Temp Sense Test Script*****\n\n");


  //Test TS_RegSetBit
  TS_RegSetBit(T_UpperBoundREG,2);
  Serial.print("Running TS_RegSetBit Test\n");

  bitReturn = TS_RegCheckBit(T_UpperBoundREG, 2);
  registerValue = TS_RegDump(T_UpperBoundREG);

  if(bitReturn == 1){
    Serial.print("Success - RegSetBit =");
    Serial.print(bitReturn);
    Serial.print("\n");
  }
  else{
    Serial.print("Fail - Expected RegSetBit =");
    Serial.print(1);
    Serial.print("\nActual Value =");
    Serial.print(bitReturn);
    Serial.print("\n");
    failFlag = 1;
  }
    Serial.print("Full Register =");
    Serial.print(registerValue, BIN);
    Serial.print("\n\n");


  //Test TS_RegClearBit
  TS_RegClearBit(T_UpperBoundREG,2);
  Serial.print("Running TS_RegClearBit Test\n");
  
  registerValue = TS_RegCheckBit(T_UpperBoundREG, 2);

  if(registerValue == 0){
    Serial.print("Success - RegSetBit =");
    Serial.print(registerValue);
    Serial.print("\n");
  }
  else{
    Serial.print("Fail - Expected RegSetBit =");
    Serial.print(testValue2);
    Serial.print("\nActual Value =");
    Serial.print(registerValue);
    Serial.print("\n");
    failFlag = 1;
  }  
    Serial.print("Full Register =");
    Serial.print(registerValue, BIN);
    Serial.print("\n\n");

  
  //Test TS_RegWrite
  TS_RegWrite(T_UpperBoundREG, testValue3);
  Serial.print("Running TS_RegWrite Test\n");

  registerValue = TS_RegDump(T_UpperBoundREG);

    if(registerValue == testValue3){
    Serial.print("Success - RegSetBit =");
    Serial.print(registerValue, BIN);
    Serial.print("\n\n");
  }
  else{
    Serial.print("Fail - Expected RegSetBit =");
    Serial.print(testValue3);
    Serial.print("\nActual Value =");
    Serial.print(registerValue, BIN);
    Serial.print("\n\n");
    failFlag = 1;
  }

  //Test TS_ReadManufactID
  
  registerValue = TS_ReadManufactID();
  Serial.print("Running TS_ReadManufactID Test\n");
  
  if(registerValue == testValue4){
    Serial.print("Success - ManufactID Register=");
    Serial.print(registerValue, BIN);
    Serial.print("\n\n");
  }
  else{
    Serial.print("Fail - Expected ManufactID Register=");
    Serial.print(testValue4);
    Serial.print("\nActual Value =");
    Serial.print(registerValue, BIN);
    Serial.print("\n\n");
    failFlag = 1;
  }


  //Test TS_Read_DevIDREV
  registerValue = TS_Read_DeviceIDREV();
  Serial.print("Running TS_Read_DevIDREV Test \n");
  
  if(registerValue == testValue5){
    Serial.print("Success - DeviceIDREV Register =");
    Serial.print(registerValue, BIN);
    Serial.print("\n\n");
  }
  else{
    Serial.println("Fail - Expected DeviceIDREV Register = " + testValue5);
    Serial.println("Actual Value = ");
    Serial.print(registerValue, BIN);
    Serial.print("\n\n");
    failFlag = 1;
  }

  //Test TS_SetShutdownMode
  
  Serial.print("Running TS_SetShutdownMode Test \n");

  Serial.print("Enable Test \n");
  TS_SetShutdownMode(1);
  registerValue = TS_RegDump(ConfigREG);
  bitReturn = TS_RegCheckBit(ConfigREG, 8);
  
  
  if(bitReturn == 1){
    Serial.print("Success - ShutdownBit = 1\n");
  }
  else{
    Serial.print("Fail - Expected ShutdownBit = 1\n");
    Serial.print("Actual Value = 0\n");
    failFlag = 1;
  }  
    Serial.print("Full Register = ");
    Serial.print(registerValue, BIN);
    Serial.print("\n\n");

  Serial.print("Disable Test \n");
  
  TS_SetShutdownMode(0);
  registerValue = TS_RegDump(ConfigREG);
  bitReturn = TS_RegCheckBit(ConfigREG, 8);
  
  if(bitReturn == 0){
    Serial.print("Success - ShutdownBit = 0\n");
  }
  else{
    Serial.print("Fail - Expected ShutdownBit = 0\n");
    Serial.print("Actual Value = 1\n");
    failFlag = 1;
  }  
    Serial.print("Full Register = ");
    Serial.print(registerValue, BIN);
    Serial.print("\n\n");

  
  return failFlag;
}
*/