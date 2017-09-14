#ifndef PFC_DISPLAY_h
#define PFC_DISPLAY_h

#include <Arduino.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
// #include <UTouch_ESP8266.h>
#include <UTouch_ESP8266.h> // This library is compatible with the shift register


#include <SPI.h>
#include "../PFC_SETTINGS/PFC_SETTINGS.h"
#include "../PFC_WIFI/PFC_WIFI.h"

// For touchscreen
#define TOUCH_CS D2
#define TOUCH_IRQ D8
#define TOUCH_CS_SR 5
#define TOUCH_IRQ_SR 7


#define TOUCH_ORIENTATION PORTRAIT
// For display
#define TFT_DC D9
#define TFT_CS D10

#define BUTTON_CENTRALIZE 888
#define BUTTON_LEFT 889
#define DEBOUNCE_TIME 400


/*
PIN MAPPINGS
----------------------------
TFT         <----> WEMOS
----------------------------
RST         <----> RESET
CS          <----> D10/SS
DC          <----> D9/TX1
MOSI/T_DIN  <----> D7/MOSI
SCK/T_CLK   <----> D5/SCK
LED         <----> 3v3
MISO        <----> D6/MISO
T_CS        <----> D2
T_OUT       <----> NIL
T_IRQ       <----> D8
*/

class PFC_INPUT_FIELD{
	public:
		PFC_INPUT_FIELD(){};
		PFC_INPUT_FIELD(int width, int height, int xPos, int yPos, int textsize){
			_widthX = width;
			_widthY = height;
			_xPos = xPos;
			_yPos = yPos;
			_textSize = textsize;
			_overflowIndex = _widthX/(6*_textSize);
		}
		void setNumRows(int numRows){ _numRows = numRows; };
		void drawInput();
		void clearText();
		void addTextToEnd(String text);
		void subtractTextFromEnd();
		void print();
		String getText();
		bool contains(int x, int y);
		bool isTouched();
		bool focus(bool isFocused);


	private:

		void _drawOutline(int color);
		bool _isOverflow();

		int _widthX, _widthY;
		int _textSize;
		int _xPos, _yPos;
		int _cursorIndex;
		int _outlineColor = ILI9341_DARKGREY, _fillColor = ILI9341_LIGHTGREY;
		String _text = "";
		int _numTextCharacters;
		int _overflowIndex;
		unsigned long _lastPressedTime;
		int _offset = -8;
		int _numRows = 1;
};





struct ButtonSettings {
	  uint16_t width;
	  uint16_t height;
	  uint16_t outline;
	  uint16_t fill;
	  uint16_t textcolor;
	  uint16_t textsize;
};	

class PFC_Button : public Adafruit_GFX_Button
{
	public:   		
		static struct ButtonSettings ALPHA_NUM_KEY, ALPHA_NUM_KEY_LONG,
									 DEFAULT_BUTTON, DEFAULT_BUTTON_SHORT, DEFAULT_LONG, DEFAULT_SHORT,
									 WHITE_BUTTON, INVIS, LIST_BUTTON;

		void initButtonAsKey(ButtonSettings bs, String val, String label, int xPos, int yPos, int alignment);
		void initButtonAsKey(ButtonSettings bs, String val, String label1, String label2, int xPos, int yPos, int alignment);
		int getWidthOfText(String text, int size){ return text.length()*3*size;};
        int getHeightOfText(int size){ return 4*size;};

        // void setValue(String val){
        // 	_val = val;
        // }

        String getValueOnPress();

        void drawButtonAsKey();
        void enable(); // when button is active, touching it will return a key, 
        void disable(); // when button is inactive, touching it will do nothing;


    private:
    	String _val = "kk";
    	unsigned long _lastPressedTime;
    	int _dispx = 240, _dispy = 320, _text_y_center = (_dispy / 2) - 6;
    	int _offset = -7;
    	int _xCursor, _yCursor, _xCursor2, _yCursor2;
    	int _textSize, _textColor;
    	String _label, _label2;
    	bool isEnabled= true;



};

// A Lifeplanter specific implementation
class PFC_DISPLAY
{
    public:
    	PFC_DISPLAY(){};
    	PFC_DISPLAY(int widthX, int widthY){_widthX = widthX; _widthY = widthY;};
        String keys = "AaBbCcDdEe";

        void initDisplay();
        void initAllButtons();
        void displayScreenByState();

        void clickEventsByScreen();
        void detectScreenSwitch();

        // Method to display various screens

        // 1. Startup Screen
        void displayScreen_Menu();

        // 2. Networks
        void initScreen_Networks();
		void displayScreen_Networks();
		void displayScreen_NetworkPasswordInput();
		void onClick_Networks();

		// 3. User Settings Screen
		void displayScreen_UserSettings();
		void onClick_UserSettings();
		void onClick_WifiPasswordInput();

		// 4. Monitor Conditions Screen
		void displayScreen_Monitor();
		void setMoisture(float moisture){ _moisture = moisture; };
    	void setLight(float light){ _light = light; };
    	void setTemperature(float temp){ _temp = temp; };
    	void setHumidity(float humidity){ _humidity = humidity; };
    	void setPh(float ph){ _ph = ph; };

    	// 5. Print statuses
    	bool connectWifiFromEEPROMAndDisplay();
    	void setWifiState(int state);
    	void printWifiState();
    	void setUserState(int state);
    	void printUserLoginState();
    	void restoreSavedWifiSession();
    	void restoreSavedUserLoginSession();

    	// Linking Input Field to Keyboard. TODO: Refactor to put this inside PFC_INPUT_FIELDS
        void typeInActiveInput();
        
        // Helpers
        void clearWorkingArea();
        void clearBannerArea();
        void writeTopBanner(String header, int headSize, String subheader, int subSize);
        void printBottomMenu();
        void printCentralised(String text, int size, int yPos);
        

        enum screenStates {
    		MAIN_MENU, 
    		SELECT_NETWORK,
    		USER_SETTINGS,
    		MONITOR_PFC,
    		NETWORK_PASSWORD_INPUT,
    	};
    	enum inputStates {
    		USER_INPUT,
    		PASSWORD_INPUT, 
    		WIFI_PASSWORD_INPUT,
    		NONE,
    	};
    	enum wifiStates {
    		DISCONNECTED, 
    		CONNECTING, 
    		WIFI_CONNECTED
    	};

    	enum userStates {
    		LOGGED_OUT,
    		LOGGED_IN,
    	};

    	enum wifiStates currWifiState;
    	enum userStates currUserState;
    	enum inputStates activeInput;
    	enum screenStates currScreenState;
    	enum screenStates prevScreenState;
    	bool isTryingToConnect = false;
    	bool isLoggedIn = false;


    private:
    	int _widthX = 240, _widthY = 320;
    	int _topYCoordinate = 56;
    	int _topPlusMargin = 80;
    	int _statusBarYPos = 35;
    	int _statusBarHeight = 10;
    	int _btmYCoordinate = 270;
    	int _btmBarHeight = 50;

    	int _numNetworks = 0;
    	String *_networks;
    	String _selectedNetwork;

    	int _keyCursorIndex;

    	void _refreshScreen();

    	// PFC Conditions
    	float _moisture=0, _light=0, _ph=0, _temp=0, _humidity=0;


};

class PFC_KEYBOARD{
	public:
		PFC_KEYBOARD(ButtonSettings bs, int xPos, int yPos, int width, int height){
			_xPos = xPos; _yPos = yPos;
			_width = width; _height = height;
			_fill = bs.fill; _outline = bs.outline;
			_textColor = bs.textcolor; _textSize = bs.textsize;
		};
		void init(); // Where we initialize all the keyboard key positions
		void hide();
		void show(bool isShift);
		String detectSpecialKeyPress();
		String returnPressedKey();
		bool isHidden();


		enum keyboardStates {
    		HIDDEN,
    		SHOWN,
    		SHIFT
    	};
    	enum keyboardStates currKeyboardState;

	private:
		int _xPos, _yPos;
		int _width, _height;
		int _fill, _outline;
		int _textColor, _textSize;
		int _totalKeys = 48;
		int _totalSpecialKeys = 4;
		int _numKeysPerRow = 11;
		int _numRows = 5;
		// ButtonSettings _button_small, _button_big;
};

extern UTouch touch;
extern Adafruit_ILI9341 tft;
extern PFC_DISPLAY pfcDisplay;

#endif