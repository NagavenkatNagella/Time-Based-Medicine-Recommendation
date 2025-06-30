#include <Wire.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>

// WiFi Credentials
const char* ssid = "Sai";
const char* password = "Jessey143";

// I2C LCD (Address: 0x27 or 0x3F depending on module)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// NTP Client Setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000); // GMT+5:30

// Pin Definitions
#define MORNING_LED D5
#define AFTERNOON_LED D6
#define NIGHT_LED D7
#define BUZZER D8

// Predefined Medicine Times
int medicineTimes[3][2] = {
  {8, 0},   // 08:00 AM
  {13, 30}, // 01:30 PM
  {19, 0}   // 07:00 PM
};

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  
  pinMode(MORNING_LED, OUTPUT);
  pinMode(AFTERNOON_LED, OUTPUT);
  pinMode(NIGHT_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Wire.begin(D2, D1);  // SDA = D2, SCL = D1
  lcd.begin(16, 2);
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Medicine Recomm.");
  lcd.setCursor(5, 1);
  lcd.print("TEAM-06");
  delay(2000);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.print(".");
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");
  delay(2000);

  timeClient.begin();
}

void loop() {
  timeClient.update();
  int HOUR = timeClient.getHours();
  int MINUT = timeClient.getMinutes();
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time:");
  lcd.setCursor(6, 0);
  lcd.printf("%02d:%02d", HOUR, MINUT);
  lcd.setCursor(0, 1);
  lcd.print("Checking...");

  checkMedicineTime(HOUR, MINUT);
  delay(1000);
}

void checkMedicineTime(int hour, int minute) {
  for (int i = 0; i < 3; i++) {
    if (hour == medicineTimes[i][0] && minute == medicineTimes[i][1]) {
      triggerReminder(i);
    }
  }
}

void triggerReminder(int index) {
  int ledPin;
  String group;

  if (index == 0) {
    ledPin = MORNING_LED;
    group = "Morning";
  } else if (index == 1) {
    ledPin = AFTERNOON_LED;
    group = "Afternoon";
  } else {
    ledPin = NIGHT_LED;
    group = "NighT";
  }

  digitalWrite(ledPin, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Take ");
  lcd.print(group);
  lcd.setCursor(0, 1);
  lcd.print("Medicine Now");
  
  beep(30000); // Buzzer for 30 seconds

  delay(60000); // LED remains on for 1 minute
  digitalWrite(ledPin, LOW);
}

void beep(int duration) {
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);
    delay(500);
  }
}
