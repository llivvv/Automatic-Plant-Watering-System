// pins
const int pump = 7; 
const int sensor = A0; // moisture sensor
const int pot = A3; 
const int trig = 3;
const int echo = 5; 

// calibration constants
const int SENSOR_WET = 457; 
const int SENSOR_DRY = 1019; 
const int MAX_DRY = 1023;

// water supply
const long EMPTY_CUP = 14; // cm
const long MIN_HEIGHT = 3; 

// control constants
const float ALPHA = 0.1; 
// const unsigned long MIN_ON_TIME = 5000; 
const unsigned long WATER_CHECK_INTERVAL = 30000; // 30 s

// runtime variables
long threshold; 
long moistureLevel;
long moistureRaw;
float moistureAvg = 0; 
bool pumpOn = false; 
bool hasEnoughWaterSupply = false;
unsigned long lastWaterCheck = 0; // timer for water supply
int wetCount = 0; 

void setup() {
  Serial.begin(9600);
  
  pinMode(sensor, INPUT);
  pinMode(pot, INPUT);
  pinMode(pump, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
  digitalWrite(pump, LOW);
}

void loop() {
  checkWaterSupply();
  readThresholdAndMoisture();
  actuatePump();
  printInfo();

  delay(250); 
}

void checkWaterSupply() {
  long waterDist = measureWaterDist();
    long waterHeight = EMPTY_CUP - waterDist;
    hasEnoughWaterSupply = (waterHeight >= MIN_HEIGHT);

    if (!hasEnoughWaterSupply) {
      pumpOn = false; 
      digitalWrite(pump, LOW);
    }
}

void actuatePump() {
  if (!pumpOn && threshold > moistureRaw && hasEnoughWaterSupply) {
    pumpOn = true; 
    wetCount = 0; 
    digitalWrite(pump, HIGH);
  } else if (pumpOn) {
    if (isConsistentlyWet(moistureRaw, threshold) || !hasEnoughWaterSupply) {
      pumpOn = false; 
      digitalWrite(pump, LOW);
    }
  }
}

bool isConsistentlyWet(long moisture, long threshold) {
  const int reqCount = 3; 
  if (moisture >= threshold) {
    wetCount++;
  } else {
    wetCount = 0; 
  }
  return (wetCount >= reqCount);
}

void readThresholdAndMoisture() {
  threshold = analogRead(pot);
  long rawSensorReading = analogRead(sensor);
  moistureRaw = MAX_DRY - map(rawSensorReading, SENSOR_WET, SENSOR_DRY, 0, 1023); 

  if (moistureAvg == 0) {
    moistureAvg = moistureRaw; 
  }
  moistureAvg = moistureAvg * (1.0 - ALPHA) + moistureRaw * ALPHA; 
  moistureLevel = (long) moistureAvg; 
}

// Serial output for Processing
void printInfo() {
  Serial.print(moistureLevel);
  Serial.print(",");
  Serial.print(threshold);
  Serial.print(",");
  Serial.println(hasEnoughWaterSupply ? 1 : 0);
}

long measureWaterDist() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH, 30000);
  if (duration == 0) return EMPTY_CUP; // assume empty if no echo

  return (duration / 2) / 29.1; // distance in cm
}
