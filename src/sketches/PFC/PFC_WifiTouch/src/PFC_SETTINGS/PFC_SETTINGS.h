#ifndef PFC_SETTINGS_h
#define PFC_SETTINGS_h

#include <WString.h>
#include "RunningAverage.h"
#include <EEPROM.h>


// Pin Mapping 
#define D0  3   // RX
#define D1  1   // TX
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



#define PH_PIN A0

#define DHTTYPE DHT11
#define DHTPIN D2

#define BASE_JSON "fields"
#define WEMOS_ADDR 8





// EEPROM Stuff
#define EEPROM_ADDRSTART_WIFI_SSID 0
#define EEPROM_ADDR_LENGTH_WIFI_SSID 255 // use the eeprom address to stor ethe length of the ssid

#define EEPROM_ADDRSTART_WIFI_PASSWORD 30
#define EEPROM_ADDR_LENGTH_WIFI_PASSWORD 254

#define EEPROM_ADDRSTART_USER 60
#define EEPROM_ADDR_LENGTH_USER 253

#define EEPROM_ADDRSTART_API_KEY 90
#define EEPROM_ADDR_LENGTH_API_KEY 252

#define EEPROM_ADDR_IS_WIFI_CONNECTED 251
#define EEPROM_ADDR_IS_LOGGED_IN 250

struct WiFiAP {
  String ssid;
  String password;
};

struct PathSettings{
  String server;
  String rootUpdate;
  String rootGetSettings;
  String rootGetManualSettings;
};

struct PeriodicSettings{
  unsigned long period;
  unsigned int timeout;
  unsigned long prevTime;
  bool currentAttemptSuccess;
  bool prevAttemptSuccess;
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

struct Actuator {
  String verbose;
  uint8_t pin;
  unsigned long period;
  float dutyCycle;
  unsigned long prevTime;
  bool isOn;
  bool activeHigh;
  String fieldName;
//  int intensity;
};

struct Auto {
  float desired;
  float threshold;
  String fieldName;
};

extern char* SERVER;
extern String SERVER2;

extern char* ROOT_UPDATE;
extern char* ROOT_GET_SETTINGS;
extern char* ROOT_GET_MANUAL_SETTINGS;

extern String PATH_UPDATE;
extern String PATH_GET_SETTINGS;
extern String PATH_GET_MANUAL_SETTINGS;

extern String API_KEY;
extern String USER;

extern const int NETWORK_TIMEOUT;
extern const int NETWORK_DELAY;

extern struct WiFiAP JohnWiFi;
extern struct WiFiAP AlvinWiFi; 
extern struct WiFiAP JohnWiFi; 
extern struct WiFiAP AlvinHotspot; 
extern struct WiFiAP ZulWiFi;
extern struct WiFiAP MakerWiFi;
extern struct WiFiAP UserWiFi;

extern struct PathSettings RootPaths;

extern struct PeriodicSettings SendToServer;
extern struct PeriodicSettings SendToServerFast; 

extern struct Sensor Moisture;
extern struct Sensor Light;
extern struct Sensor Humidity;
extern struct Sensor Temp;
extern struct Sensor PH;

extern struct Actuator Pump;
extern struct Actuator Led;
extern struct Actuator Fan;
extern struct Actuator Feeder;

extern struct Auto AutoWatering;
extern struct Auto AutoPh;
extern struct Auto AutoLight;

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

void writeSettings(String ssid, String password, String user, String apikey);
extern void writeSettings(String ssid, String password, String user, String apikey);

void loadSettings(String &ssid, String &password, String &user, String &apikey);
extern void loadSettings(String &ssid, String &password, String &user, String &apikey);


void writeWifiSettings(String ssid, String password);
extern void writeWifiSettings(String ssid, String password);

void loadWifiSettings(String &ssid, String &password);
extern void loadWifiSettings(String &ssid, String &password);


void writeUserSettings(String user, String apikey);
extern void writeUserSettings(String user, String apikey);

void loadUserSettings(String &user, String &apikey);
extern void loadUserSettings(String &user, String &apikey);

#endif