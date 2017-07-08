
void autoWatering(){
  double currentMoisture = Moisture.reading;
  if (currentMoisture < AutoWatering.threshold ) { 
    digitalWrite(Pump.pin, HIGH);
    Pump.isOn = true;
  }
  if (currentMoisture > AutoWatering.desired ) {
    digitalWrite(Pump.pin, LOW);
    Pump.isOn = false;
  }
}

