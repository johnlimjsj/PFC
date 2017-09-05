#include "PFC_DISPLAY.h"



UTouch touch(TOUCH_CS, TOUCH_IRQ);
Adafruit_ILI9341 tft(TFT_CS, TFT_DC);
PFC_DISPLAY pfcDisplay;

// Internal objects
PFC_Button *networkButtons;

// Buttons in Menu
PFC_Button selectNetworkButton;
PFC_Button userSettingsButton;
PFC_Button monitorButton;

// Buttons in User Settings Screen
PFC_Button editUserButton;
PFC_Button editPasswordButton;


PFC_KEYBOARD pfcKeyboard(PFC_Button::ALPHA_NUM_KEY, 0, 175, 245, 100);
PFC_INPUT_FIELD userInputField(220, 35, 5, 70, 2);
PFC_INPUT_FIELD passwordInputField(220, 35, 5, 130, 2);
PFC_INPUT_FIELD wifiPasswordInputField(220, 35, 10, 110, 2);
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

    writeTopBanner("Lifeplanters", 3, "Personal Food Computer", 1);  
    
    tft.drawLine(0, _topYCoordinate - 2, _widthX, _topYCoordinate - 2, ILI9341_BLACK);

    initAllButtons();
    printBottomMenu();

    pfcKeyboard.init();

  }

  void PFC_DISPLAY::initAllButtons(){
    // Init Bottom Menu Buttons
    selectNetworkButton.initButtonAsKey(PFC_Button::DEFAULT_BUTTON, "Wifi", "Select",  "Wifi", 40, 300, BUTTON_CENTRALIZE);
    userSettingsButton.initButtonAsKey(PFC_Button::DEFAULT_BUTTON, "Settings", "User",  "Settings", 120, 300, BUTTON_CENTRALIZE);
    monitorButton.initButtonAsKey(PFC_Button::DEFAULT_BUTTON, "Monitor", "Monitor", "Conditions", 200, 300, BUTTON_CENTRALIZE);

    // Init User Settings Page Buttons
    editUserButton.initButtonAsKey(PFC_Button::DEFAULT_BUTTON, "user", "Edit", "User", 200, _topYCoordinate + 30, BUTTON_CENTRALIZE);
    editPasswordButton.initButtonAsKey(PFC_Button::DEFAULT_BUTTON, "user", "Input", "Password", 200, _topYCoordinate + 80, BUTTON_CENTRALIZE);

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
    switch(currScreenState){
      case MAIN_MENU:        
        writeTopBanner("Lifeplanters", 3, "Personal Food Computer", 1);
        Serial.println("displaying main menu");
        break;
      case SELECT_NETWORK:
        writeTopBanner("Select Wifi", 3, "Touch to select Wifi", 1);
        initScreen_Networks();
        displayScreen_Networks();
        break;
      case USER_SETTINGS:
        writeTopBanner("User Settings", 3, "Configure PFC User settings", 1);
        Serial.println("displaying user settings");
        displayScreen_UserSettings();
        break;
      case MONITOR_PFC:
        writeTopBanner("Monitor", 3, "View plant conditions in real time", 1);
        Serial.println("displaying monitor pfc");
        displayScreen_Monitor();
        break;
      case NETWORK_PASSWORD_INPUT:
        writeTopBanner("Wifi Password", 3, "Enter your wifi password", 1);
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
    printCentralised(subheader, subSize, 30);
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
    tft.fillRect(0, _topYCoordinate, _widthX, _btmYCoordinate - _topYCoordinate, tft.color565(255, 255, 255));
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
    }

    // Can possibly delete this function
    void PFC_DISPLAY::onClick_WifiPasswordInput(){
      // selectSingleInput();
      if(inputFields[WIFI_PASSWORD_INPUT].isTouched()){
        activeInput = WIFI_PASSWORD_INPUT;
        inputFields[WIFI_PASSWORD_INPUT].focus(true);
        pfcKeyboard.show(false);
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
      inputFields[USER_INPUT].drawInput();
      inputFields[PASSWORD_INPUT].drawInput();
    }

    // Can possibly delete this function
    void PFC_DISPLAY::onClick_UserSettings(){
      tft.setCursor(10, 50);
      tft.setTextSize(2);
      tft.setTextColor(ILI9341_BLACK);
      tft.println("Username");
      tft.setCursor(10, 110);
      tft.println("Secret Key");
      for(int i=0; i<2; i++){
        if(inputFields[i].isTouched()){
          activeInput = (inputStates)i; // typecasting int into enum
          for(int j=0; j<2; j++){
            inputFields[j].focus(j == activeInput); // Focus the active field. Unfocus the rest
          }
          pfcKeyboard.show(false);
        }
      }
    }

  // =====**** 3. Monitor Conditions Screen ****==========

    void PFC_DISPLAY::displayScreen_Monitor(){

      String settingLabels[] = {"Moisture", "Light", "PH", "Temperature", "Humidity"};
      float settingValues[] = {_moisture, _light, _ph, _temp, _humidity};
      int initYPos = 60;
      int yPos, lineYPos;
      for(int i=0; i<5; i++){
        yPos = initYPos + i*20;
        lineYPos = initYPos + (i+1)*20 - 2;
        tft.setCursor(15, yPos);
        tft.setTextSize(2);
        tft.setTextColor(ILI9341_BLACK);
        tft.println(settingLabels[i]);
        tft.setCursor(165, yPos);
        tft.println(settingValues[i]);
        tft.drawLine(15, lineYPos, _widthX, lineYPos, ILI9341_BLACK);
      }
    }



//======********* On Screen Widgets ******========

  void PFC_DISPLAY::typeInActiveInput(){
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
        else if(pressedKey="done"){
          Serial.println("done key pressed");
          pfcKeyboard.hide();
          _refreshScreen();
        }
      }
    }
  }

