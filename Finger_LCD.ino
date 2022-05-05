#include <Adafruit_Fingerprint.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 4);

SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

#define LOCK 4

void setup()
{
  lcd.init();
  lcd.backlight();
  finger.begin(57600);
  pinMode(LOCK, OUTPUT);
  digitalWrite(LOCK, LOW);
  delay(5);
  if (finger.verifyPassword()) {
    lcd.setCursor(0,0);
    lcd.print("FP Device Found ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    delay(2000);
  } else {
    lcd.setCursor(0,0);
    lcd.print("No FP Sensor    ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    delay(2000);
    while (1) { delay(1); }
  }

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
      lcd.setCursor(0,0);
      lcd.print("No FP data found");
      lcd.setCursor(0,1);
      lcd.print("Enroll FP first ");
      delay(2000);
  }
  else {
      lcd.setCursor(0,0);
      lcd.print("Sensor has ");
      lcd.print(finger.templateCount);
      lcd.print(" FP");
      lcd.setCursor(0,1);
      lcd.print("OK to proceed   ");
      delay(2000);
  }

}

void loop()                     // run over and over again
{
  lcd.setCursor(0,0);
  lcd.print("Place finger to     ");
  lcd.setCursor(0,1);
  lcd.print("start scan          ");
  getFingerprintID();
  delay(50);            //don't ned to run this at full speed.
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  if(p == FINGERPRINT_NOFINGER){
    return p;
  }
  else if(p != FINGERPRINT_OK){
    lcd.setCursor(0,0);
    lcd.print("Scan Error      ");
    lcd.setCursor(0,1);
    lcd.print("                "); 
    return p;  
  }
  
  p = finger.image2Tz();
  if(p != FINGERPRINT_OK){
    lcd.setCursor(0,0);
    lcd.print("Processing Error");
    lcd.setCursor(0,1);
    lcd.print("                "); 
    return p;
  }
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    lcd.setCursor(0,0);
    lcd.print("Access Granted  ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    digitalWrite(LOCK,HIGH);
    delay(5000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.setCursor(0,0);
    lcd.print("Comm Error      ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    delay(2000);
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    lcd.setCursor(0,0);
    lcd.print("Access Denied   ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    delay(2000);
    return p;
  } else {
    lcd.setCursor(0,0);
    lcd.print("Error in matching");
    lcd.setCursor(0,1);
    lcd.print("                ");
    delay(2000);
    return p;
  }
  digitalWrite(LOCK,LOW);
  return finger.fingerID;
}
