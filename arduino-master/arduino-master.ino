#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
SoftwareSerial GSM(7, 8); // RX, TX

LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int i2cDeviceAddress[] = {4,1,2}; // 0 - master uno,  1 - sound, 2 - servo

char phone_no[]="+48508513055";

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
byte pos = 0; // TODO remove
char inChar;

int lastReceivedSMSId = 0;
boolean validSender = false;


void setup() {
  //Wire.begin(i2cDeviceAddress[0]);
  
  GSM.begin(9600);
  Serial.begin(9600);
  delay(300); 
  pinMode(13, OUTPUT); // test builtin led

  GSM.println("AT+CMGF=1"); // select sms text mode
  delay(2000);

  GSM.println("AT+CMGD=1,4"); // delete all sms from memory
  delay(2000);

  //delay(10000); // 10 sec
  //sendSMS("Christmas frame powered on");  

  // Not really necessary but prevents the serial monitor from dropping any input
  while(GSM.available()) {
    Serial.write(GSM.read());
  }  

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

void loop() {
  
  while( GSM.available() > 0 ) {
    
    inChar = GSM.read();
    inData[bufferIndex] = inChar;
    bufferIndex++;
    inData[bufferIndex] = '\0';

    if ( inChar == '\n' ) {
      //Serial.println(inData);
      if ( strstr(inData, "+C") ) {
        Serial.print("resetBuffer:");
        Serial.println(inData);
        resetBuffer();
      } 

      if ( strlen(inData) > 2 ) {
        Serial.print(strlen(inData));
        Serial.print("Data:");
        Serial.println(inData);
        lcd.clear();
        lcd.setCursor(8,0);
        lcd.print("SMS");
        lcd.setCursor(0,1);
        memcpy(inDataLcd, inData, strlen(inData) - 2);
        lcd.print(inDataLcd);
        resetBuffer();
        //i2cSendData(2, 1); // arduino servos
      }
    }
  }
}

void i2cSendData(int deviceAddress, int data) {
  Wire.beginTransmission(i2cDeviceAddress[deviceAddress]);
  Wire.write(data);
  Wire.endTransmission(true);
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

