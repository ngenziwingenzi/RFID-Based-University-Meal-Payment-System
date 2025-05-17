#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "Gloria";
const char* password = "12345678";
const char* server_url = "http://192.168.43.52:8000/api/scan/";

String uid = ""; // Simulate UID from RFID for now

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi.");
}

void loop() {
  if (Serial.available()) {
    uid = Serial.readStringUntil('\n');
    uid.trim();
    
    if (uid.length() > 0) {
      Serial.println("Sending UID to server: " + uid);

      if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverUrl);
        http.addHeader("Content-Type", "application/json");

        String jsonData = "{\"uid\":\"" + uid + "\"}";
        int httpResponseCode = http.POST(jsonData);

        if (httpResponseCode > 0) {
          String response = http.getString();
          Serial.println("Server response: " + response);
        } else {
          Serial.println("Error sending POST: " + String(httpResponseCode));
        }

        http.end();
      } else {
        Serial.println("WiFi not connected");
      }
    }
  }

  delay(1000);
}
