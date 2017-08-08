#include "PFC.h"
#include "src/PFC_WIFI/PFC_WIFI.h"
#include "src/PFC_SETTINGS/PFC_SETTINGS.h"
#include "src/PFC_DISPLAY/PFC_DISPLAY.h"





WiFiClient client;
HttpClient http(client);
DHT dht(DHTPIN, DHTTYPE);
I2CSoilMoistureSensor soilSensor(0x21);
PFC_WIFI pfcwifi;

void setup() {
  Serial.begin(9600);  
  dht.begin();
  soilSensorInit();
  initActuatorPins();
  
  // LCD load screen for inputting credentials if unset. Otherwise, load option to update the credentials within 10 seconds, if not proceed. 
  // Do LCD screen stuff. Return a string for the following: SSID, Password, user, apikey
  // Then write the above items to EEPROM
  EEPROM.begin(512);

  String ssid;
  String password;
  String user;
  String apikey;

  
  ssid = "dlink-3160_EXT";
  password = "zdgwj81446";
  user = "johan-kok";
  apikey = "891b2d8285354ed";
  
  EEPROM_writeString_storeLength(EEPROM_ADDRSTART_WIFI_SSID, ssid, EEPROM_ADDR_LENGTH_WIFI_SSID);
  EEPROM_writeString_storeLength(EEPROM_ADDRSTART_WIFI_PASSWORD, password, EEPROM_ADDR_LENGTH_WIFI_PASSWORD);
  EEPROM_writeString_storeLength(EEPROM_ADDRSTART_USER, user, EEPROM_ADDR_LENGTH_USER);
  EEPROM_writeString_storeLength(EEPROM_ADDRSTART_API_KEY, apikey, EEPROM_ADDR_LENGTH_API_KEY);



// Retrieve the user particulars from EEPROM on setup  
  ssid = EEPROM_retrieveLen_readString(EEPROM_ADDRSTART_WIFI_SSID, EEPROM_ADDR_LENGTH_WIFI_SSID);
  password = EEPROM_retrieveLen_readString(EEPROM_ADDRSTART_WIFI_PASSWORD, EEPROM_ADDR_LENGTH_WIFI_PASSWORD);
  user = EEPROM_retrieveLen_readString(EEPROM_ADDRSTART_USER, EEPROM_ADDR_LENGTH_USER);
  apikey = EEPROM_retrieveLen_readString(EEPROM_ADDRSTART_API_KEY, EEPROM_ADDR_LENGTH_API_KEY);

  
  pfcwifi.initPathSettings(RootPaths, user, apikey);
  pfcwifi.initPeriodicSettings(SendToServer);
//  pfcwifi.connectWiFi(ssid, password);
  

  

}

void loop() {
  
  return;
  
  if (isManualOverride()){
    manualActuateFromServer();
    String getStr = generateSensorGetStr();
    // pfcwifi.sendDataToServer(getStr);
  }
  else{
    Feeder = periodControl(Feeder);
    Pump = periodControl(Pump);
    Led = periodControl(Led);
    
    sendSensorReadingToServer();
    //getSettingsFromServer();  
  }
  
  
}

