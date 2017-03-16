
double filterReading(Sensor s, double rawReading){
//  double reading = s.get
  s.runavg.addValue(rawReading);
  double filtered = s.runavg.getAverage();
  s.reading = filtered;
  return filtered;
}

