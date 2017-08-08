#include "PFC_SETTINGS.h"


char* SERVER = "www.lifeplanters.com";
String SERVER2 = "www.lifeplanters.com";
char* ROOT_UPDATE = "/user/panel/update/";
char* ROOT_GET_SETTINGS = "/user/panel/get-configuration/";
char* ROOT_GET_MANUAL_SETTINGS = "/user/panel/main-view/get-manual-override/";

String API_KEY = "891b2d8285354ed";
String USER = "johan-kok";

String PATH_UPDATE = "/user/panel/update/johan-kok/891b2d8285354ed/";
String PATH_GET_SETTINGS = "/user/panel/get-configuration/johan-kok/891b2d8285354ed/"; 
String PATH_GET_MANUAL_SETTINGS = "/user/panel/main-view/get-manual-override/johan-kok/891b2d8285354ed/"; 


const int NETWORK_TIMEOUT= 30*1000;
const int NETWORK_DELAY= 500;


struct WiFiAP AlvinWiFi = {.ssid = "HUAT AH", .password = "AAAA-888-888" };
struct WiFiAP JohnWiFi = {.ssid = "dlink-3160_EXT", .password = "zdgwj81446" };
struct WiFiAP AlvinHotspot = {.ssid = "Chewy", .password = "hengchew" };
struct WiFiAP ZulWiFi = {.ssid = "3MobileWiFi-2537", .password = "10119784" };
struct WiFiAP MakerWiFi = {.ssid = "maker_fare exhibitor", .password = "2017mfex" };
struct WiFiAP UserWiFi = {.ssid = "", .password = "" };


struct PathSettings RootPaths = {
	.server = "www.lifeplanters.com",
	.rootUpdate = "/user/panel/update/",
	.rootGetSettings = "/user/panel/get-configuration/",
	.rootGetManualSettings = "/user/panel/main-view/get-manual-override/"
}; 

struct PeriodicSettings SendToServer = { .period=900000, .timeout = 300000, .prevTime=0, .currentAttemptSuccess=false, .prevAttemptSuccess=false};
struct PeriodicSettings SendToServerFast = { .period=5000, .timeout = 5000, .prevTime=0, .currentAttemptSuccess=false, .prevAttemptSuccess=false};



struct Sensor Moisture = {.verbose="moisture", .reading=NULL, .minInput=236, .maxInput=655, .minOutput=0, .maxOutput=100, .runavg = RunningAverage(5)};
struct Sensor Light    = {.verbose="light", .reading=NULL, .minInput=236, .maxInput=60000, .minOutput=0, .maxOutput=100, .runavg = RunningAverage(5)};
struct Sensor Humidity = {.verbose="humidity", .reading=NULL, .minInput=0, .maxInput=100, .minOutput=0, .maxOutput=100, .runavg = RunningAverage(5)};
struct Sensor Temp     = {.verbose="temp", .reading=NULL, .minInput=0, .maxInput=100, .minOutput=0, .maxOutput=100, .runavg = RunningAverage(5)};
struct Sensor PH       = {.verbose="ph", .reading=NULL, .minInput=0, .maxInput=1023, .minOutput=0, .maxOutput=14, .runavg = RunningAverage(5)};

struct Actuator Pump   = {.verbose="pumpIsOn", .pin=D5, .period=3600000, .dutyCycle=0.66, .prevTime=0, .isOn=false, .activeHigh=false, .fieldName="set_pump"};
struct Actuator Led    = {.verbose="ledIsOn", .pin=D7, .period=3600000, .dutyCycle=0.33, .prevTime=0, .isOn=false, .activeHigh=false, .fieldName="set_LED"};
struct Actuator Fan    = {.verbose="fanIsOn", .pin=D6, .period=100000, .dutyCycle=0.5, .prevTime=0, .isOn=false, .activeHigh=true, .fieldName="fan_state"};
struct Actuator Feeder = {.verbose="feederIsOn", .pin=D8, .period=86400000, .dutyCycle=0.0005, .prevTime=0, .isOn=false, .activeHigh=true, .fieldName="feeder_state"};

struct Auto AutoWatering = {.desired=60, .threshold=40, .fieldName="target_moisture" };
struct Auto AutoPh = {.desired=10.4, .threshold=10.4, .fieldName="target_ph" };
struct Auto AutoLight = {.desired=34, .threshold=34, .fieldName="target_light" };
