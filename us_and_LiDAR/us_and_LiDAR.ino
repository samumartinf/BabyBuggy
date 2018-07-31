/*
 *  This code is used to compare the reading between the two ultrasound and the lidar. The extra sensors provide stability and redundancy.
 *  In the end, the smaller, therefore closer measurement will be taken into account. Also, multiple readings are made per device and the
 *  average is taken to reduce the effect of false measurements.
*/
#include <Wire.h>
#include <LIDARLite.h>

// Globals
LIDARLite lidarLite;
const int M1=6;
const int trigPin = 8;
const int echoPin = 9;
const int trigPin2 = 10;
const int echoPin2 = 11;

void setup()
{
  Serial.begin(9600); // Initialize serial connection to display distance readings
  Serial.println("Program Started");

  pinMode(M1, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input


  lidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz
  lidarLite.configure(0); // Change this number to try out alternate configurations
  Serial.println("Loop Started");
}

void loop()
{
  int dist;
  int dist1;
  int dist2;
  long duration;
  int cal_cnt = 0;

  // At the beginning of every 100 readings,
  // take a measurement with receiver bias correction


  dist1 = getDistance(trigPin, echoPin);

  dist2 = getDistance(trigPin2, echoPin2);

  dist3 = getDistanceLidar(lidarLite);

  if((dist2<dist1)&&(dist2!=0))
  {
    dist1 = dist2; //use minimum of both distances for safety
  }

  if(dist3 < dist1 && dist3 != 0)
  {
    dist1 = dist3;
  }

  feedback1(dist1);
}

double getDistance(int trig, int echo)
{
  double duration = 0;
  double distance = 0;
  double distances[5] = {0,0,0,0,0};
  double average = 5;

  for(int n=0; n<average; n++)
  {

    // Clears the trigPin
    digitalWrite(trig, LOW);
    delayMicroseconds(2);

    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echo, HIGH);


    distances[n] = duration *0.034/2;
  }

  for(int n = 0; n<distances.count; n++)
  {
    distance = distance + distances[n];
  }

  distance = distance/average

  return distance
}

double getDistanceLidar(LIDARLite lidar)
{
  double distance = 0;
  double average = 50;

  for (size_t i = 0; i < average; i++) {
    distance = distance + lidar.distance();
  }

  distance = distance / average;

  return distance;
}

void feedback1(int dist)
{
  int thr = 200;
  int crit = 50;

  if(dist<thr && dist>crit)
  {
    digitalWrite(M1, HIGH);
    delay(0.08*(91-dist)*(91-dist));
    digitalWrite(M1, LOW);
    delay(0.08*(91-dist)*(91-dist));
  }

  if((dist<crit)&&(dist!=0))
  {
    digitalWrite(M1, HIGH);
  }

  else
  {
    digitalWrite(M1, LOW);
  }

  Serial.print(dist1);
}
