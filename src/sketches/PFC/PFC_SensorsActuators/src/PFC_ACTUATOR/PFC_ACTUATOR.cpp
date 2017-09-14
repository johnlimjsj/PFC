#include "PFC_ACTUATOR.h"

void PFC_ACTUATOR::periodControl(){
  unsigned long currTime = millis();
  unsigned long onPeriod=_period*_dutyCycle;
  
  if(currTime - _prevTime < _period){
    if (currTime - _prevTime < onPeriod){
    	writePin(_activeHigh);
      	_isOn = true;
    }
    else{
    	writePin(!_activeHigh);
      	_isOn = false;
    }
  }
  else {
    _prevTime += _period;
  }
}

void PFC_ACTUATOR::periodControl(int intensity){
  unsigned long currTime = millis();
  unsigned long onPeriod=_period*_dutyCycle;
  
  if(currTime - _prevTime < _period){
    if (currTime - _prevTime < onPeriod){
    	writePin(intensity);
      	_isOn = true;
    }
    else{
    	writePin(intensity);
      	_isOn = false;
    }
  }
  else {
    _prevTime += _period;
  }
}

void PFC_ACTUATOR::writePin(bool isOn){
	digitalWrite(_pin, isOn);
}

void PFC_ACTUATOR::writePin(int intensity){
	analogWrite(_pin, intensity);
}



