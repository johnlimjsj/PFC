#ifndef PFC_SENSOR_h
#define PFC_SENSOR_h

#include <Arduino.h>
#include "RunningAverage.h"


class PFC_SENSOR{
	public:
		PFC_SENSOR(float minInput, float maxInput);
		PFC_SENSOR(float minInput, float maxInput, float minOutput, float maxOutput);
		void setCurrentRawReading(float reading);

		float getReading(){ return _reading; };
		float getFilteredReading();

		float getMappedReading(int mode);
		float getFilteredMappedReading(int mode);
		
		static const int LINEAR_MAPPING = 0;
		static const int INVERSE_LINEAR_MAPPING = 1;

	private:
		RunningAverage *runavg;
		String _verbose;
		float _reading=-1;
		float _minInput, _maxInput;
		float _maxOutput = 100, _minOutput = 0;
		double _mapDouble(double in_reading, double in_min, double in_max, double out_min, double out_max, int mode);
};



#endif