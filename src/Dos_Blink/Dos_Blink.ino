/**
  Created for HPE Girls Technology Camp 2019

  This simple sketch blinks the red, green and blue LEDs in tandem once
  every second. This sketch serves as a good first lesson to gettting
  started with learning to program on the HPE GTC Dos boards.

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

// LED Pins
#define LED_RED    5
#define LED_GREEN 12
#define LED_BLUE  13

void setup() {
  // put your setup code here, to run once:

  // Set baud rate on the serial port.
  Serial.begin(115200);
  Serial.println("HPE GTC Dos says Hello!");
  Serial.println();

  // Initialize LED Pins.
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Turn LEDs on
  Serial.println("Turning LEDs on...");
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);

  // Sleep for a second
  delay(1000);

  // Turn LEDs off
  Serial.println("Turning LEDs off...");
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);
  
  // Sleep for a second
  delay(1000);
}
