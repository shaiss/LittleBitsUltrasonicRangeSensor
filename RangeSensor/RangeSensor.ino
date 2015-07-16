/* Ping))) Sensor

   This sketch reads a PING))) ultrasonic rangefinder and returns the
   distance to the closest object in range. To do this, it sends a pulse
   to the sensor to initiate a reading, then listens for a pulse
   to return.  The length of the returning pulse is proportional to
   the distance of the object from the sensor.

   The circuit:
  * +V connection of the PING))) attached to +5V
  * GND connection of the PING))) attached to ground
  * SIG connection of the PING))) attached to digital pin 2

   http://www.arduino.cc/en/Tutorial/Ping

   created 3 Nov 2008
   by David A. Mellis
   modified 06 Aug 2011
   by Tom Igoe
   modified 22 Jun 2015
   by Shai Perednik shaiss@gmail.com

   This example code is in the public domain.

 */

#include <Wire.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

// this constant won't change.  It's the pin number
// of the sensor's output:
const int trimpot = A0;
const int slidebtn = 7;
const int pingPin = 2; //Sig pin of the Ping))) Sensor
const int ledPin = 6; //The pin the LED is attached to

//define some vars
float brightness = 0; //set the initial value to null
int analogVolts = 0; //set initial value to null for voltage out
float trimpotval = 0; //potentiometer on trimpot pin's current position
float maxrange = 0; //max range sensor is set to

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);  //declare ledPin as an output
  pinMode(slidebtn, INPUT); //declare the slidebtn as an input

  // For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VCC)
  // For MCP4725A0 the address is 0x60 or 0x61
  // For MCP4725A2 the address is 0x64 or 0x65
  dac.begin(0x60);  //china board from ebay is 0x60
}

void loop()
{
  //read trimpot's value
  trimpotval = analogRead(trimpot);

  maxrange = (trimpotval/1023)*300;
  
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH  
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  //convert distance of object to brightness.  
  //Ping))) range is 2cm - 3M/.78in - 118.11in
  if (digitalRead(slidebtn) == HIGH) {
    //if slide btn is HIGH or on, then sensor is in close mode
    brightness = ((((cm/maxrange)*255)-255)*-1);  //closer == brighter.  If cm =0 returns 255.
  }
  else {
    //if slide btn is LOW or off, then sensor is in far mode
    brightness = ((cm/maxrange))*255; //farther == brighter.  If cm = 300 returns 255
  }

  if (brightness > 255){brightness = 255;}
  if (brightness < 0) {brightness = 0;}
    
  //Serial.println(brightness);
  
  //write distance to LED
  analogWrite(ledPin, brightness);

  analogVolts = (int) brightness * 16.05; //convert (aka cast) the brightness value from float to int
  
  //start write values to dac
  analogVolts = analogVolts; //upper range of mcp4725 is 4095

  dac.setVoltage(analogVolts, false);
  //end write values to dac

  //Serial.print(inches);
  //Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm  | ");
  Serial.print("brightness: ");
  Serial.print(brightness);
  Serial.print("  aout: ");
  Serial.print(analogVolts);
  Serial.print("  | maxrange: ");
  Serial.print(maxrange);
  Serial.print("  | trimpot: ");
  Serial.print(analogRead(trimpot));
  Serial.print("  | trimpotval: ");
  Serial.print(trimpotval);
  Serial.print("  | slidebtn: ");
  Serial.print(digitalRead(slidebtn));
  Serial.println();
  
  delay(100);
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

int normAnalog(int analogval)
{
  if (analogval < 341)  // Lowest third of the potentiometer's range (0-340)
    {                  
      analogval = (analogval * 3) / 4; // Normalize to 0-255
    }
    else if (analogval < 682) // Middle third of potentiometer's range (341-681)
    {
      analogval = ( (analogval-341) * 3) / 4; // Normalize to 0-255
    }
    else  // Upper third of potentiometer"s range (682-1023)
    {
      analogval = ( (analogval-683) * 3) / 4; // Normalize to 0-255
    }

    return analogval;
}



