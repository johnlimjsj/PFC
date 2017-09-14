/*
double mapDouble(double in_reading, double in_min, double in_max, double out_min, double out_max, uint8_t mode){
    double out_reading;
    // Saturation limits
    if(in_reading>in_max){in_reading = in_max;}
    if(in_reading<in_min){in_reading = in_min;}
    if(in_reading == NULL){Serial.println("null input into the mapping function"); return NULL;};
    switch(mode) {     
      // linear mapping
      case 1:{ out_reading = out_min + (double)(out_max - out_min)*(double)((double)(in_reading - in_min)/(in_max - in_min)); break;}
      // inverse linear mapping
      case 2:{ out_reading = (double)out_max - (double)(out_max - out_min)*((double)(in_reading - in_min)/(in_max - in_min)); break;}
      default:{ out_reading = NULL; }
    }
    return out_reading;
}
*/

JsonObject& getParsedJsonObject(String jsonStr){
  int size = jsonStr.length() + 1;
  char json[size]; 
  jsonStr.toCharArray(json, size);
  DynamicJsonBuffer jsonBuffer;
  JsonObject &json_parsed = jsonBuffer.parseObject(json);
  if (!json_parsed.success()){ 
    Serial.println("parseObject() failed");
  }
  return json_parsed;
}


