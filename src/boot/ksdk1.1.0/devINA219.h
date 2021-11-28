void        initINA219(const uint8_t i2cAddress, uint16_t operatingVoltageMillivolts);
WarpStatus  writeSensorRegisterINA219(uint8_t deviceRegister, uint8_t payload_MSB, uint8_t payload_LSB);
WarpStatus  configureSensorINA219(uint8_t payloadCONFIG_MSB, uint8_t payloadCONFIG_LSB, uint8_t payloadCALIBRATE_MSB, uint8_t payloadCALIBRATE_LSB);
WarpStatus  readSensorRegisterINA219(uint8_t deviceRegister, int numberOfBytes);
void        printSensorDataINA219(bool hexModeFlag);
