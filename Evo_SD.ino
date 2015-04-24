/* 6 SD Data log -----------------------------------------------------------
 6a SDInitialize
 6b SDDataLog
 6c SDWebLoad
 */

/* >>>>>>>>>>>>>>>>>>>>> Constants and Variables <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */ 
File dataFile;                            // will log all data for the run

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Functions<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
// 6a SDInitialize
void SDInitialize() {
  dataFile = SD.open("data.csv", O_CREAT | O_APPEND | O_WRITE);     
  dataFile.print("Program Started: ");
  dataFile.print(hour(tStart));
  dataFile.print(":");
  if (minute(tStart) < 10) {
    dataFile.print("0");
  }
  dataFile.print(minute(tStart));
  dataFile.print(":");
  if (second(tStart) < 10) {
    dataFile.print("0");
  }
  dataFile.print(second(tStart));
  dataFile.print(" ");
  dataFile.print(month(tStart));
  dataFile.print("/");
  dataFile.print(day(tStart));
  dataFile.print("/");
  dataFile.print(year(tStart));
  dataFile.print("\n");
  dataFile.print("Unix Time,Actual Time,Elapsed Time,OD Reading #,Desired OD,Tstat OD,Diode,Pulse #,Max Flowrate,Pulse Volume,Total Volume Added,Instantaneous Flowrate,Temp");
  dataFile.print("\n");
  dataFile.print("s,m/d/y h:mm:ss,s,#,A600,A600,0-1023,#,ml/hr,ml,ml,ml/hr,C");
  dataFile.print("\n");
  dataFile.close();
}

// 6b SDDataLog
void SDDataLog(char c, int count, float value) {
  // Find time as above functions have delay 
  t = now();
  tElapsed = t - tStart;
  tUnix = tUnixStart + tElapsed;
  dataFile = SD.open("data.csv", O_APPEND | O_WRITE);

  // Time
  dataFile.print(tUnix);
  dataFile.print(",");
  dataFile.print(month(t));
  dataFile.print("/");
  dataFile.print(day(t));
  dataFile.print("/");
  dataFile.print(year(t));
  dataFile.print(" ");
  dataFile.print(hour(t));
  dataFile.print(":");
  if (minute(t) < 10) {
    dataFile.print("0");
  }
  dataFile.print(minute(t));
  dataFile.print(":");
  if (second(t) < 10) {
    dataFile.print("0");
  }
  dataFile.print(second(t));
  dataFile.print(",");
  dataFile.print(tElapsed);
  dataFile.print(",");

  // Record OD
  if (c == 'o') {
    dataFile.print(count);
    dataFile.print(",");
    dataFile.print(ODDesired);
    dataFile.print(",");
    dataFile.print(ODMin[0]);
    dataFile.print(",");
    dataFile.print(diodeReading);
    dataFile.print(",");
    dataFile.print(" , , , , ,");
  }

  // Record Pulse
  if (c == 'f') {
    dataFile.print(" , , , ,");
    dataFile.print(count);
    dataFile.print(",");
    dataFile.print(flowMax);
    dataFile.print(",");
    dataFile.print(pulseVol);
    dataFile.print(",");
    dataFile.print(totalVol);
    dataFile.print(",");
    dataFile.print(value);
    dataFile.print(",");
  }

  dataFile.print(tempPrintAvg);
  dataFile.print("\n");
  dataFile.close(); 
}


// 6c SDWebLoad
void SDWebLoad(EthernetClient client) {
  // Header
  char d[32];                                // buffer
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text");        // designate to print txt
  client.println("Content-Disposition: attachment; filename=data.csv");    // download to file
  client.print("\n");
  dataFile = SD.open("data.csv");
  while (dataFile.available()) {
    dataFile.readBytes(d, 31);
    d[31] = 0;
    client.print(d);
  }
  dataFile.close();
}

void SDWebLoadLIMS(EthernetClient client, char *timeStamp) {
  // Header
  char d[32];                                // buffer
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text");        // designate to print txt
  client.println("Content-Disposition: attachment; filename=data.csv");    // download to file
  client.print("\n");
  dataFile = SD.open("data.csv");
  dataFile.seek(0);
  if (dataFile.find(timeStamp)) {
    client.print(timeStamp);
    while (dataFile.available()) {
      dataFile.readBytes(d, 31);
      d[31] = 0;
      client.print(d);
    }
  }
  else {
    client.println("could not find timestamp");
  }
  dataFile.seek(0);
  dataFile.close();
}
