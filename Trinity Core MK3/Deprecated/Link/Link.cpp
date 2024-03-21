/*
#include "Link.h"
#include "Trinity/setup.h"

WebServer server(80); //Probably bad. Probably shouldn't put this variable here and put it in the Link class instead. But hey it makes the funny code compile so I'm not complaining.

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}
 
void handleADC()
{
  int a = analogRead(A0);
  String adcValue = String(a);
 
  server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
}




Link::Link()
{
  if(DEBUGLEVEL >= DEBUG_OPERATIONS)
  {
    Serial.print(F("Creating Webserver at adress "));
    Serial.println((int)this, DEC);
  
    Serial.print(F("Connecting to "));
    Serial.println(SSID);
  }
  //server = new WebServer(80);
  WiFi.begin(SSID, PASSWORD);
  connected = false;

  Serial.println(F("...Webserver Started"));
}

void Link::tick()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    connected = false;
    Serial.print(".");
    return;
  }
  else if (!connected) //If connected but not noted down before
  {
    connected = true;
    server.on("/", handleRoot);      //This is display page
    server.on("/readADC", handleADC);//To get update of ADC Value only
    server.begin();
    Serial.println(F("Server connected, IP address: "));
    Serial.println(WiFi.localIP());
  }



  server.handleClient();

  
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client)
  {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println(F("New Client."));          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= TIMEOUTTIME)
    {  // loop while the client's connected
      currentTime = millis();
      if (client.available())
      {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n')
        {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            printPage(client);
            break;
          }
          else
          {
            currentLine = "";
          }
        }
        else if (c != '\r')
        {  
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println(F("Client disconnected.\n"));
  }
  
}
*/

/*
void Link::printPage(WiFiClient client)
{
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println("Connection: close");
  client.println();
  
  // turns the GPIOs on and off
  if (header.indexOf("GET /26/on") >= 0)
  {
    Serial.println(F("GPIO 26 on"));
    output26State = "on";
    digitalWrite(PIN_A, HIGH);
  }
  else if (header.indexOf("GET /26/off") >= 0)
  {
    Serial.println(F("GPIO 26 off"));
    output26State = "off";
    digitalWrite(PIN_A, LOW);
  } 
  else if (header.indexOf("GET /27/on") >= 0)
  {
    Serial.println(F("GPIO 27 on"));
    output27State = "on";
    digitalWrite(PIN_B, HIGH);
  } 
  else if (header.indexOf("GET /27/off") >= 0)
  {
    Serial.println(F("GPIO 27 off"));
    output27State = "off";
    digitalWrite(PIN_B, LOW);
  }
  
  // Display the HTML web page
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  // Web Page Heading
  client.println("<title>Trinity</title>");
  client.println("<body><h1>ESP32 Web Server</h1>");
  // CSS to style the on/off buttons 
  // Feel free to change the background-color and font-size attributes to fit your preferences
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println(".button2{ background-color: #555555;}</style></head>");
  
  
  // Display current state, and ON/OFF buttons for GPIO 26  
  client.println("<p>GPIO 26 - State " + output26State + "</p>");
  // If the output26State is off, it displays the ON button       
  if (output26State=="off")
  {
    client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
  }
  else
  {
    client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
  } 
    
  // Display current state, and ON/OFF buttons for GPIO 27  
  client.println("<p>GPIO 27 - State " + output27State + "</p>");
  // If the output27State is off, it displays the ON button       
  if (output27State=="off")
  {
    client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
  }
  else
  {
    client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
  }
  client.println("</body></html>");
  
  // The HTTP response ends with another blank line
  client.println();
  // Break out of the while loop
}
*/