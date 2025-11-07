
const int pump = 7; 
const int sensor = A0; // moisture sensor
const int pot = A1; 
const int MAX_DRY = 1023; 

int threshold; // if above threshold, stop watering 
int moistureLevel;

void setup() {
  Serial.begin(9600);
  
  pinMode(sensor, INPUT);
  pinMode(pot, INPUT);

  pinMode(pump, OUTPUT);
}

void loop() {
  threshold = analogRead(pot);
  moistureLevel = MAX_DRY - analogRead(sensor);
  Serial.print("threshold: ");
  Serial.println(threshold);
  Serial.print("moisture: ");
  Serial.println(moistureLevel);

  Serial.print("keep watering? ");
  if (threshold > moistureLevel) {
    Serial.println("yes, too dry");
    digitalWrite(pump, HIGH);
  } else {
    Serial.println("no, hydrated!");
    digitalWrite(pump, LOW);
  }
  delay(500);
}
