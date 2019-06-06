/**
  Created for HPE Girls Technology Camp 2019

  This sketch runs a web server on the board and serves up HTML 
  to the browser on the client side to allow the user to control
  the red, blue and green LEDs over the web. 

  The laptop/smartphone needs to be on the same WiFi network as the 
  GTC Dos board for this to work.

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

// LED Pins
#define LED_RED    5
#define LED_GREEN 12
#define LED_BLUE  13

// WiFi credentials.
const char* ssid     = "************";  // Enter your WiFi name here.
const char* password = "************";  // Enter your WiFi password here.

// Create a web server instance on port 80.
WebServer server(80);

/**
 * Generate HTML content based on the state of the pins
 * and return to the client.
 */
void sendResponse()
{
  String htmltext = "<!DOCTYPE html><html>\
                     <head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
                     <link rel=\"icon\" href=\"data:,\">\
                     <style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\
                     .button { background-color: #E7E7E7; border: none; color: white; padding: 16px 40px;\
                     text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}\
                     .button_r {background-color: #F44336;}\
                     .button_g {background-color: #4CAF50;}\
                     .button_b {background-color: #008CBA;}</style></head>\
                     <body><h1>HPE GTC Dos</h1>\
                     <h2>LEDs</h2>";

  // Determine state of Red LED.
  if (digitalRead(LED_RED) == HIGH)
  {
    // LED is ON. Provide visual indication and set href to enable turning off.
    htmltext += "<a href=\"/R/0\"><button class=\"button button_r\">R</button></a>";
  }
  else
  {
    // LED is OFF. Provide visual indication and set href to enable turning on.
    htmltext += "<a href=\"/R/1\"><button class=\"button\">R</button></a>";
  }

  // Determine state of Green LED.
  if (digitalRead(LED_GREEN) == HIGH)
  {
    // LED is ON. Provide visual indication and set href to enable turning off.
    htmltext += "<a href=\"/G/0\"><button class=\"button button_g\">G</button></a>";
  }
  else
  {
    // LED is OFF. Provide visual indication and set href to enable turning on.
    htmltext += "<a href=\"/G/1\"><button class=\"button\">G</button></a>";
  }

  // Determine state of Blue LED.
  if (digitalRead(LED_BLUE) == HIGH)
  {
    // LED is ON. Provide visual indication and set href to enable turning off.
    htmltext += "<a href=\"/B/0\"><button class=\"button button_b\">B</button></a>";
  }
  else
  {
    // LED is OFF. Provide visual indication and set href to enable turning on.
    htmltext += "<a href=\"/B/1\"><button class=\"button\">B</button></a>";
  }

  htmltext += "</body></html>";

  // Return the response to the client.
  server.send(200, "text/html", htmltext);
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

/**
 * Handler to turn Red LED Off.
 */
void handleRedLed_Off()
{
  // Turn the LED off.
  digitalWrite(LED_RED, LOW);
  
  // Send HTTP response.
  sendResponse();
}

/**
 * Handler to turn Red LED On.
 */
void handleRedLed_On()
{
  // Turn the LED on.
  digitalWrite(LED_RED, HIGH);
  
  // Send HTTP response.
  sendResponse();
}

/**
 * Handler to turn Green LED Off.
 */
void handleGreenLed_Off()
{
  // Turn the LED off.
  digitalWrite(LED_GREEN, LOW);
  
  // Send HTTP response.
  sendResponse();
}

/**
 * Handler to turn Green LED On.
 */
void handleGreenLed_On()
{
  // Turn the LED on.
  digitalWrite(LED_GREEN, HIGH);
  
  // Send HTTP response.
  sendResponse();
}

/**
 * Handler to turn Blue LED Off.
 */
void handleBlueLed_Off()
{
  // Turn the LED off.
  digitalWrite(LED_BLUE, LOW);
  
  // Send HTTP response.
  sendResponse();
}

/**
 * Handler to turn Blue LED On.
 */
void handleBlueLed_On()
{
  // Turn the LED on.
  digitalWrite(LED_BLUE, HIGH);
  
  // Send HTTP response.
  sendResponse();
}

void setup()
{
  // Set baud rate on the serial port.
  Serial.begin(115200);
  Serial.println("HPE GTC Dos says Hello!");

  // Initialize LED Pins.
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);
  
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
  server.on("/", handleRoot);             // http://ipaddr/    - Return current state.
  server.on("/R/0", handleRedLed_Off);    // http://ipaddr/R/0 - Turn Red LED off.
  server.on("/R/1", handleRedLed_On);     // http://ipaddr/R/1 - Turn Red LED on.
  server.on("/G/0", handleGreenLed_Off);  // http://ipaddr/G/0 - Turn Green LED off.
  server.on("/G/1", handleGreenLed_On);   // http://ipaddr/G/1 - Turn Green LED on.
  server.on("/B/0", handleBlueLed_Off);   // http://ipaddr/B/0 - Turn Blue LED off.
  server.on("/B/1", handleBlueLed_On);    // http://ipaddr/B/1 - Turn Blue LED on.

  server.begin();
  Serial.println("Web server is now running.");
  Serial.println("Type in the IP address above in your browser to connect.");
}

void loop() 
{
  // Start accepting and processing client connections.
  server.handleClient();
}
