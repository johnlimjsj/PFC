#include "PFC_WIRE.h"



void PFC_WIRE::manualSetPump(bool isOn){
	byte controlByte = isOn ? BYTE_MANUAL_SET_PUMP_ON : BYTE_MANUAL_SET_PUMP_OFF;
	I2C_helper::writeI2cRegister8Bit(_address, controlByte);
}


void PFC_WIRE::manualSetLight(bool isOn){
	byte controlByte = isOn ? BYTE_MANUAL_SET_LIGHT_ON : BYTE_MANUAL_SET_LIGHT_OFF;
	I2C_helper::writeI2cRegister8Bit(_address, controlByte);
}

void PFC_WIRE::manualSetFan(bool isOn){
	byte controlByte = isOn ? BYTE_MANUAL_SET_FAN_ON : BYTE_MANUAL_SET_FAN_OFF;
	I2C_helper::writeI2cRegister8Bit(_address, controlByte);
}

void PFC_WIRE::manualSetFeeder(bool isOn){
	byte controlByte = isOn ? BYTE_MANUAL_SET_FEEDER_ON : BYTE_MANUAL_SET_FEEDER_OFF;
	I2C_helper::writeI2cRegister8Bit(_address, controlByte);
}


float PFC_WIRE::_writeRequestFloat(byte controlByte){
	I2C_helper::writeI2cRegister8Bit(_address, controlByte);
	// delay(20);
	return I2C_helper::requestI2cFloat(_address);
}

int PFC_WIRE::_writeRequestInteger(byte controlByte){
	// write first. It will be received by micro controller
	I2C_helper::writeI2cRegister8Bit(_address, controlByte);
	// delay(20);
	// After microcontroller knows what was received, then request bytes accordingly
	return I2C_helper::requestI2cInteger(_address, 2);
}

byte PFC_WIRE::_writeRequestByte(byte controlByte){
	I2C_helper::writeI2cRegister8Bit(_address, controlByte);
	// delay(20);
	return I2C_helper::requestI2cInteger(_address, 1);
}