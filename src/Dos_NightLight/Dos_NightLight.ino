/**
  Created for HPE Girls Technology Camp 2019

  This sketch periodically measures the ambient light and turns on the 
  NeoPixel RGB Led if it determines that there isn't sufficient light.
  It turns the RGB Led off once the light level is acceptable again.
  This somewhat simulates a household sensor night light.

  If you don't already have the Adafruit NeoPixel library installed, please
  install it before trying this sketch out. Installation instructions can be
  found here -
  https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-installation

  The code to communicate with the LTR-303ALS light sensor over I2C is based on the 
  following library but does not use it. So you DO NOT need to install this library.
  https://github.com/automote/LTR303
  
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

#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define LED_RED         5

// Light Sensor I2C Address
#define ALS_I2C_ADDR    0x29

// Light Sensor Register Addresses
#define ALS_CONTR       0x80
#define ALS_MEAS_RATE   0x85
#define ALS_PART_ID     0x86
#define ALS_MANUFAC_ID  0x87
#define ALS_DATA_CH1_0  0x88
#define ALS_DATA_CH1_1  0x89
#define ALS_DATA_CH0_0  0x8A
#define ALS_DATA_CH0_1  0x8B
#define ALS_STATUS      0x8C

/** 
  Light Sensor Gain and Integration Time.
  These values are used in lux calculation.
  Using default gain of 1x.
  Using default integration time of 100ms.
  Map these to ALS_GAIN and ALS_INT as shown
  in the datasheet Appendix A.
*/
#define ALS_GAIN        1.0
#define ALS_INT         1.0

#define MIN_GOOD_LUX    20.0  // Trigger for the night light (NeoPixel).

#define NEOPIXEL_PIN    4  // Neopixel is on pin 4.
#define NEOPIXEL_COUNT  1  // There is only 1 RGB LED on Dos.
#define NEOPIXEL_INDEX  0  // RGB LED is at index 0.

// Create a NeoPixel object 
Adafruit_NeoPixel Pixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

byte I2C_Error = 0;
byte RegisterData = 0x00;
boolean NightLightState = false;

/**
 * Read a byte from the given sensor register.
 */
boolean readRegister(byte Address, byte &Value)
{
  boolean Status = false;
  
  Wire.beginTransmission(ALS_I2C_ADDR);
  Wire.write(Address);
  I2C_Error = Wire.endTransmission();

  if (I2C_Error == 0)
  {
    Wire.requestFrom(ALS_I2C_ADDR, 1);
    
    if (Wire.available() == 1)
    {
      Value = Wire.read();
      Status = true;
    }
  }

  return Status;
}

/**
 * Write given byte to given sensor register.
 */
boolean writeRegister(byte Address, byte Value)
{
  boolean Status = false;
  
  Wire.beginTransmission(ALS_I2C_ADDR);
  Wire.write(Address);
  Wire.write(Value);
  I2C_Error = Wire.endTransmission();
  if (I2C_Error == 0)
  {
    Status = true;
  }

  return Status;
}

/**
 * Calculate lux from CH0 and CH1 sensor data.
 * Formula can be found in the datasheet [Appendix A].
 */
double calculateLux(word CH0, word CH1)
{
  double D0, D1, Ratio, Lux;
  
  // Convert CH data to floating point.
  D0 = CH0;
  D1 = CH1;

  if (!D0 && !D1)
  {
    Lux = 0.0;
  }
  else
  {
    Ratio = D1 / (D0 + D1);
  
    if (Ratio < 0.45)
    {
      Lux = ((1.7743 * D0) + (1.1059 * D1)) / ALS_GAIN / ALS_INT;
    }
    else if ((Ratio < 0.64) && (Ratio >= 0.45))
    {
      Lux = ((4.2785 * D0) - (1.9548 * D1)) / ALS_GAIN / ALS_INT;
    }
    else if ((Ratio < 0.85) && (Ratio >= 0.64))
    {
      Lux = ((0.5926 * D0) + (0.1185 * D1)) / ALS_GAIN / ALS_INT;
    }
    else
    {
      Lux = 0.0;
    }
  }

  return Lux;
}

/**
 * Turn the NeoPixel on/off.
 */
void toggleNightLight(boolean On)
{
  if (On)
  {
    Pixel.setPixelColor(NEOPIXEL_INDEX, 255, 255, 255);
  }
  else
  {
    Pixel.setPixelColor(NEOPIXEL_INDEX, 0, 0, 0);
  }

  Pixel.show();
  NightLightState = On;   
}

void setup() 
{
  // Set baud rate on the serial port.
  Serial.begin(115200);
  Serial.println("HPE GTC Dos says Hello!");
  Serial.println();

  // Turn red LED off which may be dimly lit upon board powerup.
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_RED, LOW);
  
  // Initialize the NeoPixel object.
  Pixel.begin(); 
  Pixel.clear();
  Pixel.show();
  // This is odd but the RGB Led seems to require
  // a duplicate clear() to actually clear.
  // Requires further debug.
  Pixel.clear();
  Pixel.show();
   
  // Light sensor starts up in standby mode.
  // Wait for it to finish initialization.
  delay(200);
  
  // Initialize I2C and join bus as master.
  Wire.begin();

  // Print identification data.
  Serial.print("Light Sensor Identification Data ");
  readRegister(ALS_PART_ID, RegisterData);
  Serial.print("[PartID:0x");
  Serial.print(RegisterData, HEX);
  readRegister(ALS_MANUFAC_ID, RegisterData);
  Serial.print("  ManufacturerID:0x");
  Serial.print(RegisterData, HEX);
  Serial.println("]");
  
  // Move the sensor state from standby to active.
  readRegister(ALS_CONTR, RegisterData);
  Serial.print("Control Register:0x");
  Serial.println(RegisterData, HEX);
  Serial.println();
  RegisterData = RegisterData | 0x01;
  writeRegister(ALS_CONTR, RegisterData);

  // Wait for transition to complete.
  delay(100);

  Serial.println("Initiating periodic measurements...");
  Serial.println();
}

void loop() 
{
  byte DataLow = 0x00;
  byte DataHigh = 0x00;
  word DataCH0 = 0;
  word DataCH1 = 0;
  double Lux = 0.0;
  
  // Measure every 2 seconds.
  delay(2000);

  // Check the Status register. 
  // It should indicate that new data is available to read.
  readRegister(ALS_STATUS, RegisterData);
  if (RegisterData & 0x04)
  {
    // Read CH1 data registers.
    readRegister(ALS_DATA_CH1_0, DataLow);
    readRegister(ALS_DATA_CH1_1, DataHigh);
    DataCH1 = word(DataHigh, DataLow);
    
    // Read CH0 data registers.
    readRegister(ALS_DATA_CH0_0, DataLow);
    readRegister(ALS_DATA_CH0_1, DataHigh);
    DataCH0 = word(DataHigh, DataLow);

    // Calculate lux from CH0 and CH1 data.
    Lux = calculateLux(DataCH0, DataCH1);
    
    Serial.print("CH0:");
    Serial.print(DataCH0);
    Serial.print(" CH1:");
    Serial.print(DataCH1);
    Serial.print(" Lux:");
    Serial.println(Lux);

    // Determine if night light needs to be toggled.
    if ((NightLightState == false) && (Lux <= MIN_GOOD_LUX))
    {
      // It is dark and the light is off. So turn it on.
      toggleNightLight(true);
    }
    else if ((NightLightState == true) && (Lux > MIN_GOOD_LUX))
    {
      // The light is on but it is not dark anymore. So turn it off.
      toggleNightLight(false);
    }
  }
}
