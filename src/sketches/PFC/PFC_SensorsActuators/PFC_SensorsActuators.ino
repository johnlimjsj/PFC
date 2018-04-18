#include "PFC_SensorsActuator_Settings.h"

DHT dht(DHTPIN, DHTTYPE);
I2CSoilMoistureSensor soilSensor(0x20);

struct ReadingsI2C{
  uint16_t raw;
  uint16_t mapped;
  uint16_t filtered;  
};

struct ReadingsI2C moistureI2CReading = {.raw = -1, .mapped = -1, .filtered = -1 };
struct ReadingsI2C lightI2CReading    = {.raw = -1, .mapped = -1, .filtered = -1 };
struct ReadingsI2C humidityI2CReading = {.raw = -1, .mapped = -1, .filtered = -1 };
struct ReadingsI2C tempI2CReading     = {.raw = -1, .mapped = -1, .filtered = -1 };
struct ReadingsI2C phI2CReading       = {.raw = -1, .mapped = -1, .filtered = -1 };

// pfcsensor(minInput, maxInput)
PFC_SENSOR moisturePFC(320, 855);
PFC_SENSOR lightPFC(236, 60000);
PFC_SENSOR humidityPFC(0, 100);
PFC_SENSOR tempPFC(0, 100);
PFC_SENSOR phPFC(0, 1024, 0, 140);

// pfcactuator(pin, period_milliseconds, dutycycle, activehigh)
PFC_ACTUATOR pumpPFC(5, 3600000, 0.66, false);
PFC_ACTUATOR ledPFC(6, 3600000, 0.33, true);
PFC_ACTUATOR fanPFC(7, 100000, 0.5, true); 

typedef union float2bytes_t   // union consists of one variable represented in a number of different ways 
{ 
  float f; 
  byte b[sizeof(float)]; 
}; 

float2bytes_t f2b; 
byte controlByte;
bool isManualActuate = false;

void setup() {
  Serial.begin(9600);           // start serial for output
  pinMode(PH_PIN, INPUT);
  pumpPFC.init();
  ledPFC.init();
  fanPFC.init();
  dht.begin();
  // Initializing as slave, wrt to the wemos
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event

  // Initializing as master, wrt the soil moisture sensor
//  soilSensorInit();
  pinMode(5, OUTPUT);

  
}

void loop() {
  analogWrite(5, 120);

  /*
  
//  Serial.println(controlByte);
  // automatic actuation. manual actuation occurs when an event is received
  
  setSensorReadings();
  
  
  storeSensorReadings(&moistureI2CReading, &moisturePFC, PFC_SENSOR::LINEAR_MAPPING);
  storeSensorReadings(&lightI2CReading, &lightPFC, PFC_SENSOR::LINEAR_MAPPING);
  storeSensorReadings(&humidityI2CReading, &humidityPFC, PFC_SENSOR::LINEAR_MAPPING);
  storeSensorReadings(&tempI2CReading, &tempPFC, PFC_SENSOR::LINEAR_MAPPING);
  storeSensorReadings(&phI2CReading, &phPFC, PFC_SENSOR::LINEAR_MAPPING);
  
  if (!isManualActuate){
    pumpPFC.periodControl();
    ledPFC.periodControl();
    fanPFC.periodControl();
  }

  */
}


void requestEvent() {
  int toWrite;
  switch(controlByte){
    case BYTE_GET_CURR_MOISTURE:
      toWrite = moistureI2CReading.mapped;
      break;
    case BYTE_GET_CURR_LIGHT:
      toWrite = lightI2CReading.mapped;
      break;
    case BYTE_GET_CURR_TEMP:
      toWrite = tempI2CReading.mapped;
//      Serial.print("temp :"); Serial.println(toWrite);
      break;
    case BYTE_GET_CURR_PH:
      toWrite = phI2CReading.mapped;
      break;
    case BYTE_GET_CURR_HUMIDITY:
      toWrite = humidityI2CReading.mapped;
      break;
    case BYTE_GET_FILTERED_MOISTURE:
      toWrite = moistureI2CReading.filtered;
      break;
    case BYTE_GET_FILTERED_CURR_LIGHT:
      toWrite = lightI2CReading.filtered;
      break;
    case BYTE_GET_FILTERED_CURR_TEMP:
      toWrite = tempI2CReading.filtered;
      break;
    case BYTE_GET_FILTERED_CURR_PH:
      toWrite = phI2CReading.filtered;
      break;
    case BYTE_GET_FILTERED_CURR_HUMIDITY:
      toWrite = humidityI2CReading.filtered;
      break;
    default:
      return;
      break;    
  }
  // guard to prevent overflow, or rubbish data from being written
  if(toWrite<0 || toWrite>255) return;
  
  Wire.write(toWrite); 
}

void receiveEvent(int howMany) {  
  
  if(Wire.available() > 0){
    controlByte = Wire.read(); 
    executeCommand(controlByte);
  }  
}

void executeCommand(byte _controlByte){

  // First check if control byte is allowing user to change the operation mode
  switch(_controlByte){
    case BYTE_SLAVE_AUTO:
      isManualActuate = false;
      break;
    case BYTE_SLAVE_MANUAL:
      isManualActuate = true;
      break;
    default:
      break;
  }

  if(!isManualActuate) return;

  // if in manual mode, then can manually set the actuators
  switch(_controlByte){
    case BYTE_MANUAL_SET_PUMP_OFF:
      pumpPFC.digitalOff();
      break;
    case BYTE_MANUAL_SET_PUMP_ON:
      pumpPFC.digitalOn();
      break;
    case BYTE_MANUAL_SET_LIGHT_OFF:
      ledPFC.digitalOff();
      break;
    case BYTE_MANUAL_SET_LIGHT_ON:
      ledPFC.digitalOn();
      break;
    case BYTE_MANUAL_SET_FAN_OFF:
      fanPFC.digitalOff();
      break;
    case BYTE_MANUAL_SET_FAN_ON:
      fanPFC.digitalOn();
      break;
    case BYTE_MANUAL_SET_FEEDER_OFF:
      break;
    case BYTE_MANUAL_SET_FEEDER_ON:
      break;
    default:
      break;
  }
}





