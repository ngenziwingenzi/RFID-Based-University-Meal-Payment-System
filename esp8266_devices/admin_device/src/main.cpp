#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN D4
#define RST_PIN D3

MFRC522 rfid(SS_PIN, RST_PIN);
WiFiClient client;

const char* ssid = "SAM";
const char* password = "12345678";
const char* server_url = "http://192.168.137.61:8000/api/refill/";

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  delay(1000);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }

  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i], HEX);
  }

  Serial.println("Sending UID: " + uid);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(client, server_url);
    http.addHeader("Content-Type", "application/json");

    const int refillAmount = 2000;  // TEMPORARY refill amount (hardcoded for now)
    String payload = "{\"uid\":\"" + uid + "\", \"amount\":" + String(refillAmount) + "}";

    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Server response: " + response);
    } else {
      Serial.println("Error sending request");
    }

    http.end();
  }

  rfid.PICC_HaltA();
  delay(3000);  // Wait before next scan
}
