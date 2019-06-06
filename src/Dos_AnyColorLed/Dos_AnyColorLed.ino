/**
  Created for HPE Girls Technology Camp 2019

  This sketch demonstrates the functionality of the Adafruit NeoPixel RGB LED. 
  It prompts the user on the serial terminal to provide values (0 to 255) for
  R, G and B components. These values are then mapped to a color that lights
  up the LED.

  If you don't already have the Adafruit NeoPixel library installed, please
  install it before trying this sketch out. Installation instructions can be
  found here -
  https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-installation

  The website below shows a sample RGB color table for reference -
  https://www.rapidtables.com/web/color/RGB_Color.html#color-table

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

#include <Adafruit_NeoPixel.h>

#define LED_RED         5

#define NEOPIXEL_PIN    4  // Neopixel is on pin 4.
#define NEOPIXEL_COUNT  1  // There is only 1 RGB LED on Dos.
#define NEOPIXEL_INDEX  0  // RGB LED is at index 0.

#define MIN_COLOR_LIMIT 0    // Min R,G,B value.
#define MAX_COLOR_LIMIT 255  // Max R,G,B value.
 
// Create a NeoPixel object 
Adafruit_NeoPixel Pixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

void setup()
{
  // Set baud rate on the serial port.
  Serial.begin(115200);
  Serial.println("HPE GTC Dos says Hello!");

  // Turn red LED off which may be on upon board powerup.
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_RED, LOW);
  
  // Initialize the NeoPixel object.
  Pixel.begin(); 

  // Set all pixel colors to OFF
  Pixel.clear();
  Pixel.show(); 

  // Show syntax for providing R,G,B values.
  Serial.println();
  Serial.println();
  Serial.println("Type comma separated values [0...255] for R, G and B in the text window above");
  Serial.println("and then press ENTER.");
  Serial.println("Example: 128,0,128<ENTER> to glow purple!");
  Serial.println();
}

void loop()
{
  // Read values from the serial port.
  while (Serial.available() > 0)
  {
    int red = Serial.parseInt();
    int green = Serial.parseInt();
    int blue = Serial.parseInt();

    // A newline character indicates end of input.
    if (Serial.read() == '\n')
    {
      // Adjust the values if out of range.
      red = constrain(red, MIN_COLOR_LIMIT, MAX_COLOR_LIMIT);
      green = constrain(green, MIN_COLOR_LIMIT, MAX_COLOR_LIMIT);
      blue = constrain(blue, MIN_COLOR_LIMIT, MAX_COLOR_LIMIT);
      
      Serial.print("Setting the RGB LED with");
      Serial.print(" R=");
      Serial.print(red, DEC);
      Serial.print(" G=");
      Serial.print(green, DEC);
      Serial.print(" B=");
      Serial.println(blue, DEC);

      // setPixelColor() takes RGB values, from 0,0,0 up to 255,255,255.
      Pixel.setPixelColor(NEOPIXEL_INDEX, red, green, blue);

      // Push the color data to the hardware.
      Pixel.show();   
    }
  }
}
