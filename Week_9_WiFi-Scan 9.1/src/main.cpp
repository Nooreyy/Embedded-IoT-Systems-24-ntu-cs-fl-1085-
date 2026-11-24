#include <Arduino.h>
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA); // station mode
  WiFi.disconnect(); // if connected to a network, disconnect
  delay(100);
  Serial.println("WiFi scanner ready");
}

void loop() {
  Serial.println("Scanning...");
  int n = WiFi.scanNetworks(); // scan if any networks are available

  if (n == 0) {
    Serial.println("No networks found");
  } else {
    Serial.printf("%d networks found\n", n);
    for (int i = 0; i < n; i++) {
      Serial.printf("%2d: %-32s  RSSI: %d dBm  Ch: %2d  %s\n",
                    i + 1,
                    WiFi.SSID(i).c_str(),
                    WiFi.RSSI(i),
                    WiFi.channel(i),
                    WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? "Open" : "Secured");
    }
  }

  WiFi.scanDelete();  // free scan results memory
  delay(5000);        // wait before next scan
}