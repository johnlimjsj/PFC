#include "ThingSpeak.h"
#include "CONSTANTS.h"
#include <ESP8266WiFi.h>
#include "DHT.h"
#include "I2CSoilMoistureSensor.h"
#include "RunningAverage.h"

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);
I2CSoilMoistureSensor soilSensor;

struct Actuator {
  uint8_t pin;
  unsigned long period;
  float dutyCycle;
  unsigned long prevTime;
//  int intensity;
};


struct Sensor {
  double reading;
  RunningAverage runavg;
};

Sensor Moisture = {.reading=NULL, .runavg = RunningAverage(5)};
Sensor Light    = {.reading=NULL, .runavg = RunningAverage(5)};
Sensor Humidity = {.reading=NULL, .runavg = RunningAverage(5)};
Sensor Temp     = {.reading=NULL, .runavg = RunningAverage(5)};

Actuator Pump = {.pin=WATERPUMP_PIN, .period=100000, .dutyCycle=0.5, .prevTime=0};
Actuator Led  = {.pin=LED_PIN, .period=100000, .dutyCycle=0.5, .prevTime=0};
Actuator Fan  = {.pin=FAN_PIN, .period=100000, .dutyCycle=0.5, .prevTime=0};

void setup() {
  Serial.begin(9600);
  dht.begin();

  
  // put your setup code here, to run once:
//  connectWiFi(SSID, PASSWORD);
}

void loop() {
  // put your main code here, to run repeatedly:
  periodControl(Pump);
  
  PRINTLN(Moisture.reading);
  


  
}

double doubleMap(double x, double in_min, double in_max, double out_min, double out_max)
{
 double temp = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
 temp = (int) (4*temp + .5);
 return (double) temp/4;
}

