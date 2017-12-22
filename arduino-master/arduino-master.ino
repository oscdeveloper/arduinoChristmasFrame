#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
SoftwareSerial GSM(7, 8); // RX, TX

LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int i2cDeviceAddress[] = {4,1,2}; // 0 - master uno,  1 - speaker, 2 - servo
// address number 4 is master UNO, zero is only for array index number

char phone_no[]="YOUR_PHONE_NUMBER";

enum _parseState {
  PS_DETECT_MSG_TYPE,
  PS_IGNORING_COMMAND_ECHO,
  PS_READ_CMTI_STORAGE_TYPE,
  PS_READ_CMTI_ID,
  PS_READ_CMGR_STATUS,
  PS_READ_CMGR_NUMBER,
  PS_READ_CMGR_SOMETHING,
  PS_READ_CMGR_DATE,
  PS_READ_CMGR_CONTENT
};

byte state = PS_DETECT_MSG_TYPE;

boolean alarmStatus = false;

char inData[300];
char inDataLcd[300];
byte bufferIndex = 0;
char inChar;

int lastReceivedSMSId = 0;
boolean validSender = false;

String inDataString;


void setup() {

  lcdShowWelcomeText();

  GSM.begin(9600);
  Serial.begin(9600);

  // synchronize A6 module with Arduino 9600 boudrate
  for( int i=0; i<=10; i++) {
    delay(500);
    GSM.println("AT");  
  }
  delay(5000);
  
  GSM.println("AT+CMGF=1"); // select sms text mode
  delay(200);

  GSM.println("AT+CMGD=1,4"); // delete all sms from memory
  delay(200);

  sendSMS("Christmas frame powered on");  
  delay(5000);
  
  // Not really necessary but prevents the serial monitor from dropping any input
  while(GSM.available()) {
    Serial.write(GSM.read());
  }  
}

void loop() {

  while( GSM.available() > 0 ) {
    
    inChar = GSM.read();
    inData[bufferIndex] = inChar;
    bufferIndex++;
    inData[bufferIndex] = '\0';

    if ( inChar == '\n' ) {
      //Serial.println(inData);
      if ( strstr(inData, "+C") ) {
        //Serial.print("resetBuffer:");
        //Serial.println(inData);
        resetBuffer();
      } 

      if ( strlen(inData) > 2 ) {
        //Serial.print(strlen(inData));
        //Serial.print("Data:");
        //Serial.println(inData);
        //inDataString = String(inData);
        //if( inDataString.indexOf("OK") == -1 ) {
          lcd.clear();        
          lcd.setCursor(8,0);
          lcd.print("SMS");
          lcd.setCursor(0,1);
          memcpy(inDataLcd, inData, strlen(inData) - 2);
          lcd.print(inDataLcd);
          resetBuffer();
          
          for ( int i=0; i<=5; i++ ) {
            lcd.noBacklight();
            delay(100);
            lcd.backlight();
            delay(100);
          }        
          
          i2cSendData(2, 1); // arduino servos
          i2cSendData(1, 1); // arduino speaker          
        //}
      }
    }
  }
}

void i2cSendData(int deviceAddress, int data) {
  Wire.beginTransmission(i2cDeviceAddress[deviceAddress]);
  Wire.write(data);
  Wire.endTransmission();
  //Serial.print("i2cSendData:");
  //Serial.println(i2cDeviceAddress[deviceAddress]);
}

void sendSMS(String msg) {
  GSM.print("AT+CMGS=\"");
  GSM.print(phone_no); 
  GSM.write(0x22);  // double quotes "
  GSM.write(0x0D);  // hex equivalent of Carraige return    
  GSM.write(0x0A);  // hex equivalent of newline
  delay(1000);
  GSM.print(msg);
  delay(500);
  GSM.println (char(26));//the ASCII code of the ctrl+z is 26  
}

void resetBuffer() {
  memset(inDataLcd, 0, sizeof(inDataLcd));
  memset(inData, 0, sizeof(inData));
  bufferIndex = 0;
}

void lcdShowWelcomeText() {
  lcd.init();  
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Merry  Christmas");
  delay(2000);
  lcd.setCursor(3,1);
  lcd.print("Motherfuckers!");
  lcd.setCursor(4,2);
  lcd.print("OH!  OH!  OH!");
  delay(2000);
}

