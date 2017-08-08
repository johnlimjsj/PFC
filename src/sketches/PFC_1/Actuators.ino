Actuator periodControl(Actuator a){
  unsigned long currTime = millis();
  unsigned long onPeriod=a.period*a.dutyCycle;
  
  if(currTime - a.prevTime < a.period){
    if (currTime - a.prevTime < onPeriod){
      //PRINTLN("turning ON");
      digitalWrite(a.pin, a.activeHigh);
      a.isOn = true;
    }
    else{
      //PRINTLN("turning OFF");
      digitalWrite(a.pin, !a.activeHigh);
      a.isOn = false;
    }
  }
  else {
    a.prevTime += a.period;
  }
  return a;
}

void getSettingsFromServer(){
  String jsonResponse = pfcwifi.getJsonFromServer(pfcwifi.getPathSettings());
  if(jsonResponse != ""){
    JsonObject &root = getParsedJsonObject(jsonResponse);

    AutoWatering.desired = root[BASE_JSON][AutoWatering.fieldName];
    AutoWatering.threshold = AutoWatering.desired * 0.6;
    
    Serial.println(AutoWatering.desired);
    Serial.println(AutoWatering.threshold);
  }
}

bool isManualOverride(){
  String jsonResponse = pfcwifi.getJsonFromServer(pfcwifi.getPathManualSettings());
  if(jsonResponse != ""){
    JsonObject &root = getParsedJsonObject(jsonResponse);
    
    return root[BASE_JSON]["set_manual_override"];
  }
  return false;
}

void manualActuateFromServer(){
  String jsonResponse = pfcwifi.getJsonFromServer(pfcwifi.getPathManualSettings());
  if(jsonResponse != ""){
    JsonObject &root = getParsedJsonObject(jsonResponse);
    bool pumpOn = root[BASE_JSON][Pump.fieldName];
    digitalWrite(Pump.pin, pumpOn ? Pump.activeHigh : !Pump.activeHigh);
    
    bool ledOn = root[BASE_JSON][Led.fieldName];
    digitalWrite(Led.pin, ledOn ? Led.activeHigh : !Led.activeHigh);

    String testpumpstr = "pump state: " + (String)pumpOn;
    String ledpumpstr = ", led state: " + (String)ledOn;
    
    Serial.println(testpumpstr + ledpumpstr);
  }
}


void readAndActuate(String jsonResp, String actuatorName){
  JsonObject &r = getParsedJsonObject(jsonResp);
  const char* h = r[actuatorName];
  PRINTLN(h);
  if (strcmp(r[actuatorName], "on") != 0) {
    Serial.println(actuatorName +" ON");
    
  }
  else{
    Serial.println(actuatorName +" OFF");
  }
}



void initActuatorPins(){
  pinMode(Led.pin, OUTPUT);  
  pinMode(Pump.pin, OUTPUT); 
  pinMode(Feeder.pin, OUTPUT); 
  pinMode(Fan.pin, OUTPUT);  
}

