
const int pump = 7;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  pinMode(pump, OUTPUT);
}


// HIGH means will pump water
void loop() {
  digitalWrite(pump, HIGH);
  delay(2000);
  // digitalWrite(pump, LOW);
  // delay(2000);
}
