// wifi.cpp

#include "WiFiS3.h"

#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

int status = WL_IDLE_STATUS;

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
    for (int i = 0; i < 10; i++) {
      delay(1000);
      if (status == WL_CONNECTED) {
        break;
      }
    }
  }
  delay(3000);
  Serial.println("Connected!");
}

String wifiStatus() {
  IPAddress ip = WiFi.localIP();
  char buf[100];
  sprintf(buf, "http://%u.%u.%u.%u (%s, %i dBm)", ip[0], ip[1], ip[2], ip[3], WiFi.SSID(), WiFi.RSSI());
  return String(buf);
}
