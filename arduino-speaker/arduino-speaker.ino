#include <Wire.h>

int i2cAddress = 1;
static volatile int dataReceived = 0;

unsigned int speakerPin = 2;

void setup() {
  Wire.begin(i2cAddress);
  Wire.onReceive(receiveEvent);

  pinMode(speakerPin, OUTPUT);

  //Serial.begin(9600);
   
}

void loop() {
  if (dataReceived) {
    noInterrupts();
    dataReceived = 0;
    interrupts();
    playMelody();
    //Serial.print("loop:");
    //Serial.println(dataReceived);    
  }
  delay(100);
}

void receiveEvent(int data) {
  while(Wire.available()) {
    Wire.read();
  }
  dataReceived = 1;
  //Serial.print("receiveEvent:");
  //Serial.println(dataReceived);  
}

void playMelody() {
  digitalWrite(speakerPin, HIGH);
  delay(100);
  digitalWrite(speakerPin, LOW);
}

