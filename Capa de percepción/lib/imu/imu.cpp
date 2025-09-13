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
 * @version 1.0
 * 
 */


#include <imu.hpp>

IMU::IMU() : lastTime(0), P(1.0f), Q(0.01f), R(0.8f), K(0.0f),
             acc_index(0), buffer_full(false), accX(0), vel_measured(0), vel_kalman(0) {
  imu_data = {0.0f, 0.0f, {0}};
  for (int i = 0; i < MOVING_AVG_SIZE; i++) acc_buffer[i] = 0;
}


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

float IMU::moving_average(float new_sample) {
  acc_buffer[acc_index] = new_sample;
  acc_index = (acc_index + 1) % MOVING_AVG_SIZE;

  if (acc_index == 0) buffer_full = true;

  int count = buffer_full ? MOVING_AVG_SIZE : acc_index;
  float sum = 0;
  for (int i = 0; i < count; i++) sum += acc_buffer[i];
  return sum / count;
}

void IMU::estimate_velocity_imu(imu_data_t *imu_data, float acceleration, float time_interval) {
  float delta_v = 100.0f * 0.5f * (imu_data->prev_acc + acceleration) * time_interval; // cm/s
  imu_data->velocity = 0.9f * imu_data->velocity + 0.1f * delta_v;
  imu_data->prev_acc = acceleration;
}

void IMU::update() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0f;
  if (dt <= 0) dt = 1e-3;
  lastTime = now;

  // Aceleración filtrada
  accX = moving_average(a.acceleration.x);

  if (accX <= 1 && accX >= -1)
  {
    accX = 0.0; // umbral para evitar ruido
  }


  // Estimación por integración trapezoidal
  estimate_velocity_imu(&imu_data, accX, dt);
  vel_measured = imu_data.velocity;

  // Kalman 1D
  float vel_pred = vel_measured;
  P += Q;

  float z = vel_measured;
  K = P / (P + R);
  vel_kalman = vel_pred + K * (z - vel_pred);
  P = (1 - K) * P;
}