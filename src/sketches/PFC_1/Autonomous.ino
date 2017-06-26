
void autoWatering(){
  double currentMoisture = Moisture.reading;
  if (currentMoisture < AutoWatering.threshold ) { 
    digitalWrite(WATERPUMP_PIN, HIGH);
    Pump.isOn = true;
  }
  if (currentMoisture > AutoWatering.desired ) {
    digitalWrite(WATERPUMP_PIN, LOW);
    Pump.isOn = false;
  }
}

