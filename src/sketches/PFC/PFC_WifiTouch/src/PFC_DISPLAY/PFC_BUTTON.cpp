#include "PFC_DISPLAY.h"


// ========= PFC_Button Class =========================

ButtonSettings PFC_Button::ALPHA_NUM_KEY_LONG = {.width=40, .height=24 , .outline=ILI9341_DARKCYAN, .fill=ILI9341_BLUE, .textcolor =ILI9341_GREENYELLOW, .textsize=1};
ButtonSettings PFC_Button::ALPHA_NUM_KEY = {.width=22, .height=22 , .outline=ILI9341_DARKCYAN, .fill=ILI9341_BLUE, .textcolor =ILI9341_GREENYELLOW, .textsize=2};
ButtonSettings PFC_Button::DEFAULT_BUTTON_SHORT = {.width=50, .height=30 , .outline=ILI9341_DARKCYAN, .fill=ILI9341_BLUE, .textcolor =ILI9341_GREENYELLOW, .textsize=1};
ButtonSettings PFC_Button::DEFAULT_BUTTON = {.width=70, .height=30 , .outline=ILI9341_DARKCYAN, .fill=ILI9341_BLUE, .textcolor =ILI9341_GREENYELLOW, .textsize=1};
ButtonSettings PFC_Button::DEFAULT_LONG = {.width=100, .height=30 , .outline=ILI9341_DARKCYAN, .fill=ILI9341_BLUE, .textcolor =ILI9341_GREENYELLOW, .textsize=1};
ButtonSettings PFC_Button::WHITE_BUTTON = {.width=100, .height=40 , .outline=ILI9341_WHITE, .fill=ILI9341_WHITE, .textcolor =ILI9341_BLACK, .textsize=2};
ButtonSettings PFC_Button::LIST_BUTTON = {.width=170, .height=25 , .outline=ILI9341_WHITE, .fill=ILI9341_WHITE, .textcolor =ILI9341_BLACK, .textsize=2};
ButtonSettings PFC_Button::INVIS = {.width=70, .height=40 , .outline=ILI9341_WHITE, .fill=ILI9341_WHITE, .textcolor =ILI9341_BLACK, .textsize=2};

String PFC_Button::getValueOnPress(){

  if(!isEnabled) return "";

  unsigned long currTime = millis();
  // Factor in a button debounce
  if (touch.dataAvailable() && currTime - _lastPressedTime > DEBOUNCE_TIME){
    
    touch.read();
    int _xpos = touch.getX();
    int _ypos = touch.getY();
    if (contains(_xpos, _ypos + _offset)){
      _lastPressedTime = millis();
      return _val; 
    }
  }
  
  return "";  
}

void PFC_Button::initButtonAsKey(ButtonSettings bs, String val, String label, int xPos, int yPos, int alignment){
  initButtonAsKey(bs, val, label, "", xPos, yPos, alignment);
  
}

void PFC_Button::initButtonAsKey(ButtonSettings bs, String val, String label, String label2, int xPos, int yPos, int alignment){
  // int xCursor, yCursor, xCursor2, yCursor2;
  switch(alignment){
    case BUTTON_CENTRALIZE:
      initButton(&tft, xPos, yPos, bs.width, bs.height, bs.outline, bs.fill, bs.fill, ".", bs.textsize);  
      _xCursor = xPos - getWidthOfText(label, bs.textsize);
      _yCursor = (label2 == "") ? yPos - getHeightOfText(bs.textsize): yPos - 2*getHeightOfText(bs.textsize);
      _xCursor2 = xPos - getWidthOfText(label2, bs.textsize);
      _yCursor2 = yPos + getHeightOfText(bs.textsize);
      break;
    case BUTTON_LEFT:
      initButtonUL(&tft, xPos, yPos, bs.width, bs.height, bs.outline, bs.fill, bs.fill, ".", bs.textsize);  
      _xCursor = _xCursor2 = xPos;
      _yCursor = yPos; 
      _yCursor2 = _yCursor + 1.5*getHeightOfText(bs.textsize);
      break;
    default:
      initButton(&tft, xPos, yPos, bs.width, bs.height, bs.outline, bs.fill, bs.fill, ".", bs.textsize);  
  }  
  _label = label;
  _label2 = label2;
  _textSize = bs.textsize;
  _textColor = bs.textcolor;
  _val = val;
}

void PFC_Button::drawButtonAsKey(){
	drawButton();  
	tft.setTextColor(_textColor);
	tft.setTextSize(_textSize);
	tft.setCursor(_xCursor, _yCursor);
	tft.println(_label);
	tft.setCursor(_xCursor2, _yCursor2);
	tft.println(_label2);
}
