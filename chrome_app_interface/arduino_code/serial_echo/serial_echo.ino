int incoming = 0;
void setup() {
  Serial.begin(9600);
}
void loop() {
  int byteCnt = 1;
  while (Serial.available() > 0) {
    incoming = Serial.read();
    Serial.print(byteCnt);
    Serial.print(" byte: ");
    Serial.println(incoming);
    byteCnt++;
  }
}
