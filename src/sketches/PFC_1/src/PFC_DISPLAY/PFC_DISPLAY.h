#ifndef PFC_DISPLAY_h
#define PFC_DISPLAY_h
#include <EEPROM.h>
#include <Arduino.h>



class PFC_DISPLAY
{
    public:   
        String keys = "AaBbCcDdEe";


       

    private:
    
};

// WRITE

int EEPROM_writeString(int addr_start, String strToWrite);
extern int EEPROM_writeString(int addr_start, String strToWrite);

void EEPROM_writeString_storeLength(int addr_start, String strToWrite, int len_addr_bit);
extern void EEPROM_writeString_storeLength(int addr_start, String strToWrite, int len_addr_bit);


// READ

String EEPROM_readString(int addr_start, int lengthStr);
extern String EEPROM_readString(int addr_start, int lengthStr);

String EEPROM_retrieveLen_readString(int addr_start, int len_addr_bit);
extern String EEPROM_retrieveLen_readString(int addr_start, int len_addr_bit);


void EEPROM_clear();
extern void EEPROM_clear();

#endif