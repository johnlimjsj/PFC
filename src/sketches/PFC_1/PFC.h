#ifndef CONSTANTS_h
#define CONSTANTS_h

#include "RunningAverage.h"
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include "DHT.h"
#include "I2CSoilMoistureSensor.h"
#include <ArduinoJson.h>
#include <Wire.h>
#include <HttpClient.h>

// Pin Mapping 
#define D0  3   // TX
#define D1  1   // RX
#define D2  16  
#define D3  5   // SCL
#define D4  4   // SDA
#define D5  14
#define D6  12
#define D7  13
#define D8  0
#define D9  2
#define D10 15
#define D11 13
#define D12 12
#define D13 14
#define D14 4
#define D15 5

#define PRINTLN Serial.println

#define FEEDER_PIN D8
#define LED_PIN D7
#define WATERPUMP_PIN D5
#define FAN_PIN D6
#define PH_PIN A0

#define DHTTYPE DHT11
#define DHTPIN D2

#define DESIRED_MOISTURE 60
#define THRESHOLD_MOISTURE 40

const char* SERVER = "www.lifeplanters.com";
const char* PATH = "/user/panel/update/Johan-Kok/c895ed0e43226ce/";
const char* PATH_GET_SETTINGS = "/user/panel/get-configuration/Johan-Kok/c895ed0e43226ce/"; 
const char* PATH_GET_MANUAL_SETTINGS = "/user/panel/get-configuration/Johan-Kok/c895ed0e43226ce/"; 
const String API_KEY = "H6VUB8650DYJZG0D";
const int NETWORK_TIMEOUT = 30*1000;
const int NETWORK_DELAY = 1000;

struct Actuator {
  String verbose;
  uint8_t pin;
  unsigned long period;
  float dutyCycle;
  unsigned long prevTime;
  bool isOn;
//  int intensity;
};

struct Sensor {
  String verbose;
  float reading;
  float minInput;
  float maxInput;
  float minOutput;
  float maxOutput;
  RunningAverage runavg;
};

struct Auto {
  float desired;
  float threshold;
  String fieldName;
};

struct WifiAP {
  char* ssid;
  char* password;
};

struct ClientSettings{
  unsigned long period;
  unsigned int timeout;
  unsigned long prevTime;
  bool currentAttemptSuccess;
  bool prevAttemptSuccess;
};

WifiAP AlvinWifi = {.ssid = "HUAT AH", .password = "AAAA-888-888" };
WifiAP JohnWifi = {.ssid = "dlink-3160", .password = "zdgwj81446" };
WifiAP AlvinHp = {.ssid = "Chewy", .password = "hengchew" };

Sensor Moisture = {.verbose="moisture", .reading=NULL, .minInput=236, .maxInput=655, .minOutput=0, .maxOutput=100, .runavg = RunningAverage(5)};
Sensor Light    = {.verbose="light", .reading=NULL, .minInput=236, .maxInput=60000, .minOutput=0, .maxOutput=100, .runavg = RunningAverage(5)};
Sensor Humidity = {.verbose="humidity", .reading=NULL, .minInput=0, .maxInput=100, .minOutput=0, .maxOutput=100, .runavg = RunningAverage(5)};
Sensor Temp     = {.verbose="temp", .reading=NULL, .minInput=0, .maxInput=100, .minOutput=0, .maxOutput=100, .runavg = RunningAverage(5)};
Sensor PH       = {.verbose="ph", .reading=NULL, .minInput=0, .maxInput=1023, .minOutput=0, .maxOutput=14, .runavg = RunningAverage(5)};

Actuator Pump   = {.verbose="pumpIsOn", .pin=WATERPUMP_PIN, .period=3600000, .dutyCycle=0.66, .prevTime=0, .isOn=false};
Actuator Led    = {.verbose="ledIsOn", .pin=LED_PIN, .period=3600000, .dutyCycle=0.33, .prevTime=0, .isOn=false};
Actuator Fan    = {.verbose="fanIsOn", .pin=FAN_PIN, .period=100000, .dutyCycle=0.5, .prevTime=0, .isOn=false};
Actuator Feeder = {.verbose="feederIsOn", .pin=FEEDER_PIN, .period=86400000, .dutyCycle=0.0005, .prevTime=0, .isOn=false};

ClientSettings SendToServer = { .period=30000, .timeout = 15000, .prevTime=0, .currentAttemptSuccess=false, .prevAttemptSuccess=false};

Auto AutoWatering = {.desired=60, .threshold=40, .fieldName="target_moisture" };
Auto AutoPh = {.desired=10.4, .threshold=10.4 };
Auto AutoLight = {.desired=34, .threshold=34, .fieldName="target_light" };

#endif
