#include "PFC_DISPLAY.h"



void PFC_INPUT_FIELD::drawInput(){
	tft.fillRect(_xPos, _yPos, _widthX, _widthY, _fillColor);
	print();
}


void PFC_INPUT_FIELD::clearText(){
	_text = "";
	_numTextCharacters = 0;
	drawInput();
}

void PFC_INPUT_FIELD::addTextToEnd(String text){
	if(_numTextCharacters >= _numRows*_overflowIndex){
		return;
	}
	else{
		_text += text;	
		_numTextCharacters ++;
	}	
}

void PFC_INPUT_FIELD::subtractTextFromEnd(){
	int lastIndex = _text.length() - 1;
	_text.remove(lastIndex);
	if (_numTextCharacters >0) _numTextCharacters --;
	drawInput();
}

String PFC_INPUT_FIELD::getText(){
	return _text;
}

void PFC_INPUT_FIELD::print(){

	Serial.println(_text);
	tft.setTextSize(_textSize);
	tft.setTextColor(ILI9341_BLACK);
	tft.setCursor(_xPos, _yPos );
	
	if (_numTextCharacters >= 1*_overflowIndex){
		Serial.print("Overflow index");
		Serial.println(_overflowIndex);
		tft.println(_text.substring(0, _overflowIndex));
		tft.setCursor(_xPos, _yPos + _textSize*8 );
		tft.println(_text.substring(_overflowIndex));
	}
	else{
		tft.println(_text);	
	}
	
}

bool PFC_INPUT_FIELD::isTouched(){
	if (touch.dataAvailable()){
		touch.read();
		int xTouch = touch.getX();
    	int yTouch = touch.getY();
  		if (contains(xTouch, yTouch)){
    		return true;
    	}
	}
	return false;
}

bool PFC_INPUT_FIELD::focus(bool isFocused){
	int outlineColor = isFocused ? _outlineColor : _fillColor;
	// drawInput();
	_drawOutline(outlineColor);
}

bool PFC_INPUT_FIELD::contains(int x, int y){
	bool containsX = ((x >= _xPos) && x <= (_xPos + _widthX));
	bool containsY = ((y >= _yPos) && y <= (_yPos + _widthY));
	return (containsX && containsY);
}

void PFC_INPUT_FIELD::_drawOutline(int color){
	tft.drawLine(_xPos, _yPos-1, _widthX, _yPos-1, color);
	tft.drawLine(_xPos, _yPos + _widthY, _widthX, _yPos + _widthY, color);
}

bool PFC_INPUT_FIELD::_isOverflow(){
	int textWidth = _numTextCharacters*3*_textSize;
	if (textWidth >= _widthX) return true;
	return false;
}
