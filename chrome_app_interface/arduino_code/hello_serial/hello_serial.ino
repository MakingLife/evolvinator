#define LED 13

void setup() {
  Serial.begin(9600);

  pinMode(LED, OUTPUT);
 
  // Turn Led off
  digitalWrite(LED, LOW);

  // Print the led Status to the serial in JSON format
  printLEDStatus();
}

int incomingByte = 0;
void loop() {

  
  // Check if there's a serial message waiting.
  if (Serial.available() > 0) {
    // If there is, read the incoming byte.
    incomingByte = Serial.read();
    
    // if "y" turn on the led, otherwise turn off 
    if (incomingByte == 'y') {
      digitalWrite(LED, HIGH);
    } else if (incomingByte == 'n') {
      digitalWrite(LED, LOW);
    }

    printLEDStatus();
  }
  
  delay(1000);

}

// Send the Led status via serial in JSON format
void printLEDStatus () {
   // read current LED status
    int ledStatus = digitalRead(LED);
    
    // Create the JSON to send
    String json = "{\"ledStatus\": " + String(ledStatus) + "}";
    
    // Print JSON via serial
    Serial.println(json);
  
}
