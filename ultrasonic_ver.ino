// pins
const int pump = 7; 
const int sensor = A0; // moisture sensor
const int pot = A3; 

const int trig = 3;
const int echo = 5; 

const int MAX_DRY = 1023;

// for cup of water 
const long EMPTY_CUP = 8; // empty water supply reads 8cm
const long MIN_HEIGHT = 2; // water supply height for sufficient water to keep pumping

int threshold; // if above threshold, stop watering 
int moistureLevel;

void setup() {
  Serial.begin(9600);
  
  pinMode(sensor, INPUT);
  pinMode(pot, INPUT);

  pinMode(pump, OUTPUT);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

void loop() {

  waitForWaterSupply();
  readThresholdAndMoisture();
  determinePump();
  delay(500);
  
}

void determinePump() {
  Serial.print("keep watering? ");
  if (threshold > moistureLevel) {
    Serial.println("yes, too dry");
    digitalWrite(pump, HIGH);
  } else {
    Serial.println("no, hydrated!");
    digitalWrite(pump, LOW);
  }
}

void readThresholdAndMoisture() {
  threshold = analogRead(pot);
  moistureLevel = MAX_DRY - analogRead(sensor);
  Serial.print("threshold: ");
  Serial.println(threshold);
  Serial.print("moisture: ");
  Serial.println(moistureLevel);
}

// if not enough water in glass cup, don't pump
void waitForWaterSupply() {
  bool enoughWaterSupply = false; 

  while (!enoughWaterSupply) {
    long waterDist = measureWaterDist();

    // strange reading if cup is full
    if (waterDist >= 200) waterDist = 0; 

    long waterHeight = EMPTY_CUP - waterDist; 
    enoughWaterSupply = (waterHeight >= MIN_HEIGHT);

    if (!enoughWaterSupply) {
      digitalWrite(pump, LOW);
      delay(30000);
    }
  }
}

long measureWaterDist() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH, 30000);
  return (duration / 2) / 29.1;
}