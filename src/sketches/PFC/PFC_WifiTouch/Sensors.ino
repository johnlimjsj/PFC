
// Belongs to Micro
/*
void soilSensorInit(){
  delay(1000);
  Wire.begin();
  soilSensor.begin();
  delay(1000); // give some time to boot up
  Serial.print("I2C Soil Moisture Sensor Address: ");
  Serial.println(soilSensor.getAddress(),HEX);
  Serial.print("Sensor Firmware version: ");
  Serial.println(soilSensor.getVersion(),HEX);
  Serial.println();
}

// Belongs to Micro
void getSensorReadings(){
  float rawMoisture = (float)soilSensor.getCapacitance();
  float rawLight = (float)soilSensor.getLight(true);  
  Moisture.reading = mapDouble(rawMoisture, Moisture.minInput, Moisture.maxInput, Moisture.minOutput, Moisture.maxOutput, 1);
  Light.reading = mapDouble(rawLight, Light.minInput, Light.maxInput, Light.minOutput, Light.maxOutput, 2);
  PH.reading = mapDouble(analogRead(PH_PIN), PH.minInput, PH.maxInput, PH.minOutput, PH.maxOutput, 1);
  Temp.reading = dht.readTemperature();
  //Humidity.reading = filterReading(Humidity, dht.readHumidity());   
}

// To be deleted. Belongs to Micro
double filterReading(Sensor s, double rawReading){
//  double reading = s.get
  s.runavg.addValue(rawReading);
  
  double filtered = s.runavg.getAverage();
  Serial.print("filtered val");
  Serial.println(filtered);
  s.reading = filtered;
  return filtered;
}
*/
/*
// Belongs to Wemos. But can also belong to Nano
String generateSensorGetStr(){
  getSensorReadings();
  String moisture = (String)Moisture.reading;
  String light = (String)Light.reading;
  String temp = (String)Temp.reading;
  String ph = (String)PH.reading;
  String pumpIsOn = (String)Pump.isOn;
  String feederIsOn = (String)Feeder.isOn;
  String parameters = "?moisture=" + moisture + "&ph=" + ph + "&light=" + light + "&temp=" + temp + "&pump=" + pumpIsOn + "&feed=" + feederIsOn;
  String getStr = PATH_UPDATE + parameters;
  Serial.print("generate sensor get string");
  PRINTLN(getStr);
  return getStr;
}

// Belongs to Wemos
void sendSensorReadingToServer(){  
  String getStr = generateSensorGetStr();
  pfcwifi.sendPeriodicRequest(getStr);
//  SendToServer = pfcwifi.sendRequestPerTimeInterval(cs, getStr);
}
*/
