#ifndef CONSTANTS_h
#define CONSTANTS_h

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


const char* SSID = "Andrew's iPhone6";
const char* PASSWORD = "hn1sxr8vo3wvg";
const char* SERVER = "api.thingspeak.com";
const String API_KEY = "H6VUB8650DYJZG0D";
const int WATERPUMP_PIN = D9; 
const int NUTRIENTPUMP_PIN = D8; 
const int LED_PIN = D7; 
const int FAN_PIN = D6; 
#define DHTTYPE DHT22
#define DHTPIN D2


#endif
