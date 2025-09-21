/**
 * @file imu.hpp
 * @brief Implementación de la clase IMU para adquisición y procesamiento de datos del sensor MPU6050.
 *
 * Esta librería proporciona una interfaz para inicializar, configurar y procesar datos de un
 * acelerómetro MPU6050 usando la librería Adafruit. Incluye filtrado de datos, estimación de
 * velocidad con integración de la aceleración y filtrado mediante Kalman y media móvil.
 * 
 * @author Julian Sanchez
 * @date 12/09/2025
 * @version 2.0
 * 
 */


#include <imu.hpp>

IMU::IMU() : lastTime(0), hpX(0), hpY(0), hpZ(0),
               accX_prev(0), accY_prev(0), accZ_prev(0),
               vibrationRMS(0.0f), vib_start_time(0),
               vib_sumSq(0.0f), vib_count(0) {}


bool IMU::begin(TwoWire *wire, uint8_t addr) {
  if (!mpu.begin(addr, wire)) {
    return false;
  }
  config();
  lastTime = millis();
  return true;
}

void IMU::config() {
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(10);
}



void IMU::update() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float accX = a.acceleration.x;
  float accY = a.acceleration.y;
  float accZ = a.acceleration.z;

  hpX = alpha * (hpX + accX - accX_prev);
  hpY = alpha * (hpY + accY - accY_prev);
  hpZ = alpha * (hpZ + accZ - accZ_prev);

  accX_prev = accX;
  accY_prev = accY;
  accZ_prev = accZ;

  // Calculo de la magnitud instantanea
  float vib_instant = sqrt(hpX * hpX + hpY * hpY + hpZ * hpZ);
  vib_sumSq += vib_instant * vib_instant;
  vib_count++;

  if (millis() - vib_start_time >= SAMPLE_WINDOW) {
    vibrationRMS = sqrt(vib_sumSq / vib_count);
    vib_sumSq = 0.0f;
    vib_count = 0;
    vib_start_time = millis();
  }
  
}