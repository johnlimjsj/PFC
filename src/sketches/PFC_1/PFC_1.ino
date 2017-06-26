#include "PFC.h"

WiFiClient client;
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

  
  Feeder = periodControl(Feeder);
  Pump = periodControl(Pump);
  Led = periodControl(Led);
  WeMos = periodControl(WeMos);
  if (WeMos.isOn) { 
    sendSensorReadingToServer(); 
    getSensorReadings(); 
  }
  Serial.print("Feeder: ");
  Serial.println(Feeder.isOn);
  
  //sendSensorReadingToServer();
  //getSettingsFromServer();
  delay(1000);
  
}

