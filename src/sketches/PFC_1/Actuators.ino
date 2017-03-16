

void periodControl(Actuator a){
  unsigned long currTime = millis();
  unsigned long onPeriod=a.period*a.dutyCycle;
  if(currTime - a.prevTime < a.period){
    (currTime - a.prevTime < onPeriod) ? digitalWrite(a.pin, HIGH) : digitalWrite(a.pin, LOW);
  }
  else {
    a.prevTime += a.period;
  }
}

