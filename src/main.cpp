#include <Arduino.h>
#include <WiFiS3.h>

#include "wifi.h"
#include "web.h"

WiFiServer server(80);
WiFiClient client;

unsigned long statusTimer = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("HTTP-API started");
  wifiConnect();
  server.begin();
}

void loop() {
  if (millis() - statusTimer > 10000 || statusTimer > millis()) {
    statusTimer = millis();
    Serial.println(wifiStatus());
  }

  client = server.available();
  if (client) {
    handleRequest(&client);
    client.stop();
  }
}