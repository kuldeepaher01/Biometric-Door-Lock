#include <Adafruit_Fingerprint.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 4);

SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
#define LOCK 4
#define LEDG 6
#define LEDB 5
#define LEDR 7
#define Buz 9
#define BGND 13
byte Smiley[8] = {
    B00000,
    B00000,
    B01010,
    B00000,
    B00000,
    B10001,
    B01110,
    B00000};
byte Lock[8] = {
    B01110,
    B10001,
    B10001,
    B11111,
    B11011,
    B11011,
    B11111,
    B00000};
byte Skull[8] = {
    B00000,
    B01110,
    B10101,
    B11011,
    B01110,
    B01110,
    B00000,
    B00000};
byte Check[8] = {
    B00000,
    B00001,
    B00011,
    B10110,
    B11100,
    B01000,
    B00000,
    B00000};
byte Unlock[8] = {
    B00110,
    B00001,
    B10001,
    B11111,
    B11011,
    B11011,
    B11111,
    B00000};
byte Cancel[8] = {
    B00000,
    B10001,
    B01010,
    B00100,
    B01010,
    B10001,
    B00000,
    B00000};

void setup()
{
  pinMode(BGND, OUTPUT);
  pinMode(LOCK, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(Buz, OUTPUT);
  digitalWrite(BGND, LOW);
  digitalWrite(LOCK, LOW);
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, Smiley);
  lcd.createChar(1, Lock);
  lcd.createChar(4, Unlock);
  lcd.createChar(3, Check);
  lcd.createChar(2, Skull);
  lcd.createChar(5, Cancel);
  lcd.home();
  finger.begin(57600);
  delay(1000);
  delay(5);
  if (finger.verifyPassword())
  {
    lcd.setCursor(0, 0);
    lcd.print("Finger Print");
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Device Found");
    lcd.setCursor(13, 1);
    lcd.write(3);
    lcd.setCursor(0, 3);
    lcd.print("Good To GO");
    lcd.setCursor(11, 3);
    lcd.write(0);
    delay(2000);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("No Finger Print");
    lcd.setCursor(0, 1);
    lcd.print("Sensor Found");
    lcd.setCursor(13, 1);
    lcd.write(2);
    lcd.setCursor(0, 2);
    lcd.print("Please check");
    lcd.setCursor(0, 3);
    lcd.print("Connections");
    digitalWrite(LEDR, LOW);
    delay(2000);
    digitalWrite(LEDR, HIGH);
    while (1)
    {
      delay(1);
    }
  }

  finger.getTemplateCount();

  if (finger.templateCount == 0)
  {
    lcd.setCursor(0, 0);
    lcd.print("No FP data found");
    lcd.setCursor(0, 1);
    lcd.print("Enroll FP first ");
    digitalWrite(LEDR, LOW);
    delay(2000);
    digitalWrite(LEDR, HIGH);
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("Sensor has ");
    lcd.print(finger.templateCount);
    lcd.print(" FP");
    lcd.setCursor(0, 1);
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, LOW);
    lcd.print("OK to proceed");
    lcd.setCursor(13, 1);
    lcd.write(3);
    delay(2000);
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
  }
  lcd.clear();
}

void loop() // run over and over again
{
  //  lcd.clear();
  lcd.setCursor(0, 0);
  digitalWrite(LEDR, HIGH); // Clearing Indicators of prev o/p
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDG, LOW);
  lcd.print("Place finger to     ");
  lcd.setCursor(0, 1);
  lcd.print("start scan");
  lcd.setCursor(11, 1);
  lcd.write(0);
  locked();
  getFingerprintID();
  delay(50);
  digitalWrite(LEDB, HIGH);
  // don't ned to run this at full speed.
}

uint8_t getFingerprintID()
{
  uint8_t p = finger.getImage();
  if (p == FINGERPRINT_NOFINGER)
  {
    return p;
  }
  else if (p != FINGERPRINT_OK)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Scan Error");
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
    digitalWrite(LEDR, LOW);
    lcd.setCursor(11, 0);
    lcd.write(2);
    lcd.setCursor(0, 1);
    lcd.print("Couldn't read FP");
    lcd.setCursor(0, 2);
    lcd.print("Please try again");
    locked();
    return p;
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Aww! Snap!!!");
    lcd.setCursor(0, 1);
    lcd.print("Couldn't process");
    lcd.setCursor(0, 2);
    lcd.print("Clean the sensor");
    lcd.setCursor(0, 3);
    lcd.print("& try again");
    lcd.setCursor(12, 3);
    lcd.write(1);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
    digitalWrite(LEDR, LOW);
    delay(1000);

    return p;
  }
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Granted!");
    lcd.setCursor(0, 1);
    lcd.print("Welcome! & have ");
    lcd.setCursor(0, 2);
    lcd.print("a nice day!");
    lcd.setCursor(12, 2);
    lcd.write(0);
    unlocked();
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDB, HIGH);
    digitalWrite(LEDG, LOW);
    beep(3);
    digitalWrite(LOCK, HIGH);
    beep(1);
    delay(4000);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
    lcd.clear();
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
    lcd.clear();
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Its not u, its");
    digitalWrite(LEDR, LOW);
    lcd.setCursor(0, 1);
    lcd.print("me");
    lcd.setCursor(2, 1);
    lcd.write(2);
    lcd.setCursor(0, 2);
    lcd.print("Please retry!");
    delay(2000);
    lcd.clear();
    return p;
  }
  else if (p == FINGERPRINT_NOTFOUND)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Access Denied");
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
    digitalWrite(Buz, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Seems like you");
    lcd.setCursor(0, 2);
    lcd.print("are lost!!");
    lcd.setCursor(0, 2);
    lcd.write(2);
    locked();
    delay(2000);
    lcd.clear();
    digitalWrite(Buz, LOW);

    return p;
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error in matching");
    lcd.setCursor(0, 1);
    lcd.print("Try again in");
    lcd.setCursor(0, 2);
    lcd.print("some time.");
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, HIGH);
    digitalWrite(LEDR, LOW);
    digitalWrite(Buz, HIGH);
    lcd.setCursor(0, 3);
    locked();

    delay(2000);
    lcd.clear();
    digitalWrite(Buz, LOW);
    return p;
  }
  digitalWrite(LOCK, LOW);
  locked();
  lcd.clear();
  return finger.fingerID;
}
void locked()
{
  lcd.setCursor(0, 3);
  lcd.print("Status-");
  lcd.setCursor(8, 3);
  lcd.write(1);
  lcd.setCursor(10, 3);
}
void unlocked()
{
  lcd.setCursor(0, 3);
  lcd.print("Status-");
  lcd.setCursor(8, 3);
  lcd.write(4);
}
void beep(int i)
{
  while (i != 0)
  {
    digitalWrite(Buz, HIGH);
    digitalWrite(LEDB, LOW);
    digitalWrite(LEDG, HIGH);
    delay(50);
    digitalWrite(LEDB, HIGH);
    digitalWrite(LEDG, LOW);
    digitalWrite(Buz, LOW);
    delay(50);
    i--;
  }
}
