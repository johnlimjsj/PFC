#include "PFC.h"

WiFiClient client;
HttpClient http(client);
DHT dht(DHTPIN, DHTTYPE);
I2CSoilMoistureSensor soilSensor(0x21);

void setup() {
  Serial.begin(9600);
  dht.begin();
  soilSensorInit();
  initActuatorPins();
  connectWiFi(MakerWifi);
}

void loop() {
  
  if (isManualOverride()){
    manualActuateFromServer();
    String getStr = generateSensorGetStr();
    sendDataToServer(getStr);
  }
  else{
    Feeder = periodControl(Feeder);
    Pump = periodControl(Pump);
    Led = periodControl(Led);
    
    sendSensorReadingToServer(SendToServer);
    //getSettingsFromServer();  
  }
  
  
}

