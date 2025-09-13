#include <Arduino.h>
#include <Wire.h>


#include <gnss.hpp>
#include <wifi_funcs.hpp>
#include <imu.hpp>
#include <platform.hpp>

// ======== GNSS =============

#define RX_PIN 17
#define TX_PIN 18
#define GNSS_BAUD 9600

GNSS gnss(Serial1, RX_PIN, TX_PIN, GNSS_BAUD);

// ===== WiFi =====
String ssid;
String password;

// ===== IMU =====
IMU imu;


// ==== LED =====

LED led(1, 48);
void setup()
{
  Serial.begin(115200);
  Wire.begin(45, 35); // Pines ESP32-S3 SDA=45, SCL=35

  gnss.begin();

  Serial.println("Iniciando GPS test...");
  delay(2000);

  // ============= Configuracion WiFi =================================================

  // ask_credentials(ssid, password);

  // if (!connect_to_wifi(ssid, password)) {
  //   Serial.println("Error. Se necesita conexión a internet para iniciar la aplicación.");
  //   while (true);     // Se queda aqui toda la vida si no se conecta a wifi
  // }

  // // ===== IMU =====
  Serial.println("Configuración MPU6050..");

  if (!imu.begin(&Wire, 0x68)) {
    Serial.println("Error. No se pudo encontrar un MPU6050 valido.");
    while(true);
  }
  Serial.println("MPU6050 OK");
}

void loop()
{

  imu.update();

  Serial.print("AccX:");
  Serial.print(imu.getAccX());
  Serial.print(", Vel (Int):");
  Serial.print(imu.getVelMeasured());
  Serial.print(", Vel (Kalman):");
  Serial.println(imu.getVelKalman());


  gnss.update();

  if (gnss.isReady()) {
    Serial.print("Lat: "); Serial.println(gnss.latitude(), 6);
    Serial.print("Lng: "); Serial.println(gnss.longitude(), 6);
    Serial.print("Alt: "); Serial.println(gnss.altitude());
    Serial.print("Hora local: "); Serial.println(gnss.localTime(-5));
    Serial.print("Velocidad [km/h]: "); Serial.println(gnss.velocity());
    Serial.println("----------------------------");
  }

  led.setColor(COLOR_PINK);

  delay(200);
  
}