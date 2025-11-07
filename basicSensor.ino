const int sensorPin = A0; // moisture sensor

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  int moistureLevel = analogRead(sensorPin);
  Serial.print("moisture: ");
  Serial.println(moistureLevel);
  Serial.println("--------");
  delay(2000);
}
