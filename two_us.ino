// defines pins numbers
const int M1=3;
const int trigPin = 4;
const int echoPin = 5;
const int trigPin2=11;
const int echoPin2=12;

// defines variables
long duration;
int dist;
int distanc;
int thr=75; //threshold
int crit=25; //critical value
int average = 5;

int dist2;

void setup(){
  pinMode(M1, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}

void loop(){
  dist=0;
  unsigned long start = micros();//Measure the time it took

  for(int n=0; n<average; n++){

    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);

    // Calculating the distanc
    distanc = duration*0.034/2;

    // Prints the distanc on the Serial Monitor
    Serial.print("distanc: ");
    Serial.println(distanc);
    dist=dist+distanc;
  }
  dist = dist/average; //Find the average distance
  // Compute the time it took
  unsigned long end = micros();
  unsigned long delta = end - start;
  Serial.println(delta)



  dist2=0;
  for(int n=0; n<average; n++){
    // Clears the trigPin2
    digitalWrite(trigPin2, LOW);
    delayMicroseconds(2);

    // Sets the trigPin2 on HIGH state for 10 micro seconds
    digitalWrite(trigPin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin2, LOW);

    // Reads the echoPin2, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin2, HIGH);

    // Calculating the distanc
    distanc = duration*0.034/2;
    Serial.println(distanc);

    // Prints the distanc on the Serial Monitor
    dist2=dist2+distanc;
  }

  //Choose the lowest value to use as output
  dist2 = dist2/average;
  if(dist==0) dist=dist2;
  else if((dist2<dist)&&(dist2!=0)) dist=dist2;

  //Turn the motor on for an experimentally determined amount of time
  if(dist<thr && dist>crit){
    digitalWrite(M1, HIGH);
    delay(0.08*(91-dist)*(91-dist));
    digitalWrite(M1, LOW);
    delay(0.08*(91-dist)*(91-dist));
  }

  if((dist<crit)&&(dist!=0)){
    digitalWrite(M1, HIGH);
  }

  else{
    digitalWrite(M1, LOW);
  }
    Serial.print(dist);
}
