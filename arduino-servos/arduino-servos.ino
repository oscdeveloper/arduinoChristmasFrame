#include <Wire.h>
#include <Servo.h>

int i2cAddress = 2;
static volatile int dataReceived = 0;

Servo servoReindeer;
Servo servoSantaClaus;

unsigned int servoReindeerPin = 6;
unsigned int servoSantaClausPin = 5;

unsigned int servoReindeerAngle = 90;
unsigned int servoSantaClausAngle = 90;

int servoAngleShakeMax = 100;
int servoAngleShakeMin = 80;

int servoAngleAnimationMax = 120;
int servoAngleAnimationMin = 60;

void setup() {
  Wire.begin(i2cAddress);
  Wire.onReceive(receiveEvent);
    
  servoReindeer.attach(servoReindeerPin);
  servoSantaClaus.attach(servoSantaClausPin);
  servoReindeer.write(servoReindeerAngle);
  servoSantaClaus.write(servoSantaClausAngle);  
  shakeRotateServo();
}

void loop() {
  if (dataReceived) {
    noInterrupts();
    dataReceived = 0;
    interrupts();
    rotateServo();
  }
}

void receiveEvent() {
  //int x = Wire.read();
  dataReceived = 1;
}

void shakeRotateServo() {
  for ( int i=0; i<=7; i++ ) {
    servoReindeer.write(servoAngleShakeMax);
    servoSantaClaus.write(servoAngleShakeMax);
    delay(100);  
    servoReindeer.write(servoAngleShakeMin);
    servoSantaClaus.write(servoAngleShakeMin);  
    delay(100);    
  }
}

void rotateServo() {

  shakeRotateServo();

  for ( int i=0; i<=3; i++ ) {
    
    for ( int angle = servoAngleAnimationMin; angle <= servoAngleAnimationMax; angle++ ) {
      servoReindeer.write(angle);
      servoSantaClaus.write(angle);
      delay(10);     
    }
  
    for ( int angle = servoAngleAnimationMax; angle >= servoAngleAnimationMin; angle-- ) {
      servoReindeer.write(angle);
      servoSantaClaus.write(angle);
      delay(10);     
    }  
  }

  for ( int angle = servoAngleAnimationMin; angle <= servoReindeerAngle; angle++ ) {
    servoReindeer.write(angle);
    servoSantaClaus.write(angle);
    delay(10);     
  }   
}

