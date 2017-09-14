#ifndef PFC_WIRE_h
#define PFC_WIRE_h

#include <Arduino.h>
#include <Wire.h>
#include <I2C_helper.h>

#define BYTE_SLAVE_AUTO 1
#define BYTE_SLAVE_MANUAL 2

#define BYTE_MANUAL_SET_PUMP_OFF 10
#define BYTE_MANUAL_SET_PUMP_ON 20
#define BYTE_MANUAL_SET_LIGHT_OFF 11
#define BYTE_MANUAL_SET_LIGHT_ON 21
#define BYTE_MANUAL_SET_FAN_OFF 12
#define BYTE_MANUAL_SET_FAN_ON 22
#define BYTE_MANUAL_SET_FEEDER_OFF 13
#define BYTE_MANUAL_SET_FEEDER_ON 23

#define BYTE_GET_CURR_MOISTURE 40
#define BYTE_GET_CURR_LIGHT 41
#define BYTE_GET_CURR_TEMP 42
#define BYTE_GET_CURR_PH 43
#define BYTE_GET_CURR_HUMIDITY 44

#define BYTE_GET_FILTERED_MOISTURE 50
#define BYTE_GET_FILTERED_LIGHT 51
#define BYTE_GET_FILTERED_TEMP 52
#define BYTE_GET_FILTERED_PH 53
#define BYTE_GET_FILTERED_HUMIDITY 54



class PFC_WIRE{
	public:
		PFC_WIRE(int addr){
			_address = addr;
		}

		// Sending commands to slave. return true if success
		void setSlaveToAuto();
		void setSlaveToManual();
		
		void manualSetPump(bool isOn);
		void manualSetLight(bool isOn);
		void manualSetFan(bool isOn);
		void manualSetFeeder(bool isOn);

		// Getting readings from slave. requestFrom
		byte getCurrMoistureFromSlave(){ 	return _writeRequestByte(BYTE_GET_CURR_MOISTURE); };
		byte getCurrLightFromSlave(){ 		return _writeRequestByte(BYTE_GET_CURR_LIGHT); };
		byte getCurrTempFromSlave(){ 		return _writeRequestByte(BYTE_GET_CURR_TEMP); };
		byte getCurrPhFromSlave(){ 			return _writeRequestByte(BYTE_GET_CURR_PH); };
		byte getCurrHumidityFromSlave(){ 	return _writeRequestByte(BYTE_GET_CURR_HUMIDITY); };

		byte getFilteredMoistureFromSlave(){	return _writeRequestByte(BYTE_GET_FILTERED_MOISTURE); };
		byte getFilteredLightFromSlave(){		return _writeRequestByte(BYTE_GET_FILTERED_LIGHT); };
		byte getFilteredTempFromSlave(){		return _writeRequestByte(BYTE_GET_FILTERED_TEMP); };
		byte getFilteredPhFromSlave(){			return _writeRequestByte(BYTE_GET_FILTERED_PH); };
		byte getFilteredHumidityFromSlave(){	return _writeRequestByte(BYTE_GET_FILTERED_HUMIDITY); };

	private:
		int _address;
		float _writeRequestFloat(byte controlByte);
		int _writeRequestInteger(byte controlByte);
		byte _writeRequestByte(byte controlByte);
};



#endif