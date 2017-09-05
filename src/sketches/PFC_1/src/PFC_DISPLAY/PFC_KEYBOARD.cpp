#include "PFC_DISPLAY.h"

PFC_Button *keys;
PFC_Button *keys_shift;
PFC_Button shiftKey;
PFC_Button deleteKey;
PFC_Button spaceKey;
PFC_Button doneKey;


void PFC_KEYBOARD::init(){

	int keyHPadding = 0;  
    int keyVPadding = 0;

    String key_values_shift[] = {	
    	"!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", 
    	"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "|",
    	"A", "S", "D", "F", "G", "H", "J", "K", "L", ":", "\"",
    	"Z", "X", "C", "V", "B", "N", "M", "<", ">", "?", "~", 
    	"{", "}", "+"};

    String key_values[] = {	
    	"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-",
	    "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "\\",
	    "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'",
	    "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "`",
	    "[", "]", "="};

	int keyShortWidth = _width/_numKeysPerRow;
	int keyLongWidth = keyShortWidth * 1.8;
	int keyHeight = _height/_numRows;

	
	keys = new PFC_Button[_totalKeys];
	keys_shift = new PFC_Button[_totalKeys];
	

	ButtonSettings SHORT_KEY = {.width=keyShortWidth, .height=keyHeight , .outline=_outline, .fill=_fill, .textcolor =_textColor, .textsize=_textSize};
	ButtonSettings LONG_KEY = {.width=keyLongWidth, .height=keyHeight , .outline=_outline, .fill=_fill, .textcolor =_textColor, .textsize=1};

	

	int keyVPos;
	int keyHPos;

	for(int i=0; i<_totalKeys; i++){

		String key = "";
		keyHPos = _xPos + keyShortWidth*(0.5+(i % _numKeysPerRow));
		keyVPos = _yPos + keyHeight*(0.5+(i/_numKeysPerRow));
		key = key_values[i];
		keys[i].initButtonAsKey(SHORT_KEY, key, key, keyHPos, keyVPos, BUTTON_CENTRALIZE);
		key = key_values_shift[i];      
		keys_shift[i].initButtonAsKey(SHORT_KEY, key, key, keyHPos, keyVPos, BUTTON_CENTRALIZE);      
    }
    
    keyHPos += 2*keyShortWidth;

    shiftKey.initButtonAsKey(LONG_KEY, "shift", "shift", keyHPos, keyVPos, BUTTON_CENTRALIZE);

    keyHPos += keyLongWidth;
    spaceKey.initButtonAsKey(LONG_KEY, " ", "space", keyHPos, keyVPos, BUTTON_CENTRALIZE);

    keyHPos += keyLongWidth;
    deleteKey.initButtonAsKey(LONG_KEY, "del", "del", keyHPos, keyVPos, BUTTON_CENTRALIZE);

    keyHPos += keyLongWidth;
    doneKey.initButtonAsKey(LONG_KEY, "done", "done", keyHPos, keyVPos, BUTTON_CENTRALIZE);
}

void PFC_KEYBOARD::hide(){
	currKeyboardState = HIDDEN;
}

void PFC_KEYBOARD::show(bool isShift){

	
	

	currKeyboardState = isShift ? SHIFT : SHOWN;
	
	Serial.print("Keypad state is shift");
	Serial.println(isShift);

	PFC_Button *keysptr = isShift ? keys_shift: keys;

	for (int i=0; i<_totalKeys; i++){
		keysptr[i].drawButtonAsKey();	
	}  

    shiftKey.drawButtonAsKey();
    doneKey.drawButtonAsKey();
    spaceKey.drawButtonAsKey();
    deleteKey.drawButtonAsKey();
}


String PFC_KEYBOARD::returnPressedKey(){
	if(currKeyboardState == HIDDEN) return "";   
    String val = "";
    val = shiftKey.getValueOnPress();

    if(val!="") {
      currKeyboardState = currKeyboardState ==SHIFT ? SHOWN: SHIFT; // Invert the state
      show(currKeyboardState == SHIFT);
    }

    PFC_Button *k = (currKeyboardState == SHIFT) ? keys_shift : keys;

    for(int i=0; i<_totalKeys; i++){
    	val = k[i].getValueOnPress();
      	if (val != "") return val;
    }

    val = spaceKey.getValueOnPress();
    if (val!="") return val;

    val = deleteKey.getValueOnPress();
    if (val!="") return val;

    val = doneKey.getValueOnPress();
    if (val!="") return val;

    return "";
}
