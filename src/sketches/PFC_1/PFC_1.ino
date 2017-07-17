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
  connectWiFi(AlvinWifi);
}

void loop() {
  if (isManualOverride()){
    Serial.println("Manual mode");
    manualActuateFromServer();
  }
  else{
    Serial.println("Auto mode");
    Feeder = periodControl(Feeder);
    Pump = periodControl(Pump);
    Led = periodControl(Led);
    getSettingsFromServer();  
  }
  sendSensorReadingToServer();  
}

