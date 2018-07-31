// defines pins numbers
const int M1=3;
const int trigPin = 4;
const int echoPin = 5;
const int trigPin2=11;
const int echoPin2=12;

double getDistance(int trig, int echo);

void setup()
{
  pinMode(M1, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}

void loop()
{
  // defines variables

  double dist1 = 0;
  double dist2 = 0;
  double vel1 = 0;
  double vel2 = 0;

  unsigned long start = micros();//Start measuring time it takes for one reading

  dist1 = getDistance(trigPin, echoPin);
  dist2 = getDistance(trigPin2, echoPin2);

  vel1 = getSpeed(trigPin, echoPin);
  vel2 = getSpeed(trigPin2, echoPin2);

  if (dist2 != 0 && dist2 < dist1)
  {
    dist1 = dist2;
  }

  if(dist1 != 0)
  {
    //feedback1(dist1);
  }
  Serial.println(vel1);


}

double getSpeed(int trig, int echo)
{
  double duration1 = 0;
  double duration2 = 0;

  // Clears the trigPin
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  int t1 = micros();
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration1 = pulseIn(echo, HIGH);


  // Clears the trigPin
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  int t2 = micros();

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration2 = pulseIn(echo, HIGH);

  double deltaDist = (duration2 - duration1) * 0.034/2;
  double velocity = deltaDist/((t2-t1)*100);

  return velocity;
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

  double t2 = micros();

  for(int n = 0; n<average; n++)
  {
    distance = distance + distances[n];
  }

  distance = distance/average;

  return distance;
}

void feedback1(int dist)
{
  int thr = 80;
  int crit = 20;

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
}
