#include <Arduino.h>
#include <Wire.h>


#include <gnss.hpp>
#include <wifi_funcs.hpp>
#include <imu.hpp>

// ======== GNSS =============

#define RX_PIN 17
#define TX_PIN 18
#define GNSS_BAUD 9600
TinyGPSPlus gps;
unsigned long lastPrint = 0;

// ===== WiFi =====
String ssid;
String password;

// ===== IMU =====
IMU imu;

void setup()
{
  Serial.begin(115200);
  Wire.begin(45, 35); // Pines ESP32-S3 SDA=45, SCL=35

  Serial1.begin(GNSS_BAUD, SERIAL_8N1, RX_PIN, TX_PIN);

  Serial.println("Iniciando GPS test...");
  delay(2000);

  // ============= Configuracion WiFi =================================================

  // ask_credentials(ssid, password);

  // if (!connect_to_wifi(ssid, password)) {
  //   Serial.println("Error. Se necesita conexión a internet para iniciar la aplicación.");
  //   while (true);     // Se queda aqui toda la vida si no se conecta a wifi
  // }

  // // ===== IMU =====
  // Serial.println("Configuración MPU6050..");

  // if (!imu.begin(&Wire, 0x68)) {
  //   Serial.println("Error. No se pudo encontrar un MPU6050 valido.");
  //   while (true);
  // }
  // Serial.println("MPU6050 OK");
}

void loop()
{

  // imu.update();

  // Serial.print("AccX:");
  // Serial.print(imu.getAccX());
  // Serial.print(", Vel (Int):");
  // Serial.print(imu.getVelMeasured());
  // Serial.print(", Vel (Kalman):");
  // Serial.println(imu.getVelKalman());
  // delay(50);

  while (Serial1.available())
  {
    char c = Serial1.read();
    gps.encode(c);
  }

  if (gps.location.isUpdated()) {
  unsigned long now = millis();
  if (now - lastPrint >= 200) {  // 5 Hz (200 ms)
    lastPrint = now;

    Serial.print("Lat: "); Serial.println(gps.location.lat(), 6);
    Serial.print("Lng: "); Serial.println(gps.location.lng(), 6);
    Serial.print("Alt: "); Serial.println(gps.altitude.meters());

    if (gps.time.isValid()) {
      int h = gps.time.hour();
      int m = gps.time.minute();
      int s = gps.time.second();

      // Ajuste a hora local Colombia (UTC−5)
      h = h - 5;
      if (h < 0) {
        h += 24;   // retrocede al día anterior
      }

      char timeBuffer[20];
      sprintf(timeBuffer, "%02d:%02d:%02d", h, m, s);
      Serial.print("Hora local (COL): ");
      Serial.println(timeBuffer);
    } else {
      Serial.println("Hora no válida");
    }

    Serial.println();
  }
}
}