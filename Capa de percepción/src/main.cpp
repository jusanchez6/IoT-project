/**
 * @file main.cpp
 * @brief Ejemplo de integración de GNSS, IMU, LED y Alarma con FreeRTOS en un ESP32-S3.
 *
 * Este programa corre en un ESP32-S3 DevKit y gestiona diferentes módulos
 * de percepción y actuación mediante tareas en FreeRTOS:
 * - GNSS (GPS) para posición, altitud y velocidad.
 * - IMU (MPU6050) para aceleración y estimación de velocidad.
 * - LED RGB (NeoPixel) como indicador de estado.
 * - Alarma (buzzer o pin digital) para notificaciones.
 *
 * Cada módulo corre en su propia tarea, lo que permite concurrencia
 * y control en tiempo real.
 *
 * @author Julián Sánchez
 * @date 15/09/2025
 *
 * @version 1.01
 *
 */

// Librerias estandar
#include <Arduino.h>
#include <Wire.h>

// Librerias
#include <gnss.hpp>
#include <wifi_funcs.hpp>
#include <imu.hpp>
#include <platform.hpp>

struct SensorData
{
  float latitude;
  float longitude;
  float altitude;
  float velocity;
  bool gnssReady;

  float accX;
  float velMeasured;
  float velKalman;
};

SensorData sensorData;
SemaphoreHandle_t dataMutex; // Para proteger acceso concurrente

// ======== GNSS =============

#define RX_PIN 17      ///< GPIO para la recepción de datos del GNSS
#define TX_PIN 18      ///< GPIO para la transmisión de datos al GNSS
#define GNSS_BAUD 9600 ///< Tasa de baudios para la comunicación UART con el GNSS

// Inicialización del objeto gnss
GNSS gnss(Serial1, RX_PIN, TX_PIN, GNSS_BAUD);

// ===== IMU =====
// Inicialización del objeto IMU
IMU imu;

// ==== LED =====
#define LED_PIN 48 ///< GPIO del led RGB
#define NUM_LEDS 1 ///< Numeros de leds presentes en la tira de leds (Por defecto se usa el integrado en el ESP32S3)

LED led(NUM_LEDS, LED_PIN);

// ========== ALARMA =================
#define ALARM_PIN 10 ///< GPIO de la alarma (buzzer)

ALARM buzzer(ALARM_PIN);

// ============= TAREAS FreeRTOS ===============================

// ====== Tarea de adquisición de sensores ======
/**
 * @brief Tarea que lee GNSS e IMU y guarda datos en la estructura compartida.
 */
void taskSensors(void *pvParameters) {
  (void) pvParameters;

  for (;;) {
    SensorData temp; // temporal

    // === GNSS ===
    gnss.update();
    temp.gnssReady  = gnss.isReady();
    temp.latitude   = gnss.latitude();
    temp.longitude  = gnss.longitude();
    temp.altitude   = gnss.altitude();
    temp.velocity   = gnss.velocity();

    // === IMU ===
    imu.update();
    temp.accX        = imu.getAccX();
    temp.velMeasured = imu.getVelMeasured();
    temp.velKalman   = imu.getVelKalman();

    // === Copiar datos a la estructura compartida ===
    if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
      sensorData = temp;
      xSemaphoreGive(dataMutex);
    }

    vTaskDelay(pdMS_TO_TICKS(500)); // 2 Hz de muestreo
  }
}


// ====== Tarea de alarma ======
/**
 * @brief Activa la alarma si la velocidad excede el umbral.
 */
void taskAlarm(void *pvParameters) {
  (void) pvParameters;

  const float SPEED_THRESHOLD = 30.0; // km/h

  for (;;) {
    if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
      if (sensorData.gnssReady && sensorData.velocity > SPEED_THRESHOLD) {
        buzzer.turnOnAlarm();
      } else {
        buzzer.turnOffAlarm();
      }
      xSemaphoreGive(dataMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(500)); // 2 Hz
  }
}

/**
 * @brief Cambia el color del LED según el estado del GNSS.
 *
 * - LED rojo mientras no hay fix.
 * - LED verde fijo una vez que el GNSS obtiene fix (permanece verde).
 */
void taskLED(void *pvParameters) {
  (void) pvParameters;

  bool wasReady = false; // Guardar el estado previo

  for (;;) {
    if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
      bool ready = sensorData.gnssReady;

      if (!ready && !wasReady) {
        led.setColor(COLOR_RED);
      } else if (ready && !wasReady) {
        led.setColor(COLOR_GREEN);
      }

      wasReady = ready; // actualizar estado previo
      xSemaphoreGive(dataMutex);
    }

    vTaskDelay(pdMS_TO_TICKS(250)); // 2 Hz basta
  }
}


/**
 * @brief Muestra por Serial los datos de los sensores.
 */
void taskPrint (void *pvParameters) {
  (void) pvParameters;

  for (;;) {
    if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
      Serial.print("GNSS Ready: ");
      Serial.print(sensorData.gnssReady ? "YES" : "NO");
      Serial.print(" | Lat: "); Serial.print(sensorData.latitude, 6);
      Serial.print(" | Lng: "); Serial.print(sensorData.longitude, 6);
      Serial.print(" | Alt: "); Serial.print(sensorData.altitude, 2);
      Serial.print(" m | Vel: "); Serial.print(sensorData.velocity, 2);
      Serial.print(" km/h");

      Serial.print(" || IMU AccX: "); Serial.print(sensorData.accX, 2);
      Serial.print(" | Vel (measured): "); Serial.print(sensorData.velMeasured, 2);
      Serial.print(" | Vel (Kalman): "); Serial.println(sensorData.velKalman, 2);

      xSemaphoreGive(dataMutex);
    }

    vTaskDelay(pdMS_TO_TICKS(1000)); // 1 Hz
  }


}


void setup()
{
  Serial.begin(115200);
  Wire.begin(45, 35); // Pines ESP32-S3 SDA=45, SCL=35

  // ===== GNSS =====
  gnss.begin();
  Serial.println("Iniciando GPS test...");
  delay(2000);

  // ===== IMU =====
  Serial.println("Configuración MPU6050..");
  if (!imu.begin(&Wire, 0x68)) {
    Serial.println("Error. No se pudo encontrar un MPU6050 valido.");
    while (true);
  }
  Serial.println("MPU6050 OK");

  // ===== LED y ALARMA =====
  led.begin();
  buzzer.begin();

  // ===== Mutex para proteger sensorData =====
  dataMutex = xSemaphoreCreateMutex();
  if (dataMutex == NULL) {
    Serial.println("Error: no se pudo crear el mutex.");
    while (true);
  }

  // ===== Crear tareas =====
  xTaskCreatePinnedToCore(taskSensors, "Task Sensors", 4096, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(taskAlarm,   "Task Alarm",   2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(taskLED,     "Task LED",     2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(taskPrint, "Task Print", 2048, NULL, 1, NULL, 0); 
}

void loop () {}