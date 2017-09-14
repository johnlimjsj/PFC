#include "PFC.h"

// create shift register object (number of shift registers, data pin, clock pin, latch pin)
ShiftRegister74HC595 *sr; 
ShiftRegister74HC595 sr2 (1, D4, D0, D3); 
WiFiClient client;
HttpClient http(client);
PFC_WIRE pfcWire(8);

void setup() {
  // Initializing as Master 
  Wire.begin();  
  EEPROM.begin(512);
  Serial.begin(9600);  
  setupScreenAndWifi();
}

void loop() { 
  byte moisture = pfcWire.getCurrMoistureFromSlave();
  byte light = pfcWire.getCurrLightFromSlave();
  byte ph = pfcWire.getCurrPhFromSlave();


//  Serial.println(moisture);
//  Serial.println(light);
//  Serial.println(ph);
  
  pfcDisplay.setMoisture((float)moisture);
  pfcDisplay.setLight((float)light);
  pfcDisplay.setPh(((float)ph)/10);

  
  pfcDisplay.displayScreenByState();
  pfcDisplay.clickEventsByScreen();
  pfcDisplay.detectScreenSwitch();

  

  

  
  

}


















void loopReadings(){
  //  pfcWire.manualSetPump(true);
  byte filteredmoisture = pfcWire.getFilteredMoistureFromSlave();
  Serial.print("filtered moisture:");
  Serial.println(filteredmoisture);
  delay(1000);
  byte moisture = pfcWire.getCurrMoistureFromSlave();
  Serial.print("moisture:");
  Serial.println(moisture);
  delay(1000);
}


void loopA(){
  if (isManualOverride()){
    manualActuateFromServer();
//    String getStr = generateSensorGetStr();
    // pfcwifi.sendDataToServer(getStr);
  }
  else{
//    Feeder = periodControl(Feeder);
//    Pump = periodControl(Pump);
//    Led = periodControl(Led);
    
//    sendSensorReadingToServer();
    //getSettingsFromServer();  
  } 
}

void loopScreen(){
  pfcDisplay.displayScreenByState();
  pfcDisplay.clickEventsByScreen();
  pfcDisplay.detectScreenSwitch();
}

void setupScreenAndWifi(){
      
  //********** End Touch Screen Stuff
  
  // LCD load screen for inputting credentials if unset. Otherwise, load option to update the credentials within 10 seconds, if not proceed. 
  // Do LCD screen stuff. Return a string for the following: SSID, Password, user, apikey
  // Then write the above items to EEPROM
  
  String ssid; // = "GodIsGracious";
  String password; // = "zdgwj81446";
  String user; // = "johan-kok";
  String apikey; // = "891b2d8285354ed";
  
  loadSettings(ssid, password, user, apikey);
  Serial.print("pw:");
  Serial.println(password);
  pfcwifi.initPathSettings(RootPaths, user, apikey);
  pfcwifi.initPeriodicSettings(SendToServer);
//  pfcwifi.connectWiFi(ssid, password);  

  

  
  
  
  
  
  //********** Touch Screen Stuff
  pfcDisplay.initDisplay();  
  // over here, should attempt to login if it detects that the user had made a successful login
  
  // over here, should attempt to connect to the wifi network using the stored eeprom ssid and password if user had made a successful wifi connection
  // restore saved wifi session
  
  int status = EEPROM.read(EEPROM_ADDR_IS_WIFI_CONNECTED);
  if (status == pfcDisplay.DISCONNECTED) return;
  if (status == pfcDisplay.WIFI_CONNECTED){
    pfcDisplay.connectWifiFromEEPROMAndDisplay();
  }
  
}

void connectOnDisplayButtonPress(){
  
  if (pfcDisplay.isTryingToConnect){
    pfcDisplay.connectWifiFromEEPROMAndDisplay();
  }
  
}


void userLoginOnButtonPress(){
  String ssid; // = "GodIsGracious";
  String password; // = "zdgwj81446";
  String user; // = "johnlimjsj";
  String apikey; // = "passwordjohn";
  loadSettings(ssid, password, user, apikey);
  pfcDisplay.currWifiState = pfcDisplay.CONNECTING;
  
  
}
