// wifi.cpp

#include <WiFiS3.h>

#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

int status = WL_IDLE_STATUS;

String wifiIp() {
  IPAddress ip = WiFi.localIP();
  char buf[100];
  snprintf(buf, sizeof(buf), "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
  return String(buf);
}

String wifiStatus() {
  char buf[100];
  snprintf(buf, sizeof(buf), "http://%s (%s, %i dBm)", wifiIp().c_str(), WiFi.SSID(), WiFi.RSSI());
  return String(buf);
}

void wifiConnect() {
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  Serial.println("Connecting...");
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    for (int i = 0; i < 40; i++) {
      delay(250);
      if (status == WL_CONNECTED) {
        break;
      }
    }
  }
  delay(500);
  Serial.println("Connected!");
  Serial.println(wifiStatus());
}