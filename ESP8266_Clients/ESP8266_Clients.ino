#include <ESP8266WiFi.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define PORT 23

const char *ssid = "Wifi";
const char *password = "Password";
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { 5, 4, 0, 2 };
byte colPins[COLS] = { 14, 12, 13, 15 };
byte degree[8] = {
  0B01110,
  0B01010,
  0B01110,
  0B00000,
  0B00000,
  0B00000,
  0B00000,
  0B00000
};
IPAddress server_ip(192, 168, 4, 1);
WiFiClient client;
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  lcd.init();
  lcd.createChar(1, degree);
  lcd.backlight();
  while (WiFi.status() != WL_CONNECTED) {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("Connecting");
    delay(500);
    lcd.setCursor(6, 1);
    lcd.print(".");
    delay(250);
    lcd.setCursor(7, 1);
    lcd.print(".");
    delay(250);
    lcd.setCursor(8, 1);
    lcd.print(".");
    delay(250);
    lcd.setCursor(9, 1);
    lcd.print(".");
    delay(250);
  }
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Connected!");
}

void loop() {
  char key = keypad.getKey();
  String req = String("");
  String res = String("");

  if (!client.connected()) {
    while (!client.connect(server_ip, PORT)) {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Disconnected");
      delay(250);
      return;
    }
  }

  if (key) {
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Press: " + String(key));
    req = String(key);
    client.write(req.c_str());
  } else {
    if (client.available()) {
      res = client.readStringUntil('\r');
      if (res.equals("W1")) {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("WARNING!");
        lcd.setCursor(2, 1);
        lcd.print("Obstructions");
        lcd.blink();
        delay(500);
      } else if (res.equals("W2")) {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("WARNING!");
        lcd.setCursor(0, 1);
        lcd.print("High temperature");
        lcd.blink();
        delay(500);
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Temperature: " + String(res.substring(res.indexOf('T') + 1, res.indexOf('T') + 3)));
        lcd.write(1);
        lcd.setCursor(0, 1);
        lcd.print("Humidity: " + String(res.substring(res.indexOf('H') + 1, res.indexOf('H') + 3)) + "%");
      }
    }
  }
}
