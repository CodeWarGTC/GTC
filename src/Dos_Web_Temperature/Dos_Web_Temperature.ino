/**
  Created for HPE Girls Technology Camp 2019

  This sketch runs a web server on the board and serves up HTML 
  to the browser on the client side to allow the user to view
  the temperature measurements read from the LM75C sensor.
  The laptop/smartphone needs to be on the same WiFi network as the 
  Dos board for this to work. The HTML refreshes every 5 seconds to
  fetch new temperature readings. 

  In order for this sketch to work please install the following LM75
  library -
  "Arduino library for I²C Temperature Sensors derived from the LM75"
  which can be found at -
  https://github.com/jeremycole/Temperature_LM75_Derived

  The MIT License

  Copyright (c) 2019 HPE Girls Technology Camp

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
  and associated documentation files (the "Software"), to deal in the Software without restriction, 
  including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
  and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
  subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial 
  portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
  LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <WiFi.h>
#include <WebServer.h>
#include <Temperature_LM75_Derived.h>

// WiFi credentials.
const char* ssid     = "*************";  // Enter your WiFi name here.
const char* password = "*************";  // Enter your WiFi password here.

// Create a web server instance on port 80.
WebServer Server(80);

// Create a temperature sensor object.
Generic_LM75 Sensor;

// Keep track of sensor readings to compare.
float PreviousReading = 0; 
float NewReading      = 0;

/**
 * Generate HTML content and return to the client.
 */
void sendResponse()
{
  String HtmlText;
  char TemperatureStr[6];
  bool ShowIncreased = false;
  bool ShowDecreased = false;

  // Read the temperature from the sensor (in F)
  NewReading = Sensor.readTemperatureF();
  
  Serial.print("Sensor reports ");
  Serial.print(NewReading);
  Serial.println("F");

  // Convert it to string.
  dtostrf(NewReading, 5, 2, TemperatureStr);
  
  // Has the temperature gone up or down?
  if ((PreviousReading != 0) && (PreviousReading != NewReading))
  {
    if (NewReading > PreviousReading)
    {
      ShowIncreased = true;
    }
    else
    {
      ShowDecreased = true;
    }
  }

  PreviousReading = NewReading;

  // Now generate the HTML to return to the browser.
  HtmlText = "<!DOCTYPE html><html>\
             <head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
             <meta http-equiv=\"refresh\" content=\"5\">\
             <link rel=\"icon\" href=\"data:,\">\
             <style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\
             </style></head>\
             <body><h1>HPE GTC Dos</h1>\
             <h2>Temperature Sensor</h2>\
             <p><span style=\"font-size:60pt;\">";
 
  HtmlText += TemperatureStr;
  
  HtmlText += "</span><span style=\"font-size:30pt;\">F</span>";

  // Provide visual indication for temperature variations (up/down).
  if (ShowIncreased)
  {
    // Display upward pointing arrow.
    HtmlText += "<span style=\"color:red;font-weight:bold;font-size:60pt;\">&uarr;</span></p>";
  }
  else if (ShowDecreased)
  {
    // Display downward pointing arrow.
    HtmlText += "<span style=\"color:blue;font-weight:bold;font-size:60pt;\">&darr;</span></p>"; 
  }
  
  HtmlText += "</body></html>";

  // Return the response to the client.
  Server.send(200, "text/html", HtmlText);
}

/**
 * Handler for the root endpoint (landing page).
 */
void handleRoot() 
{
  // Call helper function that builds the HTML content and sends it back 
  // to the client.
  sendResponse();
}

void setup()
{
  // Set baud rate on the serial port.
  Serial.begin(115200);
  Serial.println("HPE GTC Dos says Hello!");

  // Initialize I2C
  Wire.begin();
  
  // Establish WiFi connection. 
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");

  // Initialize and start the server.
  Server.on("/", handleRoot); // http://ipaddr - Return temperature reading.

  Server.begin();
  Serial.println("Web server is now running.");
  Serial.println("Type in the IP address above in your browser to connect.");
  Serial.println();
}

void loop() 
{
  // Start accepting and processing client connections.
  Server.handleClient();
}
