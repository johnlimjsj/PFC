Actuator periodControl(Actuator a){
  unsigned long currTime = millis();
  unsigned long onPeriod=a.period*a.dutyCycle;
  
  if(currTime - a.prevTime < a.period){
    if (currTime - a.prevTime < onPeriod){
      //Serial.println("turning ON");
      digitalWrite(a.pin, HIGH);
      a.isOn = true;
    }
    else{
      //Serial.println("turning OFF");
      digitalWrite(a.pin, LOW);
      a.isOn = false;
    }
  }
  else {
    a.prevTime += a.period;
  }
  return a;
}

void getSettingsFromServer(){
  String jsonResponse = getJsonFromServer(PATH_GET_SETTINGS);
  if(jsonResponse != ""){
    JsonObject &root = getParsedJsonObject(jsonResponse);
    
    
    AutoWatering.desired = root["fields"]["target_moisture"];
    AutoWatering.threshold = AutoWatering.desired * 0.6;
    
    Serial.println(AutoWatering.desired);
    Serial.println(AutoWatering.threshold);
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




void controlActuators(){
  String getStr = "/user/panel/update/Alvin-Chew/17e373405989a62/";
  String jsonResponse = getJsonFromServer(getStr);
//  readAndActuate(jsonResponse, "pump");
  
  if(jsonResponse != ""){
    JsonObject &root = getParsedJsonObject(jsonResponse);
    String retJson;
    
    const char* user = root["fields"]["target_moisture"];
    PRINTLN(user);

    
        
//    if (strcmp(user, "2") == 0) {
//      digitalWrite(LED_PIN, LOW);
//      PRINTLN("LED ON");
//    }

    /*
    if (strcmp(r["led"], "on") == 0) {
      digitalWrite(LED_PIN, LOW);
      PRINTLN("LED ON");
    }
    else{
      digitalWrite(LED_PIN, HIGH);
      PRINTLN("LED OFF");
    }

    if (strcmp(r["pump"], "on") == 0) {
      digitalWrite(WATERPUMP_PIN, HIGH);
      PRINTLN("PUMP ON");
    }
    else{
      digitalWrite(WATERPUMP_PIN, LOW);
      PRINTLN("PUMP OFF");
    }

    */
  }  
  

}

void initActuatorPins(){
  pinMode(LED_PIN, OUTPUT);  
  pinMode(WATERPUMP_PIN, OUTPUT); 
  pinMode(FEEDER_PIN, OUTPUT); 
  pinMode(FAN_PIN, OUTPUT);  
}

