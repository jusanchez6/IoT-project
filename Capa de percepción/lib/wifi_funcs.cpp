#include "wifi_funcs.h"

void ask_wifi_credentials(String &ssid, String &password) {
  Serial.println("\nConfiguración WiFi:");
  Serial.print("Ingrese SSID: ");
  while (ssid.length() == 0) {
    if (Serial.available()) {
      ssid = Serial.readStringUntil('\n'); ssid.trim();
    }
  }
  Serial.print("\nIngrese Contraseña: ");
  while (password.length() == 0) {
    if (Serial.available()) {
      password = Serial.readStringUntil('\n'); password.trim();
    }
  }
}

void connect_to_wifi(const String &ssid, const String &password) {
  Serial.println("\nConectando a WiFi...");
  WiFi.begin(ssid.c_str(), password.c_str());
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 20) {
    delay(500); Serial.print("."); timeout++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado a WiFi!");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nError al conectar a WiFi.");
  }
}

