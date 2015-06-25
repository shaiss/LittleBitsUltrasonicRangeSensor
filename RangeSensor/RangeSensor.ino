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


// this constant won't change.  It's the pin number
// of the sensor's output:
const int pingPin = 2; //Sig pin of the Ping))) Sensor
const int ledPin = 6; //The pin the LED is attached to
float brightness = 0; //set the initial value to null

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);  //declare ledPin as an output
}

void loop()
{
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

  //Serial.print(inches);
  //Serial.print("in, ");
  Serial.print(cm);
  //Serial.print("cm");
  Serial.println();

  //convert distance of object to brightness.  
  //Ping))) range is 2cm - 3M/.78in - 118.11in
  //brightness = 144/300.00;
  //brightness = ((((cm/300.00)*255)-255)*-1);  //closer == brighter.  If cm =0 returns 255.
 //Serial.println(brightness);
  brightness = ((cm/300.00))*255; //farther == brighter.  If cm = 300 returns 255
  
  //write distance to LED
  analogWrite(ledPin, brightness);

  //delay to get a fading effect
  delay(50);
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
