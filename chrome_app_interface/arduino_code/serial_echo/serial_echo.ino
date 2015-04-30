char incoming = 0;
char time[11];
void setup() {
  Serial.begin(9600);
}
int byteCnt = 1;


void loop() {
  while (Serial.available() > 0) {
    // Serial.readBytesUntil(character, buffer, length);
    incoming = Serial.read();
    Serial.print(byteCnt);
    Serial.print(" byte: ");
    Serial.println(incoming);
    byteCnt++;
  }
}

/*
void loop(){
  while (Serial.available() > 0) {
    char lf = 'Q';
    // Expand array size to the number of bytes you expect:
    //byte[] inBuffer = new byte[11];
    byte inBuffer[11];
    Serial.readBytesUntil(lf, inBuffer);
    if (inBuffer != null) {
      String myString = new String(inBuffer);
      println(myString);
    }
  }
}
*/
