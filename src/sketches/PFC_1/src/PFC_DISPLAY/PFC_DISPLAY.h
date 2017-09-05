#ifndef PFC_DISPLAY_h
#define PFC_DISPLAY_h

#include <Arduino.h>
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <UTouch_ESP8266.h>
#include <SPI.h>
#include "../PFC_SETTINGS/PFC_SETTINGS.h"
#include "../PFC_WIFI/PFC_WIFI.h"

// For touchscreen
#define TOUCH_CS D2
#define TOUCH_IRQ D8
#define TOUCH_ORIENTATION PORTRAIT
// For display
#define TFT_DC D9
#define TFT_CS D10

#define BUTTON_CENTRALIZE 888
#define BUTTON_LEFT 889
#define DEBOUNCE_TIME 500




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
		void drawInput();
		void clearText();
		void addTextToEnd(String text);
		void subtractTextFromEnd();
		void print();
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
		int _offset = -7;
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
									 DEFAULT_BUTTON, DEFAULT_LONG, DEFAULT_SHORT,
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


    private:
    	String _val = "kk";
    	unsigned long _lastPressedTime;
    	int _dispx = 240, _dispy = 320, _text_y_center = (_dispy / 2) - 6;
    	int _offset = -7;
    	int _xCursor, _yCursor, _xCursor2, _yCursor2;
    	int _textSize, _textColor;
    	String _label, _label2;



};

class PFC_DISPLAY
{
    public:
    	PFC_DISPLAY(){};
    	PFC_DISPLAY(int widthX, int widthY){_widthX = widthX; _widthY = widthY;};
        String keys = "AaBbCcDdEe";

        void initDisplay();
        void displayScreenByState();

        void clickEventsByScreen();
        void detectScreenSwitch();

        // Method to display various screens
        void displayScreen_Menu();
        void initScreen_Networks();
		void displayScreen_Networks();
		void displayScreen_NetworkPasswordInput();

		void onClick_Networks();
		void displayScreen_UserSettings();
		void onClick_UserSettings();
		void onClick_WifiPasswordInput();

		void displayScreen_Monitor();
		void setMoisture(float moisture);
    	void setLight(float light);
    	void setTemperature(float temp);
    	void setHumidity(float humidity);
    	void setPh(float ph);

		// Methods to display screen widgets
        void displayKeyboard();
        String returnPressedKey();

        void selectSingleInput();

        void initAllInputs();
        void typeInActiveInput();
        
        void clearWorkingArea();
        void clearBannerArea();

        void writeTopBanner(String header, int headSize, String subheader, int subSize);
        void printBottomMenu();

        void printCentralised(String text, int size, int yPos);
        
        void initAllButtons();

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

    	enum inputStates activeInput;

    	enum screenStates currScreenState;
    	enum screenStates prevScreenState;

    private:
    	int _widthX = 240, _widthY = 320;
    	int _topYCoordinate = 45;
    	int _topPlusMargin = 60;
    	int _btmYCoordinate = _widthY - _topYCoordinate + 10;

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
		int _totalKeys = 47;
		int _totalSpecialKeys = 4;
		int _numKeysPerRow = 11;
		int _numRows = 5;
		// ButtonSettings _button_small, _button_big;
};

extern UTouch touch;
extern Adafruit_ILI9341 tft;
extern PFC_DISPLAY pfcDisplay;
// extern PFC_Button *keys;
// extern PFC_Button *keys_shift;
// extern PFC_Button shiftKey;
// extern PFC_Button deleteKey;
// extern PFC_Button spaceKey;
// extern PFC_Button doneKey;

#endif