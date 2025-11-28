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
 * @version 1.3
 */

#include <Arduino.h>
#include <Wire.h>

#include <gnss.hpp>
#include <wifi_funcs.hpp>
#include <imu.hpp>
#include <platform.hpp>
#include <types.hpp>

flags_t flags;

SemaphoreHandle_t dataMutex; ///< Mutex para proteger acceso concurrente a sensorData

Telemetry_t Data;
// ======== GNSS =============
#define RX_PIN 17      ///< GPIO para la recepción de datos del GNSS
#define TX_PIN 18      ///< GPIO para la transmisión de datos al GNSS
#define GNSS_BAUD 9600 ///< Tasa de baudios para la comunicación UART con el GNSS

#define SPEED_THRESHOLD 30.0

GNSS gnss(Serial1, RX_PIN, TX_PIN, GNSS_BAUD); ///< Objeto GNSS

// ===== IMU =====
IMU imu; ///< Objeto IMU (MPU6050)

// ==== LED =====
#define LED_PIN 48 ///< GPIO del LED RGB
#define NUM_LEDS 1 ///< Número de LEDs presentes en la tira (por defecto el integrado en ESP32-S3)

LED led(NUM_LEDS, LED_PIN); ///< Objeto LED RGB

// ========== ALARMA =================
#define ALARM_PIN 10 ///< GPIO de la alarma (buzzer)

ALARM buzzer(ALARM_PIN); ///< Objeto buzzer/alarma

// ============= TAREAS FreeRTOS ===============================

/**
 * @brief Tarea que lee GNSS e IMU y guarda datos en la estructura compartida.
 *
 * - Actualiza el estado del GNSS (posición, altitud, velocidad).
 * - Lee aceleración y calcula velocidades desde la IMU.
 * - Copia la información en la variable global @ref sensorData protegida por un mutex.
 *
 * @param pvParameters Parámetros de tarea (no usados).
 */
void taskSensors(void *pvParameters)
{

  Telemetry_t temp; // temporal

  for (;;)
  {

    // === TELEMETRIA ====
    temp.timeStamp = millis();
    temp.sequenceId += 1;

    // === GNSS ===
    gnss.update();
    temp.gpsData.gnssReady = gnss.isReady();
    temp.gpsData.latitude = gnss.latitude();
    temp.gpsData.longitude = gnss.longitude();
    temp.gpsData.altitude = gnss.altitude();
    temp.gpsData.velocity = gnss.velocity();
    temp.gpsData.localTime = gnss.localTime();

    // === IMU ===
    imu.update();

    temp.imuData.vibraciones = imu.getVibRMS();
    temp.imuData.aX = imu.getAccX();
    temp.imuData.aY = imu.getAccY();
    temp.imuData.aZ = imu.getAccZ();

    temp.imuData.gX = imu.getGyroX();
    temp.imuData.gY = imu.getGyroY();
    temp.imuData.gZ = imu.getGyroZ();

    temp.anglesData.picht = imu.getPitch();
    temp.anglesData.roll = imu.getRoll();

    // === Copiar datos a la estructura compartida ===
    if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE)
    {
      Data = temp;
      xSemaphoreGive(dataMutex);
    }

    vTaskDelay(pdMS_TO_TICKS(1000)); // 1 Hz de muestreo
  }
}

/**
 * @brief Tarea que activa la alarma si hay mas vibraciones de lo normal para indicar el estado de una carretera.
 *
 * - Enciende el buzzer si GNSS tiene fix y velocidad > 30 km/h.
 * - Apaga el buzzer en caso contrario.
 *
 * @param pvParameters Parámetros de tarea (no usados).
 */
void taskAlarm(void *pvParameters)
{

  for (;;)
  {
    if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE)
    {
      if (Data.gpsData.gnssReady && Data.gpsData.velocity > SPEED_THRESHOLD)
      {
        buzzer.turnOnAlarm();
      }
      else
      {
        buzzer.turnOffAlarm();
      }

      xSemaphoreGive(dataMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

/**
 * @brief Tarea que cambia el color del LED según el estado del GNSS.
 *
 * - LED rojo mientras no hay fix.
 * - LED verde fijo una vez que el GNSS obtiene fix
 *
 * @param pvParameters Parámetros de tarea (no usados).
 */
void taskLED(void *pvParameters)
{
  bool wasReady = false; ///< Estado previo del GN0SS

  for (;;)
  {
    if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE)
    {
      bool ready = Data.gpsData.gnssReady;

      if (!ready && !wasReady)
      {
        led.setColor(COLOR_RED);
      }
      else if ((ready && !wasReady) | flags.connectionBroker)
      {
        led.setColor(COLOR_GREEN);
      }
      else if (!flags.connectionBroker)
      {
        led.setColor(COLOR_MUSTARD);
      } 

      wasReady = ready; // actualizar estado previo
      xSemaphoreGive(dataMutex);
    }

    vTaskDelay(pdMS_TO_TICKS(250)); // 2 Hz
  }
}

// /**
//  * @brief Tarea que imprime en Serial los datos de los sensores.
//  *
//  * - Estado GNSS (fix o no).
//  * - Latitud, longitud, altitud, velocidad.
//  * - Datos de la IMU (aceleración y velocidades).
//  *
//  * @param pvParameters Parámetros de tarea (no usados).
//  */
// void taskPrint (void *pvParameters) {

//   for (;;) {
//     if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
//       Serial.print("GNSS Ready: ");
//       Serial.print(sensorData.gnssReady ? "YES" : "NO");
//       Serial.print(" | Lat: "); Serial.print(sensorData.latitude, 6);
//       Serial.print(" | Lng: "); Serial.print(sensorData.longitude, 6);
//       Serial.print(" | Alt: "); Serial.print(sensorData.altitude, 2);
//       Serial.print(" m | Vel: "); Serial.print(sensorData.velocity, 2);
//       Serial.print(" km/h | Vibraciones: "); Serial.print(sensorData.vibraciones, 2);
//       Serial.println(" m/s^2");

//       xSemaphoreGive(dataMutex);
//     }

//     vTaskDelay(pdMS_TO_TICKS(1000)); // 1 Hz
//   }
// }

void taskSend(void *pvParameters)
{

  for (;;)
  {
    if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10) == pdTRUE))
    {
      String msg = msgToJson(Data);
      flags.connectionBroker = sendData(msg);

      vTaskDelay(pdMS_TO_TICKS(500));

      xSemaphoreGive(dataMutex);
      if (!flags.connectionBroker)
      {
        Serial.println("Datos no enviados.\n");
      }
      else
      {
        Serial.println("Datos enviados.\n");
      }
    }

    vTaskDelay(pdMS_TO_TICKS(30000));
  }
}

void taskWiFiPortal(void *pvParameters)
{
  for (;;)
  {
    dns.processNextRequest();
    server.handleClient();
    vTaskDelay(pdMS_TO_TICKS(5)); // evitamos watchdog
  }
}

/**
 * @brief Configuración inicial del sistema.
 *
 * - Inicializa Serial, I2C, GNSS, IMU, LED y buzzer.
 * - Crea mutex de protección de datos.
 * - Lanza las tareas de FreeRTOS.
 */
void setup()
{
  Serial.begin(115200);
  Wire.begin(45, 35);

  // ===== GNSS =====
  gnss.begin();
  Serial.println("Iniciando GPS...");
  delay(2000);

  // ===== IMU =====
  Serial.println("Configurando MPU6050...");
  imu.begin(&Wire, 0x68);

  // ===== LED y Buzzer =====
  led.begin();
  led.setColor(COLOR_BLUE);
  buzzer.begin();

  // ===== Mutex =====
  dataMutex = xSemaphoreCreateMutex();

  // ===== WIFI: integrar tus funciones =====
  Serial.println("\n=== Inicio del sistema ===");
  if (!connectToWiFi())
  {
    Serial.println("===================Iniciando modo configuración (AP + portal cautivo) =================== \n");
    startAP();

    xTaskCreatePinnedToCore(taskWiFiPortal,
                            "WiFi Portal",
                            4096,
                            NULL,
                            1,
                            NULL,
                            0);
  }
  else
  {
    Serial.println("Iniciando Comunicación con el broker MQTT...\n");
    init_communications();
  }

  // ===== Tareas =====
  xTaskCreatePinnedToCore(taskSensors, "Task Sensors", 4096, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(taskAlarm, "Task Alarm", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(taskLED, "Task LED", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(taskSend, "Task Send", 8192, NULL, 1, NULL, 0);
}

/**
 * @brief Bucle principal (no utilizado en FreeRTOS).
 */
void loop() {}
