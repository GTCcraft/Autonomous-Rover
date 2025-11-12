//Motor 1 - back left wheel
const int enA_BL = 2;
const int in1_BL = 22;
const int in2_BL = 24;

//Motor 2 - front left wheel
const int enB_FL = 3;
const int in3_FL = 26;
const int in4_FL = 28;

//Motor 3 - back right wheel
const int enB_BR = 13;
const int in3_BR = 32;
const int in4_BR = 30;

//Motor 4 - front right wheel
const int enA_FR = 12;
const int in1_FR = 36;
const int in2_FR = 34;

//Ultrasonic sensors
const int echo_back = 8;
const int trig_back = 9;

const int echo_left = 6;
const int trig_left = 7;

const int echo_right = 4;
const int trig_right = 52;

const int echo_front = 10; 
const int trig_front = 11;

struct Sensor {
  int echo;
  int trig;
};

struct Sensor backSensor = {echo_back, trig_back}; 
struct Sensor leftSensor = {echo_left, trig_left};
struct Sensor rightSensor = {echo_right, trig_right};
struct Sensor frontSensor = {echo_front, trig_front};

Sensor sensors[] = {backSensor, leftSensor, rightSensor, frontSensor};

const float SPEED_OF_SOUND_CM_PER_MICROSEC = 0.0343;

//Wheels
struct Wheel { 
  bool isFront;
  int en;
  int first;
  int second; 
};

struct Wheel backLeft = { false, enA_BL, in1_BL, in2_BL};
struct Wheel backRight = { false, enB_BR, in3_BR, in4_BR};
struct Wheel frontLeft = { true, enB_FL, in3_FL, in4_FL};
struct Wheel frontRight = { true, enA_FR, in1_FR, in2_FR};

Wheel wheels[4] = {backLeft, backRight, frontLeft, frontRight};

//Speed settings
const int PRETTY_FAST_SPEED = 120;
const int MAX_SPEED = 255;

void setup() {
  Serial.begin(9600);

  //Init sensor pins
  for(Sensor sensor : sensors) {
    pinMode(sensor.trig, OUTPUT);
    pinMode(sensor.echo, INPUT);
  }

  //Init wheel pins
  for(Wheel wheel : wheels){
    pinMode(wheel.en, OUTPUT);
    pinMode(wheel.first, OUTPUT);
    pinMode(wheel.second, OUTPUT);
  }
}

void loop() {
  float backDistance = getDistance(backSensor);
  float leftDistance = getDistance(leftSensor);
  float rightDistance = getDistance(rightSensor); 
  float frontDistance = getDistance(frontSensor); 

  startMoveForward();
  while(frontDistance < 20) {
    startMoveLeft();
    delay(100);
    leftDistance = getDistance(leftSensor);
    if(leftDistance < 20){ //Hit left wall? 
      startMoveRight();
      while(frontDistance < 20)
        delay(100);
    }
  }
  delay(300);
}

// < -------------- SUPPORT FUNCTIONS ----------------->
void sendTrigPulse(int trigPin){
  digitalWrite(trigPin, LOW);  
	delayMicroseconds(2);  
	digitalWrite(trigPin, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(trigPin, LOW);  
}

float getDistance(Sensor sensor){
  float distance, echoTime;
  sendTrigPulse(sensor.trig);
  echoTime = pulseIn(sensor.echo, HIGH);
  distance = (echoTime * SPEED_OF_SOUND_CM_PER_MICROSEC)/2;
  return distance;
}

void setWheelForward(Wheel wheel){
  if(wheel.isFront){
    digitalWrite(wheel.first, LOW);
    digitalWrite(wheel.second, HIGH);
  }
  else{
    digitalWrite(wheel.second, LOW);
    digitalWrite(wheel.first, HIGH);
  } 
}

void setWheelBackward(Wheel wheel){
  if(wheel.isFront){
    digitalWrite(wheel.first, HIGH);
    digitalWrite(wheel.second, LOW);
  }
  else{
    digitalWrite(wheel.second, HIGH);
    digitalWrite(wheel.first, LOW);
  } 
}

void setDecentSpeed(){
  for(Wheel wheel : wheels) 
    analogWrite(wheel.en, PRETTY_FAST_SPEED);
}

void startMoveForward() {
  for(Wheel wheel : wheels)
    setWheelForward(wheel);
  setDecentSpeed();
}

void startMoveBack() {
  for (Wheel wheel : wheels)
    setWheelBackward(wheel);
  setDecentSpeed();
}

void startMoveRight() {
  setWheelForward(frontLeft);
  setWheelForward(backRight);
  setWheelBackward(frontRight);
  setWheelBackward(backLeft);
  setDecentSpeed();
}

void startMoveLeft() {
  setWheelBackward(frontLeft);
  setWheelBackward(backRight);
  setWheelForward(frontRight);
  setWheelForward(backLeft);
  setDecentSpeed();
}