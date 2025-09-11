#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


#include <wifi_funcs.hpp>
#include <imu.hpp>



// ===== WiFi =====
String ssid;
String password;

// ===== IMU =====
IMU imu;


void setup() {
  Serial.begin(115200);
  Wire.begin(45, 35);  // Pines ESP32-S3 SDA=45, SCL=35

  delay(2000);

  // ============= Configuracion WiFi =================================================

  ask_credentials(ssid, password);

  if (!connect_to_wifi(ssid, password)) {
    Serial.println("Error. Se necesita conexión a internet para iniciar la aplicación.");
    while (true);     // Se queda aqui toda la vida si no se conecta a wifi
  }



  // ===== IMU =====
  Serial.println("Configuración MPU6050..");

  if (!imu.begin(&Wire, 0x68)) {
    Serial.println("Error. No se pudo encontrar un MPU6050 valido.");
    while (true);
  }
  Serial.println("MPU6050 OK");
}

void loop() {

  imu.update();

  Serial.print("AccX:");
  Serial.print(imu.getAccX());
  Serial.print(", Vel (Int):");
  Serial.print(imu.getVelMeasured());
  Serial.print(", Vel (Kalman):");
  Serial.println(imu.getVelKalman());

  delay(50);
}
