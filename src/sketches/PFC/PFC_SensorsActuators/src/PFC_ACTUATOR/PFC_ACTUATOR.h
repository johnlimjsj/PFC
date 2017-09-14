#ifndef PFC_ACTUATOR_h
#define PFC_ACTUATOR_h

#include <WString.h>
#include <Arduino.h>

class PFC_ACTUATOR{
  public:
    PFC_ACTUATOR(int pin, unsigned long period_milliseconds, float dutyCycle, bool activeHigh){
      _pin = pin;
      _period = period_milliseconds;
      _dutyCycle = dutyCycle;
      _activeHigh = activeHigh;
    }
    void init(){ pinMode(_pin, OUTPUT); };
    void periodControl();
    void periodControl(int intensity); 
    void digitalOn(){   digitalWrite(_pin, _activeHigh); };
    void digitalOff(){  digitalWrite(_pin, !_activeHigh); };
    void writePin(bool ison);
    void writePin(int intensity);

    
  private:
    String _verbose;
    int _pin;
    unsigned long _period;
    float _dutyCycle;
    unsigned long _prevTime;
    bool _isOn;
    bool _activeHigh;
};



#endif