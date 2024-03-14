// web.cpp

#include <Arduino.h>
#include <WiFiS3.h>

#include "wifi.h"

int ledPin = LED_BUILTIN;

String getRoute(String line) {
  unsigned int len = line.length();
  if (!line.startsWith("GET /") || len < 5) {
    return "";
  }
  String route;
  int i = 4;
  char c = line.charAt(i);
  while (c != '\n' && c != '\r' && c != ' ') {
    route += c;
    i++;
    if (i >= len) {
      break;
    }
    c = line.charAt(i);
  }
  return route;
}

String readLineFromRequest(WiFiClient *client) {
  String currentLine = "";
  while (client->connected() && client->available()) {
    char c = client->read();
    if (c == '\n') {
      return currentLine;
    } else if (c != '\r') {
      currentLine += c;
    }
  }
  return currentLine;
}

void handleRequest(WiFiClient *client) {
  String line;
  bool empty = false;
  while (true) {
    line = readLineFromRequest(client);

    if (line == "") {
      if (empty) {
        break;
      }
      empty = true;
      continue;
    } else {
      empty = false;
    }

    String route = getRoute(line);
    if (route == "/high") {
      Serial.println("! High !");
      digitalWrite(ledPin, HIGH);
    } else if (route == "/low") {
      Serial.println("! Low !");
      digitalWrite(ledPin, LOW);
    } else if (route != "") {
      Serial.println("page: " + route);
    }
  }

  client->println("HTTP/1.1 200 OK");
  client->println("Content-type:text/html");
  client->println();
  client->println("<link rel=\"icon\" type=\"image/gif\" href=\"data:image/gif;base64,iVBORw0KGgoAAAANSUhEUgAAADAAAAAwCAYAAABXAvmHAAAACXBIWXMAAAsTAAALEwEAmpwYAAACJklEQVR4nO2YzUuVQRTGf5mBC78iTfDaB5FB4KKFKwlXLsK1tHDhx/9QBEHQIkha6LJopdKmxDaVQlCuWrhwZSAouBA3QlJai0jyjYHnwvTyvnfeK9y5U8wDh3vPzBl9njtn5swMRERERERE/I0eYAE4BJJA7BBYBHopQH4/AMJJjhlupUoCFhT4xhXoGSXgrbi9rBRYTpuQyJdxQdy+UQHlqQoViYtfFFBjJHEG/qcZOAdMOopLBzCUspvA+YzYZmBMY7wIWFbsrv55FlZzCtJvVdZOK/aO+qZ9CLieImRmIgvb6n8BTMnM9x9qXwPOKPah2p77EPBYcc/0ueIQYNLGxhXgq/pGfQtoAHaAX1oHW8AxcLkKAYhoAjz1LWBIMUvyH8l/UKWAJ+qb8y1gXjET8vvkbwKnCgo4rfxPgHs+BTRrAf4E2q32zxo3kCPgtvLe2CDwWu3fgW6fAsbV/x44a9mUtaizBORdUoatWC8CPjguHGZnacoQ8BF4JZvVnt+V+ts1F3BRBejIImPbnpUuRRZxGjUXcD+1++QRMDenIAVsqM+cV7JwTf1HVnoEI6BNxcpc51oqjP+k8bfkr8u/UUDAXcXO1EIA2jH6HSQu6WjQKN8QH6EYWnWm6jwhv2IBdUYSBdQZSZyB0GfgQAHmjfSffJlbtKppSCJ6gHfiZt5vc2Gq6RfHga3er9NXXWpLegEup1MIdqBf3kk+IiIiIoJq8Ae8LKZ8gNjlgAAAAABJRU5ErkJggg==\">");
  client->println("Click <a href=\"/high\">here</a> turn the LED on<br>");
  client->println("Click <a href=\"/low\">here</a> turn the LED off<br><br>");
  client->println("Click <a href=\"/some\">here</a> to go to some page<br><br>");
  client->println("<hr>");
  client->println();
}