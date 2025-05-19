#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN D3  // Adjust these pins according to your wiring
#define SS_PIN  D4  // SDA pin of RC522

const char* ssid = "SAM";
const char* password = "12345678";
const char* server_url = "http://192.168.137.61:8000/api/scan/";

MFRC522 rfid(SS_PIN, RST_PIN);
WiFiClient client;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi.");
  Serial.println("Waiting for RFID card...");
}

void loop() {
  // Look for new cards
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    delay(500);
    return;
  }

  // Read UID
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toLowerCase();

  Serial.println("Card detected: UID = " + uid);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(client, server_url);
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

  rfid.PICC_HaltA();  // Halt PICC
  rfid.PCD_StopCrypto1();  // Stop encryption
  delay(2000); // Prevent repeated reads
}
