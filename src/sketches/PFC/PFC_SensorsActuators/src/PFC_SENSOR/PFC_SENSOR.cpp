#include "PFC_SENSOR.h"



PFC_SENSOR::PFC_SENSOR(float minInput, float maxInput){
    _minInput = minInput; _maxInput = maxInput;
    runavg = new RunningAverage(5);
}

PFC_SENSOR::PFC_SENSOR(float minInput, float maxInput, float minOutput, float maxOutput){
    _minInput = minInput; _maxInput = maxInput;
    _maxInput = maxInput; _maxOutput = maxOutput;
    runavg = new RunningAverage(5);
};

float PFC_SENSOR::getMappedReading(int mode){

    return _mapDouble(_reading, _minInput, _maxInput, _minOutput, _maxOutput, mode);
}


float PFC_SENSOR::getFilteredReading(){ 
    return runavg->getAverage(); 
}

float PFC_SENSOR::getFilteredMappedReading(int mode){
    double filteredReading = runavg->getAverage();
    return _mapDouble(filteredReading, _minInput, _maxInput, _minOutput, _maxOutput, mode);
}

void PFC_SENSOR::setCurrentRawReading(float reading){ 
    _reading = reading;
    runavg->addValue(reading);
}


double PFC_SENSOR::_mapDouble(double in_reading, double in_min, double in_max, double out_min, double out_max, int mode){
    double out_reading;
    // Saturation limits
    if(in_reading>in_max){in_reading = in_max;}
    if(in_reading<in_min){in_reading = in_min;}
    if(in_reading == NULL){Serial.println("null input into the mapping function"); return NULL;};
    switch(mode) {     
      // linear mapping
      case LINEAR_MAPPING:{ out_reading = out_min + (double)(out_max - out_min)*(double)((double)(in_reading - in_min)/(in_max - in_min)); break;}
      // inverse linear mapping
      case INVERSE_LINEAR_MAPPING:{ out_reading = (double)out_max - (double)(out_max - out_min)*((double)(in_reading - in_min)/(in_max - in_min)); break;}
      default:{ out_reading = NULL; }
    }
    return out_reading;
}














