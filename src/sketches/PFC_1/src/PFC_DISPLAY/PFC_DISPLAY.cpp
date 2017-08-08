#include "PFC_DISPLAY.h"





int EEPROM_writeString(int addr_start, String strToWrite){
    
    for(int addr = addr_start; addr < (addr_start + strToWrite.length()); addr ++){
        
        EEPROM.write(addr, strToWrite[addr - addr_start]);
        EEPROM.commit();
    }
    return strToWrite.length();
}

String EEPROM_readString(int addr_start, int lengthStr){
	String retStr = "";
	for(int addr = addr_start; addr < (addr_start + lengthStr); addr ++){
	    int value = EEPROM.read(addr);
	    retStr += (char)value;
	}
	return retStr;
}

void EEPROM_writeString_storeLength(int addr_start, String strToWrite, int len_addr_bit){
	EEPROM_writeString(addr_start, strToWrite);
  	EEPROM.write(len_addr_bit, strToWrite.length());
  	EEPROM.commit();
}

String EEPROM_retrieveLen_readString(int addr_start, int len_addr_bit){
	int len = EEPROM.read(len_addr_bit);
  	String retStr = EEPROM_readString(addr_start, len);
  	return retStr;
}



void EEPROM_clear(){
	for (int i = 0 ; i < 256 ; i++) {
    	EEPROM.write(i, 0);
    	EEPROM.commit();
  	}
}