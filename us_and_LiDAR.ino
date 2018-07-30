/*
 *  This code is used to compare the reading between the two ultrasound and the lidar. The extra sensors provide stability and redundancy.
 *  In the end, the smaller, therefore closer measurement will be taken into account. Also, multiple readings are made per device and the
 *  average is taken to reduce the effect of false measurements.
*/
#include <Wire.h>
#include <LIDARLite.h>

// Globals
LIDARLite lidarLite;
int cal_cnt = 0;
int thr=200;
int crit=50;
const int M1=6;
const int trigPin = 8;
const int echoPin = 9;
const int trigPin2 = 10;
const int echoPin2 = 11;

long duration;
int dist1;
int distanc;
int thr_u=75;
int crit_u=50;
int average = 5;

int dist2;

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

  // At the beginning of every 100 readings,
  // take a measurement with receiver bias correction
  if ( cal_cnt == 0 ) {
    dist = lidarLite.distance();      // With bias correction
  } else {
    dist = lidarLite.distance(false); // Without bias correction
  }

  // Increment reading counter
  cal_cnt++;
  cal_cnt = cal_cnt % 100;

  // Display distance
  Serial.print(dist);
  Serial.println(" cm");

  dist1=0;
  for(int n=0; n<average; n++)
  {
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);


    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);

    // Calculating the distance
    distanc = duration*0.034/2;

    // Prints the distance on the Serial Monitor

    dist1=dist1+distanc;
    }

    dist1 = dist1/average;
    Serial.println(dist1);

    dist2=0;
    for(int n=0; n<average; n++)
    {
      // Clears the trigPin
      digitalWrite(trigPin2, LOW);
      delayMicroseconds(2);

      // Sets the trigPin on HIGH state for 10 micro seconds
      digitalWrite(trigPin2, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin2, LOW);


      // Reads the echoPin, returns the sound wave travel time in microseconds
      duration = pulseIn(echoPin2, HIGH);

      // Calculating the distance
      distanc = duration*0.034/2;

      // Prints the distance on the Serial Monitor

      dist2=dist2+distanc;
    }

    dist2 = dist2/average;
      Serial.println(dist2);

    if((dist2<dist1)&&(dist2!=0)) dist1=dist2; //Distance 1 will be the minimum of both measurments (dist1 & dist2)
    dist1+=10; // For stability

    if((dist1<thr_u)&&(dist1!=0)&&(dist1<dist)){ //Case for ultrasound a smaller ultrasound reading
      if(dist1>crit)
       {
          digitalWrite(M1, HIGH);
          delay(2*(thr-dist1));
          digitalWrite(M1, LOW);
          delay(2*(thr-dist1));
       }

       else
       {
          digitalWrite(M1, HIGH);
       }
    }
    else{
      if(dist < 5){
      return;
    }
    else if(dist<thr && dist>crit) //Case the reading of the lidar is smaller
    {
      digitalWrite(M1, HIGH);
      delay(2*(thr-dist));
      digitalWrite(M1, LOW);
      delay(2*(thr-dist));
    }
    else if(dist<=crit)
    {
        digitalWrite(M1, HIGH);
      }
      else{
        digitalWrite(M1,LOW);
      }
    }
}
