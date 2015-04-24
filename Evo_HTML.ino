// 5 Web Interface -----------------------------------------------------------

/* >>>>>>>>>>>>>>>>>>>>> Constants and Variables <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

boolean reload = false;           // variable to control redirecting, was bugging when I made it local

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Functions <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
// 5a webLoop - prints data to webserver if there is a request
void webLoop() {
  boolean dataRead = false;                // flag for if data has been read
  EthernetClient client = server.available();
  if (client) {                                         
    while (client.connected()) {
      if (client.available()) {
        parseHttpHeader(client, &dataRead);

        if (dataRead == true) {             // don't print rest of html code if data has been viewed. 
          break;
        }
        if (reload == true) {             // redirect to main page if parameter has been changed
          htmlRedirect(client);
          client.stop();
          webLoop();
          break;
        }

        htmlHeader(client);

        client.print("The Evolvinator");
        client.print("<br />");
        client.print("<a href=\"http://pterodactyl.ginkgobioworks.com:8081\" target=\"_blank\">View Evolvinator</a>");
        client.print("<p>");
        displayData(client, "Volume", volume, "ml");
        displayData(client, "OD Threshold", ODDesired, "A600");
        displayData(client, "OD Zero Value", ODZero, "");
        displayData(client, "Maximum Flow", flowMax, "ml/hr");
        displayData(client, "Pulse Volume", pulseVol, "ml");
        displayData(client, "Max Pulse Frequency", float(feedFrequency / 1000), "seconds");
        displayData(client, "Temperature Desired", tempDesired, "C");
        client.print("</p>");

        // Pre run optionst
        if (!tStart) {      
          submitButton(client, "Begin Evolvination!", "s");
        }

        // Run Data
        // Temp
        client.print("<p>");
        displayData(client, "One Minute Temperature Average", tempPrintAvg, "C");
        client.print("</p>");
        if (tStart) {
          // Time
          client.print("<p>");
          displayTime(client, "Run Started", tStart, true, false);
          displayTime(client, "Elapsed Time", tElapsed, false, true);
          client.print("</p>");
          // OD
          client.print("<p>");
          displayData(client, "Last OD Measurement", ODMin[0], "A600");
          displayData(client, "3 Minute OD Average", OD3MinAvg, "A600");
          client.print("</p>");
          // Flow
          client.print("<p>");
          if (tPulse) {
            displayTime(client, "Last Pulse At", tPulse, true, false);
          } 
          else {
            client.print("Last Pulse At: Threshold Not Reached");
          }
          client.print("</p>");       
        }

        // Buttons
        submitButton(client, "Download Data", "d");
        fieldSubmitButton(client, "Change OD Threshold:", "o");
        fieldSubmitButton(client, "Change Max Flowrate (ml/h):", "f");
        fieldSubmitButton(client, "Change Temperature:", "t");
        submitButton(client, "Zero OD Sensor", "z");
        fieldSubmitButton(client, "Add Media (ml):", "a");

        submitButton(client, "Toggle Debug Mode", "i");
        if (debugMode) {
          client.print("Debug:<br />");
          client.print("tstat1v5.5<br />Unix Time ");
          client.print(tUnix);
          client.print("<br />tElapsed ");
          client.print(tElapsed);
          client.print("<br />msElapsedPrestart ");
          client.print(msElapsedPrestart);
        }

        htmlFooter(client);

        break;  
      }
    }
    delay(1);               // give the web browser time to receive the data
    client.stop();          // close the connection:
  }
}

void htmlHeader(EthernetClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println("<HTML>\n<HEAD>");
  client.println("<TITLE>The Evolvinator</TITLE>");//
  client.println("</HEAD><BODY bgcolor=\"#FFFFFF\">");
}

void htmlFooter(EthernetClient client) {
  client.println("</BODY></HTML>");
}

void htmlRedirect(EthernetClient client) {
  client.println("<HTML>\n<HEAD>");
  client.println("<meta http-equiv='Refresh' content='0;url=http://192.168.100.53' />");
  client.println("<BODY>Redirecting</BODY>");
  client.println("</HEAD>\n</HTML>");
}

void displayData(EthernetClient client, char *label, float value, char *unit) {
  client.print(label);
  client.print(" = ");
  client.print(value);
  client.print(" ");
  client.print(unit);
  client.print("<br />");
}

void displayTime(EthernetClient client, char *label, unsigned long time, boolean date, boolean days) {
  client.print(label);
  client.print(": ");
  if (days) {
    client.print(time / 86400);
    client.print(":");
  }
  client.print(hour(time));
  client.print(":");
  if (minute(time) < 10) {
    client.print("0");
  }
  client.print(minute(time));
  client.print(":");
  if (second(time) < 10) {
    client.print("0");
  }
  client.print(second(time));
  if (date) {
    client.print(" ");
    client.print(month(time));
    client.print("/");
    client.print(day(time));
    client.print("/");
    client.print(year(time));
  }
  client.print("<br />");
}

void submitButton(EthernetClient client, char *label, char *identifier) {
  client.print("<form method=post action=\"/?");
  client.print(identifier);
  client.print("\"><input type=submit value=\"");
  client.print(label);
  client.print("\">");
  client.print("</form>");
}

void fieldSubmitButton(EthernetClient client, char *fieldLabel, char *identifier) {
  client.print("<form method=get action=\"/?\">");
  client.print(fieldLabel);
  client.print("<input type=txt name=\"");
  client.print(identifier);
  client.print("\"> <input type=submit value=\"Submit\">");
  client.print("</form>");
} 

void parseHttpHeader(EthernetClient client, boolean *dataRead) {
  char c;
  while((c = client.read()) != '?' && client.available()) { // Skip through until a question mark (first one) 
  }
  if (client.available()) {
    c = client.read();
  }
  else {
    return;
  }
  // Change Parameters
  switch (c) {      // Change OD
  case 'o':
    client.read();             // skip = sign
    char ODHtml[5];      // 4 character string (3 numbers with decimal)
    for (int i=0; i < 4; i++) {
      ODHtml[i] = client.read(); // Read and dump data to debug port
    }
    ODDesired = atof(ODHtml);
    reload = true;
    break;
  case 'f':  
    client.read();             // skip = sign
    char flowHtml[6];      // 5 character string (3 numbers with decimal)
    for (int i=0; i < 5; i++) {
      flowHtml[i] = client.read(); // Read data
    }
    flowMax = atof(flowHtml);
    flowSet();
    reload = true;
    break; 
  case 't':   
    client.read();             // skip = sign
    char tempHtml[5];      // 4 character string (3 numbers with decimal)
    for (int i=0; i < 4; i++) {
      tempHtml[i] = client.read(); // Read data
    }
    tempDesired = atof(tempHtml);
    tempSet();
    reload = true;
    break;
  case 's':
    startRun();
    reload = true;    
    break;
  case 'z':
    ODCalibrate();
    reload = true;
    break;
  case 'd':
    SDWebLoad(client);
    *dataRead = true;
    break;
  case 'e':
    c = client.read();
    char unixTimeStamp[11];
    for (int i=0; i < 10; i++) {
      unixTimeStamp[i] = client.read(); //record unix timestamp
    }
    SDWebLoadLIMS(client, unixTimeStamp);
    *dataRead = true;
    break;
  case 'a':
    {  
      client.read();             // skip = sign
      char addMediaHtml[5];      // 4 character string (3 numbers with decimal)
      for (int i=0; i < 4; i++) {
        addMediaHtml[i] = client.read(); // Read and dump data to debug port
      }
      float addMediaMl = atof(addMediaHtml);
      addMedia(addMediaMl);
      reload = true;
      break;
    }
  case 'i':
    if (debugMode) debugMode = false;
    else debugMode = true;
    reload = true;
    break;
  }
}



