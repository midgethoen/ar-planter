#include <ArduinoJson.h>
#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(3, OUTPUT);
}

void loop(){}

void serialEvent() {
  char receivedChar = processSerialInput();
  if (receivedChar == -1){
    return;
  }
  if (receivedChar == '1'){
    turnOnRelay();
  } else if (receivedChar == '0'){
    turnOffRelay();
  }

  sendSensorData(receivedChar);
}

char processSerialInput(){
  // put your main code here, to run repeatedly:
  if (Serial.available() < 1){
    return -1;
  }
  char receivedChar = Serial.read();
  // discard all other chars (\n)
  while (Serial.available()) {Serial.read();}
  if (receivedChar == '\n'){
    return -1;
  }
  return receivedChar;
};

void sendSensorData(char receivedChar){
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["humidity"] = getHumidity();
  root["temperature"] = getTemperature();
  root.set("moisture_1", getMoisture(0), 6);
  root.set("moisture_2", getMoisture(1), 6);
  root.set("char", (String) receivedChar);

  root.printTo(Serial);
  Serial.println();
}

float getTemperature(){
  return dht.readTemperature();
}

float getHumidity(){
  return dht.readHumidity();
}

float getMoisture(int pin){
  return analogRead(pin);
}


void turnOnRelay(){
  digitalWrite(3, LOW);
}
void turnOffRelay(){
  digitalWrite(3, HIGH);
}
