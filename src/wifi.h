// wifi.h

#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>

void wifiConnect();
String wifiStatus();
String readLineFromRequest(WiFiClient *client);

#endif