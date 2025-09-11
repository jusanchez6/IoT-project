#include <Arduino.h>

#include <Wire.h>

#include "DFRobot_GNSSAndRTC.h"

#include <wifi_funcs.hpp>
#include <imu.hpp>


#define D2 2
#define D3 3

DFRobot_GNSSAndRTC_UART gnss(&Serial1, UART_BAUDRATE,/*rx*/D2,/*tx*/D3);


// ===== WiFi =====
String ssid;
String password;

// ===== IMU =====
IMU imu;


// ===== GNSS =====

void callback(char* data, uint8_t len) 
{
  for (uint8_t i = 0; i < len; i++) {
    Serial.print((char)data[i]);
  }
  delay(1);
}



void setup() {
  Serial.begin(115200);
  Wire.begin(45, 35);  // Pines ESP32-S3 SDA=45, SCL=35

  delay(2000);

  while (!gnss.begin()) 
  {
    Serial.println("NO DEVICES !");
    delay(1000);
  }
  
  gnss.enablePower();

  gnss.setGnss(gnss.eGPS_BeiDou_GLONASS);

  gnss.setCallback(callback);

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

void loop() {

  // imu.update();

  // Serial.print("AccX:");
  // Serial.print(imu.getAccX());
  // Serial.print(", Vel (Int):");
  // Serial.print(imu.getVelMeasured());
  // Serial.print(", Vel (Kalman):");
  // Serial.println(imu.getVelKalman());
  // delay(50);

  Serial.println("\n---------------------------Raw data from L76K-------------------------------");
  gnss.getAllGnss();
  delay(3000);

  
}
