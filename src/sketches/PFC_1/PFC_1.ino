#include "PFC.h"
#include "src/PFC_WIFI/PFC_WIFI.h"
#include "src/PFC_SETTINGS/PFC_SETTINGS.h"
#include "src/PFC_DISPLAY/PFC_DISPLAY.h"


UTouch myTouch = UTouch(TOUCH_CS, TOUCH_IRQ);
Adafruit_ILI9341 myGLCD = Adafruit_ILI9341(TFT_CS, TFT_DC);
PFC_Button button;
PFC_Button button2;

WiFiClient client;
HttpClient http(client);
DHT dht(DHTPIN, DHTTYPE);
I2CSoilMoistureSensor soilSensor(0x21);


uint32_t cx, cy;
uint32_t rx[8], ry[8];
uint32_t clx, crx, cty, cby;
float px, py;
int dispx, dispy, text_y_center;
uint32_t calx, caly, cals;
char buf[13];

void setup() {
  Serial.begin(9600);  

//  initActuatorPins();
    

  
  //********** End Touch Screen Stuff
  
  // LCD load screen for inputting credentials if unset. Otherwise, load option to update the credentials within 10 seconds, if not proceed. 
  // Do LCD screen stuff. Return a string for the following: SSID, Password, user, apikey
  // Then write the above items to EEPROM
  

  String ssid = "GodIsGracious";
  String password = "zdgwj81446";
  String user = "johan-kok";
  String apikey = "891b2d8285354ed";
  EEPROM.begin(512);
  loadSettings(ssid, password, user, apikey);
  
  pfcwifi.initPathSettings(RootPaths, user, apikey);
  pfcwifi.initPeriodicSettings(SendToServer);
//  pfcwifi.connectWiFi(ssid, password);
  dht.begin();
  soilSensorInit();
  
  
  //********** Touch Screen Stuff
  pfcDisplay.initDisplay();
  
  
}

void loop() { 

  pfcDisplay.displayScreenByState();
  pfcDisplay.clickEventsByScreen();
  pfcDisplay.detectScreenSwitch();
  
}


void loopDisp(){
  startup();

  drawCrossHair(dispx - 11, 10, myGLCD.color565(80, 80, 80));
  drawCrossHair(dispx / 2, 10, myGLCD.color565(80, 80, 80));
  drawCrossHair(10, 10, myGLCD.color565(80, 80, 80));
  drawCrossHair(dispx - 11, dispy / 2, myGLCD.color565(80, 80, 80));
  drawCrossHair(10, dispy / 2, myGLCD.color565(80, 80, 80));
  drawCrossHair(dispx - 11, dispy - 11, myGLCD.color565(80, 80, 80));
  drawCrossHair(dispx / 2, dispy - 11, myGLCD.color565(80, 80, 80));
  drawCrossHair(10, dispy - 11, myGLCD.color565(80, 80, 80));
  myGLCD.setTextColor(myGLCD.color565(255, 255, 255));
  myGLCD.setCursor(100, 40);
  myGLCD.println("***********");
  myGLCD.setCursor(100, 60);
  myGLCD.println("***********");

  calibrate(10, 10, 0);
  calibrate(10, dispy / 2, 1);
  calibrate(10, dispy - 11, 2);
  calibrate(dispx / 2, 10, 3);
  calibrate(dispx / 2, dispy - 11, 4);
  calibrate(dispx - 11, 10, 5);
  calibrate(dispx - 11, dispy / 2, 6);
  calibrate(dispx - 11, dispy - 11, 7);

  if (TOUCH_ORIENTATION == LANDSCAPE)
    cals = (long(dispx - 1) << 12) + (dispy - 1);
  else
    cals = (long(dispy - 1) << 12) + (dispx - 1);

  if (TOUCH_ORIENTATION == PORTRAIT)
    px = abs(((float(rx[2] + rx[4] + rx[7]) / 3) - (float(rx[0] + rx[3] + rx[5]) / 3)) / (dispy - 20));  // PORTRAIT
  else
    px = abs(((float(rx[5] + rx[6] + rx[7]) / 3) - (float(rx[0] + rx[1] + rx[2]) / 3)) / (dispy - 20));  // LANDSCAPE

  if (TOUCH_ORIENTATION == PORTRAIT)
  {
    clx = (((rx[0] + rx[3] + rx[5]) / 3));  // PORTRAIT
    crx = (((rx[2] + rx[4] + rx[7]) / 3));  // PORTRAIT
  }
  else
  {
    clx = (((rx[0] + rx[1] + rx[2]) / 3));  // LANDSCAPE
    crx = (((rx[5] + rx[6] + rx[7]) / 3));  // LANDSCAPE
  }
  if (clx<crx)
  {
    clx = clx - (px * 10);
    crx = crx + (px * 10);
  }
  else
  {
    clx = clx + (px * 10);
    crx = crx - (px * 10);
  }

  if (TOUCH_ORIENTATION == PORTRAIT)
    py = abs(((float(ry[5] + ry[6] + ry[7]) / 3) - (float(ry[0] + ry[1] + ry[2]) / 3)) / (dispx - 20));  // PORTRAIT
  else
    py = abs(((float(ry[0] + ry[3] + ry[5]) / 3) - (float(ry[2] + ry[4] + ry[7]) / 3)) / (dispx - 20));  // LANDSCAPE

  if (TOUCH_ORIENTATION == PORTRAIT)
  {
    cty = (((ry[5] + ry[6] + ry[7]) / 3));  // PORTRAIT
    cby = (((ry[0] + ry[1] + ry[2]) / 3));  // PORTRAIT
  }
  else
  {
    cty = (((ry[0] + ry[3] + ry[5]) / 3));  // LANDSCAPE
    cby = (((ry[2] + ry[4] + ry[7]) / 3));  // LANDSCAPE
  }
  if (cty<cby)
  {
    cty = cty - (py * 10);
    cby = cby + (py * 10);
  }
  else
  {
    cty = cty + (py * 10);
    cby = cby - (py * 10);
  }

  calx = (long(clx) << 14) + long(crx);
  caly = (long(cty) << 14) + long(cby);
  if (TOUCH_ORIENTATION == LANDSCAPE)
    cals = cals + (1L << 31);

  done();
  while (true) { yield(); }

}

void loopA(){
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



  
  


