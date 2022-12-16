#include <ESP8266WiFi.h>

#define PORT 23
#define MAX_CLIENTS 1
#define RIGHT_MT_POS 5
#define RIGHT_MT_NEG 4
#define LEFT_MT_POS 14
#define LEFT_MT_NEG 12

// Ten va mat khau cua ESP8266 AP se tao
const char *ssid = "Group1";
const char *password = "12345678";
//khoi tao IP adress
IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
// Khoi tao port de clients ket noi.
WiFiServer server(PORT);
WiFiClient clients[MAX_CLIENTS];

void turnLeft() {
  digitalWrite(RIGHT_MT_POS, HIGH);
  digitalWrite(RIGHT_MT_NEG, LOW);
  digitalWrite(LEFT_MT_POS, LOW);
  digitalWrite(LEFT_MT_NEG, LOW);
}

void turnRight() {
  digitalWrite(RIGHT_MT_POS, LOW);
  digitalWrite(RIGHT_MT_NEG, LOW);
  digitalWrite(LEFT_MT_POS, HIGH);
  digitalWrite(LEFT_MT_NEG, LOW);
}

void goAhead() {
  digitalWrite(RIGHT_MT_POS, HIGH);
  digitalWrite(RIGHT_MT_NEG, LOW);
  digitalWrite(LEFT_MT_POS, HIGH);
  digitalWrite(LEFT_MT_NEG, LOW);
}

void goBack() {
  digitalWrite(RIGHT_MT_POS, LOW);
  digitalWrite(RIGHT_MT_NEG, HIGH);
  digitalWrite(LEFT_MT_POS, LOW);
  digitalWrite(LEFT_MT_NEG, HIGH);
}

void stop() {
  digitalWrite(RIGHT_MT_POS, LOW);
  digitalWrite(RIGHT_MT_NEG, LOW);
  digitalWrite(LEFT_MT_POS, LOW);
  digitalWrite(LEFT_MT_NEG, LOW);
}

void setup() {
  Serial.begin(115200);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid, password);
  server.begin();
  pinMode(RIGHT_MT_POS, OUTPUT);
  pinMode(RIGHT_MT_NEG, OUTPUT);
  pinMode(LEFT_MT_POS, OUTPUT);
  pinMode(LEFT_MT_NEG, OUTPUT);
}

void loop() {
  uint8_t i;
  String req = String("");
  String res = String("");

  if (server.hasClient()) {
    for (i = 0; i < MAX_CLIENTS; i++) {
      if (!clients[i].connected()) {
        if (clients[i]) {
          clients[i].stop();
        }
        clients[i] = server.available();
        Serial.println("New client: " + String(clients[i]));
        continue;
      }
    }
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }

  for (i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i].connected()) {
      if (Serial.available()) {
        res = Serial.readStringUntil('\r');
        clients[i].write(res.c_str());
      }

      if (clients[i].available()) {
        req = clients[i].readStringUntil('\r');
        if (req.equals("A")) {
          while (!req.equals("C")) {
            if (res.substring(res.indexOf('D') + 1, res.indexOf('T') - 1).toFloat() >= 20.0 && res.substring(res.indexOf('T') + 1, res.indexOf('H') - 1).toFloat() <= 60.0) {
              goAhead();
              delay(1500);
              stop();
            } else {
              goBack();
              delay(1500);
              turnRight();
              delay(1000);
              stop();
            }
            if (clients[i].available()) {
              req = clients[i].readStringUntil('\r');
            }
          }
        } else if (req.equals("2")) {
          if (res.substring(res.indexOf('D') + 1, res.indexOf('T') - 1).toFloat() >= 20.0 && res.substring(res.indexOf('T') + 1, res.indexOf('H') - 1).toFloat() <= 60.0) {
            goAhead();
            delay(1500);
            stop();
          } else if (res.substring(res.indexOf('D') + 1, res.indexOf('T') - 1).toFloat() < 20.0) {
            res = String("W1\r");
            clients[i].write(res.c_str());
          } else if (res.substring(res.indexOf('T') + 1, res.indexOf('H') - 1).toFloat() > 60.0) {
            res = String("W2\r");
            clients[i].write(res.c_str());
          } else {
            stop();
          }
        } else if (req.equals("8")) {
          if (res.substring(res.indexOf('D') + 1, res.indexOf('T') - 1).toFloat() >= 20.0 && res.substring(res.indexOf('T') + 1, res.indexOf('H') - 1).toFloat() <= 60.0) {
            goBack();
            delay(1500);
            stop();
          } else if (res.substring(res.indexOf('D') + 1, res.indexOf('T') - 1).toFloat() < 20.0) {
            res = String("W1\r");
            clients[i].write(res.c_str());
          } else if (res.substring(res.indexOf('T') + 1, res.indexOf('H') - 1).toFloat() > 60.0) {
            res = String("W2\r");
            clients[i].write(res.c_str());
          } else {
            stop();
          }
        } else if (req.equals("6")) {
          if (res.substring(res.indexOf('D') + 1, res.indexOf('T') - 1).toFloat() >= 20.0 && res.substring(res.indexOf('T') + 1, res.indexOf('H') - 1).toFloat() <= 60.0) {
            turnRight();
            delay(1000);
            stop();
          } else if (res.substring(res.indexOf('D') + 1, res.indexOf('T') - 1).toFloat() < 20.0) {
            res = String("W1\r");
            clients[i].write(res.c_str());
          } else if (res.substring(res.indexOf('T') + 1, res.indexOf('H') - 1).toFloat() > 60.0) {
            res = String("W2\r");
            clients[i].write(res.c_str());
          } else {
            stop();
          }
        } else if (req.equals("4")) {
          if (res.substring(res.indexOf('D') + 1, res.indexOf('T') - 1).toFloat() >= 20.0 && res.substring(res.indexOf('T') + 1, res.indexOf('H') - 1).toFloat() <= 60.0) {
            turnLeft();
            delay(1000);
            stop();
          } else if (res.substring(res.indexOf('D') + 1, res.indexOf('T') - 1).toFloat() < 20.0) {
            res = String("W1\r");
            clients[i].write(res.c_str());
          } else if (res.substring(res.indexOf('T') + 1, res.indexOf('H') - 1).toFloat() > 60.0) {
            res = String("W2\r");
            clients[i].write(res.c_str());
          } else {
            stop();
          }
        }
      }
    }
  }
}
