#include <Arduino.h>
#include <WiFiS3.h>

#include "wifi.h"
#include "web.h"

#include <ArduinoGraphics.h>
#include <Arduino_LED_Matrix.h>

ArduinoLEDMatrix matrix;

String textToDrawOnMatrix = "";

void drawTextOnEmbeddedMatrix() {
  matrix.clear();
  if (textToDrawOnMatrix == "") {
    return;
  }
  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(80);
  matrix.textFont(Font_4x6);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(textToDrawOnMatrix.substring(textToDrawOnMatrix.length()-3));
  matrix.endText();
}

WiFiServer server(80);
WiFiClient client;

unsigned long statusTimer = 0;

void setup() {
  matrix.begin();
  matrix.beginDraw();

  Serial.begin(9600);
  while (!Serial) {
    delay(250);
    Serial.println("!Serial, wait");
  }
  delay(250);
  
  Serial.println("HTTP-API started");
  wifiConnect();
  server.begin();
}

void loop() {
  if (millis() - statusTimer > 10000 || statusTimer > millis()) {
    statusTimer = millis();
    Serial.println(wifiStatus());
    textToDrawOnMatrix = wifiIp();
    drawTextOnEmbeddedMatrix();
  }

  client = server.available();
  if (client) {
    handleRequest(&client);
    client.stop();
  }
}