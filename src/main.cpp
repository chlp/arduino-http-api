#include <Arduino.h>
#include "WiFiS3.h"
#include "wifi.h"

int ledPin = LED_BUILTIN;

WiFiServer server(80);
WiFiClient client;

unsigned long statusTimer = 0;


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("Program started");
  wifiConnect();

  server.begin();
}

String readLine() {
  String currentLine = "";
  while (client.connected() && client.available()) {
    char c = client.read();
    if (c == '\n') {
      return currentLine;
    } else if (c != '\r') {
      currentLine += c;
    }
  }
  return currentLine;
}

String getPage(String line) {
  unsigned int len = line.length();
  if (!line.startsWith("GET /") || len < 5) {
    return "";
  }
  String page;
  int i = 4;
  char c = line.charAt(i);
  while (c != '\n' && c != '\r' && c != ' ') {
    page += c;
    i++;
    if (i >= len) {
      break;
    }
    c = line.charAt(i);
  }
  return page;
}

void printWEB() {
  String line;
  bool empty = false;
  while (true) {
    line = readLine();

    if (line == "") {
      if (empty) {
        break;
      }
      empty = true;
      continue;
    } else {
      empty = false;
    }

    String page = getPage(line);
    if (page == "high") {
      Serial.println("! High !");
      digitalWrite(ledPin, HIGH);
    } else if (page == "low") {
      Serial.println("! Low !");
      digitalWrite(ledPin, LOW);
    } else if (page != "") {
      Serial.println("page: " + page);
    }
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  client.print("Click <a href=\"/high\">here</a> turn the LED on<br>");
  client.print("Click <a href=\"/low\">here</a> turn the LED off<br><br>");
  client.print("Click <a href=\"/some\">here</a> to go to some page<br><br>");
  client.print("<hr>");
  client.println();
}

void loop() {
  if (millis() - statusTimer > 10000 || statusTimer > millis()) {
    statusTimer = millis();
    Serial.println(wifiStatus());
  }

  client = server.available();
  if (client) {
    printWEB();
    client.stop();
  }
}