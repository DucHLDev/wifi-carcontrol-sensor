#include <DHT.h>

#define DHTPIN 8
#define DHTTYPE DHT11

const int trig = 9;
const int echo = 10;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  dht.begin();
}

void loop() {
  String res = String("");
  // Distance
  long duration;
  float distance;
  digitalWrite(trig, 0);
  delayMicroseconds(2);
  digitalWrite(trig, 1);
  delayMicroseconds(5);
  digitalWrite(trig, 0);
  duration = pulseIn(echo, HIGH);
  distance = float(duration * 0.017);
  // DHT11
  float temp = dht.readTemperature();
  float humi = dht.readHumidity();

  if (isnan(temp) && isnan(humi)) {
    res = "D" + String(distance) + "T" + String(temp) + "H" + String(humi);
    Serial.print(res.c_str());
  }
}