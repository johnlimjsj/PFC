#include "PFC_DISPLAY.h"

UTouch touch(TOUCH_CS, TOUCH_IRQ);
// UTouch touch(TOUCH_CS_SR, TOUCH_IRQ, 1, D4, D0, D3);
Adafruit_ILI9341 tft(TFT_CS, TFT_DC);

PFC_DISPLAY pfcDisplay;

// Network buttons
PFC_Button *networkButtons;

// Buttons in Menu
PFC_Button selectNetworkButton;
PFC_Button userSettingsButton;
PFC_Button monitorButton;

// Buttons in User Settings Screen
PFC_Button loginButton;

//Buttons in Network password input
PFC_Button connectWifiButton;


PFC_KEYBOARD pfcKeyboard(PFC_Button::ALPHA_NUM_KEY, 0, 178, 245, 100);
PFC_INPUT_FIELD userInputField(230, 32, 5, 77, 2);
PFC_INPUT_FIELD passwordInputField(175, 32, 5, 134, 2);
PFC_INPUT_FIELD wifiPasswordInputField(230, 32, 5, 102, 2);
PFC_INPUT_FIELD inputFields[] = {userInputField, passwordInputField, wifiPasswordInputField};
int numInputs = 3;

// ========= PFC_DISPLAY Class =========================

// =========******** Screen Init Stuff *****============
  void PFC_DISPLAY::initDisplay(){
    pinMode(TFT_CS, OUTPUT);
    digitalWrite(TFT_CS, HIGH);
    pinMode(TOUCH_CS, OUTPUT);
    digitalWrite(TOUCH_CS, HIGH);
    tft.begin();
    tft.setRotation(0);
    tft.fillScreen(ILI9341_WHITE);
    tft.setTextColor(ILI9341_BLACK);    
    touch.InitTouch(TOUCH_ORIENTATION);
    touch.setPrecision(PREC_MEDIUM);
    writeTopBanner("Lifeplanters", 2, "Personal Food Computer", 1);
    printWifiState();
    printUserLoginState();
    tft.drawLine(0, _topYCoordinate - 2, _widthX, _topYCoordinate - 2, ILI9341_BLACK);
    initAllButtons();
    printBottomMenu();
    pfcKeyboard.init();
    for(int i=0; i<numInputs; i++){ inputFields[i].setNumRows(2); }    
  }

  void PFC_DISPLAY::initAllButtons(){
    // Init Bottom Menu Buttons
    selectNetworkButton.initButtonAsKey(PFC_Button::DEFAULT_BUTTON, "Wifi", "Select",  "Wifi", 40, 305, BUTTON_CENTRALIZE);
    userSettingsButton.initButtonAsKey(PFC_Button::DEFAULT_BUTTON, "Settings", "User",  "Settings", 120, 305, BUTTON_CENTRALIZE);
    monitorButton.initButtonAsKey(PFC_Button::DEFAULT_BUTTON, "Monitor", "Monitor", "Conditions", 200, 305, BUTTON_CENTRALIZE);

    // Init buttons on user setting screen
    loginButton.initButtonAsKey(PFC_Button::DEFAULT_BUTTON_SHORT, "login", "Login", 210, _topYCoordinate + 95, BUTTON_CENTRALIZE );

    // init buttons on wifi password input screen
    connectWifiButton.initButtonAsKey(PFC_Button::DEFAULT_BUTTON, "connect", "Connect", 190, _topYCoordinate + 100, BUTTON_CENTRALIZE );    
  }

  void PFC_DISPLAY::clickEventsByScreen(){
    switch(currScreenState){
      case MAIN_MENU:
        break;
      case SELECT_NETWORK:
        onClick_Networks();
        break;
      case USER_SETTINGS:
        onClick_UserSettings();   
        break;
      case MONITOR_PFC:

        break;
      case NETWORK_PASSWORD_INPUT:
        onClick_WifiPasswordInput();
        break;
      default:
        break;
    }
    typeInActiveInput();
  }

  void PFC_DISPLAY::displayScreenByState(){
    if(currScreenState == MONITOR_PFC){
      displayScreen_Monitor();
    }
    if(currScreenState != prevScreenState){      
      _refreshScreen();      
      prevScreenState = currScreenState;
    } 
  }

  void PFC_DISPLAY::_refreshScreen(){
    pfcKeyboard.hide(); // Hide keyboard whenever screen is changed
    activeInput = NONE; // No input wil be active whenever screen is changed
    clearBannerArea();
    clearWorkingArea();
    printWifiState();
    printUserLoginState();
    switch(currScreenState){
      case MAIN_MENU:        
        writeTopBanner("Lifeplanters", 2, "Personal Food Computer", 1);
        Serial.println("displaying main menu");
        break;
      case SELECT_NETWORK:
        writeTopBanner("Select Wifi", 2, "Touch to select Wifi", 1);
        initScreen_Networks();
        displayScreen_Networks();
        break;
      case USER_SETTINGS:
        writeTopBanner("User Settings", 2, "Configure PFC User settings", 1);
        Serial.println("displaying user settings");
        displayScreen_UserSettings();
        break;
      case MONITOR_PFC:
        writeTopBanner("Monitor", 2, "View plant conditions in real time", 1);
        Serial.println("displaying monitor pfc");
        displayScreen_Monitor();
        break;
      case NETWORK_PASSWORD_INPUT:
        writeTopBanner("Wifi Password", 2, "Enter your wifi password", 1);
        displayScreen_NetworkPasswordInput();
        break;
      default:
        break;    
    }
  }

  void PFC_DISPLAY::detectScreenSwitch(){

    String ret;
    ret = selectNetworkButton.getValueOnPress();

    if (ret != ""){
      currScreenState = SELECT_NETWORK;
      return;
    }

    ret = userSettingsButton.getValueOnPress();
    if (ret != ""){
      currScreenState = USER_SETTINGS;
      return;
    }

    ret = monitorButton.getValueOnPress();
    if (ret != ""){
      currScreenState = MONITOR_PFC;
      return;
    }

  }

// =======****** Header and Footers of Screen ******==========

  void PFC_DISPLAY::writeTopBanner(String header, int headSize, String subheader, int subSize){
    tft.setTextWrap(true);
    tft.setTextColor(ILI9341_BLACK);
    printCentralised(header, headSize, 5);
    printCentralised(subheader, subSize, 23);
  }

  void PFC_DISPLAY::printBottomMenu(){
    selectNetworkButton.drawButtonAsKey();
    userSettingsButton.drawButtonAsKey();
    monitorButton.drawButtonAsKey();
  }

// ======****** Helper Methods *******===========

  void PFC_DISPLAY::printCentralised(String text, int size, int yPos){
    tft.setTextSize(size);
    int len = text.length()*3*size;
    tft.setCursor(_widthX/2 - len, yPos);
    tft.println(text);
  }

  void PFC_DISPLAY::clearWorkingArea(){
    int height = 230; // _widthY - _topYCoordinate - _btmBarHeight;
    tft.fillRect(0, _topYCoordinate, _widthX, height, tft.color565(255, 255, 255));
  }

  void PFC_DISPLAY::clearBannerArea(){
    tft.fillRect(0, 0, _widthX, _topYCoordinate - 3, tft.color565(255, 255, 255));
  }

// =======****** Screens *******===========

  // =====**** 1. Select Wifi Networks ****==========
    void PFC_DISPLAY::initScreen_Networks(){
      tft.setTextColor(ILI9341_BLACK);
      printCentralised("Loading", 2, 100);
      printCentralised("Networks", 2, 120);
      printCentralised("...", 2, 140);
      _networks = pfcwifi.getAllNetworks(_numNetworks);
      clearWorkingArea();
    }

    void PFC_DISPLAY::displayScreen_Networks(){
      yield();
      networkButtons = new PFC_Button[_numNetworks];
      // clearWorkingArea();
      for(int i = 0; i < _numNetworks; i++ ){
        String nw = _networks[i];
        networkButtons[i].initButtonAsKey(PFC_Button::LIST_BUTTON, nw, nw, 120, _topPlusMargin + PFC_Button::LIST_BUTTON.height*(i + 0.5), BUTTON_CENTRALIZE);
        networkButtons[i].drawButtonAsKey();
      }    
    }

    void PFC_DISPLAY::displayScreen_NetworkPasswordInput(){
      printCentralised(_selectedNetwork, 2, 60);
      printCentralised("Password:", 2, 80);
      inputFields[WIFI_PASSWORD_INPUT].drawInput();
      connectWifiButton.drawButtonAsKey();      
    }

    // Can possibly delete this function
    void PFC_DISPLAY::onClick_WifiPasswordInput(){
      // selectSingleInput();
      if(inputFields[WIFI_PASSWORD_INPUT].isTouched()){
        activeInput = WIFI_PASSWORD_INPUT;
        inputFields[WIFI_PASSWORD_INPUT].focus(true);
        pfcKeyboard.show(false);
      }
      String val = connectWifiButton.getValueOnPress();
      if (val != ""){
        String pw = inputFields[WIFI_PASSWORD_INPUT].getText();
        writeWifiSettings(_selectedNetwork, pw);
        tft.setCursor(10, 150);
        tft.setTextSize(2);
        tft.setTextColor(ILI9341_BLACK);
        tft.println("Connecting");
        String msg = connectWifiFromEEPROMAndDisplay() ? "Connected" : "Failed";

        tft.fillRect(5, 140, 140, 30, ILI9341_WHITE);   
        tft.setCursor(10, 150);    
        tft.setTextSize(2);
        tft.setTextColor(ILI9341_BLACK);
        tft.println(msg);
      }
    }

    void PFC_DISPLAY::onClick_Networks(){
      for(int i = 0; i < _numNetworks; i++ ){
        String network = networkButtons[i].getValueOnPress();        

        if (network != ""){
          _selectedNetwork = network;
          currScreenState = NETWORK_PASSWORD_INPUT;
          return;
        }
      }    
    }


  // =====**** 2. User Settings Screen ****==========
    void PFC_DISPLAY::displayScreen_UserSettings(){
      yield();
      tft.setCursor(10, 60);
      tft.setTextSize(2);
      tft.setTextColor(ILI9341_BLACK);
      tft.println("Username");
      tft.setCursor(10, 60 + 55);
      tft.println("Secret Key");
      inputFields[USER_INPUT].drawInput();
      inputFields[PASSWORD_INPUT].drawInput();
      loginButton.drawButtonAsKey();
    }

    void PFC_DISPLAY::onClick_UserSettings(){
      for(int i=0; i<2; i++){
        if(inputFields[i].isTouched()){
          activeInput = (inputStates)i; // typecasting int into enum
          for(int j=0; j<2; j++){
            inputFields[j].focus(j == activeInput); // Focus the active field. Unfocus the rest
          }
          pfcKeyboard.show(false);
        }
      }
      // print the login button

    }

  // =====**** 3. Monitor Conditions Screen ****==========

    void PFC_DISPLAY::displayScreen_Monitor(){

      String settingLabels[] = {"Moisture", "Light", "PH", "Temperature", "Humidity"};
      float settingValues[] = {_moisture, _light, _ph, _temp, _humidity};
      int initYPos = 60;
      int yPos, lineYPos;
      for(int i=0; i<5; i++){
        yPos = initYPos + i*25 + 2;
        lineYPos = initYPos + (i+1)*25 - 2;
        tft.setCursor(15, yPos);
        tft.setTextSize(2);
        tft.setTextColor(ILI9341_BLACK);
        tft.println(settingLabels[i]);
        tft.setCursor(165, yPos);
        tft.fillRect(165, lineYPos -24 , 100, 23, tft.color565(255, 255, 255));
        tft.println(settingValues[i]);
        tft.drawLine(15, lineYPos, _widthX, lineYPos, ILI9341_BLACK);
      }
    }

  //========== 5. Print Statuses ==========

    void PFC_DISPLAY::printWifiState(){
      tft.fillRect(0, _statusBarYPos, _widthX, _statusBarHeight, ILI9341_YELLOW);
      tft.setCursor(1, _statusBarYPos + 1);
      tft.setTextSize(1);
      tft.setTextColor(ILI9341_BLACK);
      String ssid = EEPROM_retrieveLen_readString(EEPROM_ADDRSTART_WIFI_SSID, EEPROM_ADDR_LENGTH_WIFI_SSID);
      String message = "";
      switch(currWifiState){
        case DISCONNECTED:
          message = "WiFi: Disconnected";
          break;
        case CONNECTING:
          message = "WiFi: Connecting to " + ssid + "...";
          break;
        case WIFI_CONNECTED:
          message = "WiFi: Connected to " + ssid;
          break;
        default:
          break;
      }
      tft.println(message);
    }

    void PFC_DISPLAY::printUserLoginState(){
      int yPos = _statusBarYPos + _statusBarHeight;
      tft.fillRect(0, yPos, _widthX, _statusBarHeight, ILI9341_GREENYELLOW);
      tft.setCursor(1, yPos + 1);
      tft.setTextSize(1);
      tft.setTextColor(ILI9341_BLACK);
      String message = isLoggedIn ? "Logged in to hhhh" : "Not logged in";
      tft.println(message);
    }

  bool PFC_DISPLAY::connectWifiFromEEPROMAndDisplay(){
    String ssid; // = "GodIsGracious";
    String password; // = "zdgwj81446";
    loadWifiSettings(ssid, password);
    // Serial.println(password);
    currWifiState = pfcDisplay.CONNECTING;
    printWifiState();
    bool isConnected = pfcwifi.connectWiFi(ssid, password); 
    currWifiState = (isConnected) ? pfcDisplay.WIFI_CONNECTED : pfcDisplay.DISCONNECTED;
    EEPROM.write(EEPROM_ADDR_IS_WIFI_CONNECTED, currWifiState);
    EEPROM.commit();
    printWifiState();
    isTryingToConnect = false;
    return isConnected;
  }

//======********* On Screen Widgets ******========

  void PFC_DISPLAY::typeInActiveInput(){

    // CHecks if the keyboard is hidden. If hidden, then cannot return pressed key
    String pressedKey = pfcKeyboard.returnPressedKey();

    if (pressedKey != "") {
      if (pressedKey.length() == 1){      
        inputFields[activeInput].addTextToEnd(pressedKey);
        inputFields[activeInput].print();
      }
      else{
        // Process special key press except shift and space key
        if(pressedKey=="del"){
          inputFields[activeInput].subtractTextFromEnd();
        }
        else if(pressedKey=="clear"){
          inputFields[activeInput].clearText();
        }
        else if(pressedKey="done"){
          Serial.println("done key pressed");
          pfcKeyboard.hide();
          _refreshScreen();
        }
      }
    }
  }

