void soilSensorInit(){
  soilSensor.begin();
  delay(1000); // give some time to boot up
  Serial.print("I2C Soil Moisture Sensor Address: ");
  Serial.println(soilSensor.getAddress(),HEX);
  Serial.print("Sensor Firmware version: ");
  Serial.println(soilSensor.getVersion(),HEX);
  Serial.println();
}

void setSensorReadings(){
  float rawMoisture = 700; //(float)soilSensor.getCapacitance();  
  if (rawMoisture <= 855*4) moisturePFC.setCurrentRawReading(rawMoisture);  
  
  float rawLight = 40000; //(float)soilSensor.getLight(true);   
  if (rawLight >= 0) lightPFC.setCurrentRawReading(rawLight);

  phPFC.setCurrentRawReading(analogRead(PH_PIN));
  
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  interrupts();

  
//  Serial.print("temp is"); Serial.println(temp);
  
//  float hum = dht.readHumidity();

  tempPFC.setCurrentRawReading(temp);
  humidityPFC.setCurrentRawReading(humidity);
}

void storeSensorReadings(ReadingsI2C *_readings, PFC_SENSOR *sensor, int mode){
  uint16_t raw = (uint16_t)sensor->getReading();  
  uint16_t mapped = (uint16_t)sensor->getMappedReading(mode);
  uint16_t filtered = (uint16_t)sensor->getFilteredMappedReading(mode);
  _readings->raw = raw;
  _readings->mapped = mapped;
  _readings->filtered = filtered;
  
//  Serial.print("mapped reading: ");
//  Serial.print(_readings->mapped);
//  Serial.print("raw reading: ");
//  Serial.print(_readings->raw);
//  Serial.println();
//  Serial.print("filtered reading: ");
//  Serial.println(_readings->filtered);
}

