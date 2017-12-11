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

  //Serial.begin(9600);
    
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
    //Serial.print("loop:");
    //Serial.println(dataReceived);    
  }
  delay(100);
}

void receiveEvent(int data) {
  //int x = Wire.read();
  while(Wire.available()) {
    Wire.read();
  }
  dataReceived = 1;
  //Serial.print("receiveEvent:");
  //Serial.println(dataReceived);  
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

