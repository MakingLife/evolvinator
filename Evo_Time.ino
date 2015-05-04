// 4 Time -----------------------------------------------------------

// timeCheck
void timeCheck() {
  t = now();
  if (year(t) == 2015 && currentMs - msBackup > 5 * 60000) {
    tBackup = now();
    msBackup = millis();
    if (debugMode) {
      Serial.print("Back up time: ");
      Serial.println(tBackup);
    }
  }
  if (year(t) != 2015) {
    if (debugMode) {
      Serial.println("Year is wrong: ");
      Serial.println(year(t));
    }
    setTime(hour(tBackup), minute(tBackup), second(tBackup), day(tBackup), month(tBackup), year(tBackup));
    int adjustTime = (currentMs - msBackup) / 1000;
    setTime(hour(tBackup), minute(tBackup) + adjustTime / 60, second(tBackup) + adjustTime % 60, day(tBackup), month(tBackup), year(tBackup));
    t = now();
    if (debugMode) {
      Serial.println("Time failed to reset correctly, backup used");
      Serial.print("Is year right? ");
      Serial.println(year(t));
    }
  }
  if (tStart) {
    tElapsed = t - tStart;
    tUnix = tUnixStart + tElapsed;
  }
}
