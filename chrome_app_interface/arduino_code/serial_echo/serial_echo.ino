int incoming = 0;
void setup() {
  Serial.begin(9600);
}
void loop() {
  while (Serial.available() > 0) {
    incoming = Serial.read();
    Serial.println(incoming);
  }
}
