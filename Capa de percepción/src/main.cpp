#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// ===== WiFi =====
String ssid;
String password;

// ===== IMU =====
Adafruit_MPU6050 mpu;

#define WIN_SIZE 35

typedef struct {
  float velocity;      // velocidad estimada (cm/s)
  float prev_acc;      // aceleración anterior (m/s^2)
  float window[WIN_SIZE];
} imu_data_t;

imu_data_t imu_data = {0.0, 0.0, {0}};

// ===== Kalman =====
float P = 1.0;   // Covarianza del error
float Q = 0.01;  // Varianza del proceso (incertidumbre del modelo)
float R = 0.8;   // Varianza de la medida (ruido del sensor)
float K;         // Ganancia de Kalman

unsigned long lastTime = 0;

// ===== Función de estimación con integración trapezoidal =====
void estimate_velocity_imu(imu_data_t *imu_data, float acceleration, float time_interval) {
    // v(t) = v(t-1) + (a(t-1) + a(t)) * dt / 2
    float delta_v = 100.0f * 0.5f * (imu_data->prev_acc + acceleration) * time_interval; // cm/s

    // Filtro exponencial para suavizar
    imu_data->velocity = 0.9f * imu_data->velocity + 0.1f * delta_v;

    imu_data->prev_acc = acceleration; // actualizar aceleración anterior
}

#define MOVING_AVG_SIZE 15   // tamaño de la ventana de media móvil
float acc_buffer[MOVING_AVG_SIZE];
int acc_index = 0;
bool buffer_full = false;

// --- Filtro de media móvil ---
float moving_average(float new_sample) {
  acc_buffer[acc_index] = new_sample;
  acc_index = (acc_index + 1) % MOVING_AVG_SIZE;

  if (acc_index == 0) buffer_full = true;

  int count = buffer_full ? MOVING_AVG_SIZE : acc_index;
  float sum = 0;
  for (int i = 0; i < count; i++) {
    sum += acc_buffer[i];
  }
  return sum / count;
}


// ===== Configuración MPU =====
void config_mpu() {
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(10);
}

void setup() {
  Serial.begin(115200);
  Wire.begin(45, 35);  // Pines ESP32-S3 SDA=45, SCL=35

  delay(2000);

  // ===== WiFi =====
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
  Serial.println("\nConectando a WiFi...");
  WiFi.begin(ssid.c_str(), password.c_str());
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 20) {
    delay(500); Serial.print("."); timeout++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado!");
    Serial.print("IP: "); Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nNo se pudo conectar.");
  }

  // ===== IMU =====
  Serial.println("\nConfiguración MPU6050:");
  if (!mpu.begin(0x68, &Wire)) {  // cambia a 0x69 si AD0=3V3
    Serial.println("No se encontró un sensor MPU6050 válido, revise conexiones/dirección.");
    while (1) delay(10);
  }
  Serial.println("MPU6050 OK");
  config_mpu();

  lastTime = millis();
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0f;
  if (dt <= 0) dt = 1e-3;
  lastTime = now;

  // --- Medida: aceleración en X ---
  float accX = a.acceleration.x; // m/s^2

  accX = moving_average(accX);  // valor filtrado

  // --- Estimación por integración trapezoidal ---
  estimate_velocity_imu(&imu_data, accX, dt);
  float vel_measured = imu_data.velocity; // cm/s

  // --- Kalman 1D sobre la velocidad ---
  // Predicción
  float vel_pred = vel_measured; // en este caso usamos lo integrado como predicción
  P += Q;

  // Actualización
  float z = vel_measured; // medida (ruidosa)
  K = P / (P + R);
  float vel_kalman = vel_pred + K * (z - vel_pred);
  P = (1 - K) * P;

  // --- Salida ---
  Serial.print("AccX:");
  Serial.print(accX);
  Serial.print(",");
  Serial.print("Vel (Int):");
  Serial.print(vel_measured);
  Serial.print(",");
  Serial.print("Vel (Kalman):");
  Serial.print(vel_kalman);
  Serial.println("");

  delay(50);
}
