/**
 * @file main.cpp
 * @brief Integración de GNSS, IMU, LED RGB, alarma y comunicaciones MQTT en ESP32-S3 usando FreeRTOS.
 *
 * Este firmware implementa un sistema de adquisición y envío de telemetría mediante:
 * - GNSS (posición, velocidad, hora local).
 * - IMU MPU6050 (vibraciones, aceleración, giroscopio y ángulos).
 * - Indicadores LED RGB basados en el estado del sistema.
 * - Alarma mediante buzzer según condiciones del entorno.
 * - Envío periódico de telemetría al broker MQTT.
 *
 * Cada módulo opera dentro de una tarea independiente de FreeRTOS,
 * lo que permite ejecución concurrente y mayor confiabilidad.
 *
 * Las comunicaciones WiFi y el portal cautivo se gestionan mediante tareas dedicadas.
 *
 * @author Julián
 * @date 15/09/2025
 * @version 3.0
 */

#include <Arduino.h>
#include <Wire.h>
#include <gnss.hpp>
#include <wifi_funcs.hpp>
#include <imu.hpp>
#include <platform.hpp>
#include <types.hpp>

// ===============================================================
// VARIABLES GLOBALES
// ===============================================================

/**
 * @brief Banderas globales de estado del sistema.
 *
 * - `gnssReady` : El GNSS tiene o no FIX.
 * - `connectionWifi` : Conectado o no a WiFi.
 * - `connectionBroker` : Conectado o no a MQTT.
 */
flags_t flags;

/**
 * @brief Mutex usado para proteger el acceso a la estructura compartida de telemetría.
 */
SemaphoreHandle_t dataMutex;

/**
 * @brief Estructura global de telemetría compartida entre tareas.
 */
Telemetry_t Data;


// ===============================================================
// CONFIGURACIÓN DEL GNSS
// ===============================================================

#define RX_PIN 17      ///< GPIO de recepción UART del GNSS
#define TX_PIN 18      ///< GPIO de transmisión UART al GNSS
#define GNSS_BAUD 9600 ///< Velocidad UART del módulo GNSS

/**
 * @brief Umbral de velocidad para activar la alarma (en km/h).
 */
#define SPEED_THRESHOLD 30.0

/**
 * @brief Objeto GNSS encargado de recibir y procesar datos NMEA.
 */
GNSS gnss(Serial1, RX_PIN, TX_PIN, GNSS_BAUD);


// ===============================================================
// CONFIGURACIÓN DE LA IMU
// ===============================================================

/**
 * @brief Objeto IMU (MPU6050) usado para obtener aceleraciones, giros y vibración RMS.
 */
IMU imu;


// ===============================================================
// LED RGB (NeoPixel integrado)
// ===============================================================

#define LED_PIN 48 ///< GPIO del LED RGB integrado
#define NUM_LEDS 1 ///< Número de LEDs controlados

/**
 * @brief Objeto LED RGB para señalización del estado del dispositivo.
 */
LED led(NUM_LEDS, LED_PIN);


// ===============================================================
// ALARMA
// ===============================================================

#define ALARM_PIN 10 ///< GPIO del buzzer de alarma

/**
 * @brief Objeto buzzer usado para alertas según vibración o velocidad.
 */
ALARM buzzer(ALARM_PIN);


// ===============================================================
// TAREAS FREERTOS
// ===============================================================

/**
 * @brief Tarea encargada de leer los sensores GNSS e IMU y actualizar la estructura de telemetría.
 *
 * Funciones principales:
 * - Lector GNSS: posición, altitud, velocidad y estado FIX.
 * - Lector IMU: aceleraciones, giros y vibración RMS.
 * - Cálculo de pitch y roll a partir del MPU6050.
 *
 * Los datos son almacenados en un objeto temporal para luego copiarse
 * a la estructura global protegida por un mutex.
 *
 * Frecuencia: 1 Hz.
 *
 * @param pvParameters Parámetro no utilizado.
 */
void taskSensors(void *pvParameters)
{
    Telemetry_t temp;

    for (;;)
    {
        // === TELEMETRÍA GENERAL ===
        temp.timeStamp = millis();
        temp.sequenceId += 1;

        // === GNSS ===
        gnss.update();
        temp.gpsData.gnssReady = gnss.isReady();
        temp.gpsData.latitude  = gnss.latitude();
        temp.gpsData.longitude = gnss.longitude();
        temp.gpsData.altitude  = gnss.altitude();
        temp.gpsData.velocity  = gnss.velocity();
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
        temp.anglesData.roll  = imu.getRoll();

        // === Copia a la estructura compartida ===
        if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE)
        {
            Data = temp;
            flags.gnssReady = temp.gpsData.gnssReady;
            xSemaphoreGive(dataMutex);
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // 1 Hz
    }
}

/**
 * @brief Tarea encargada de activar o desactivar la alarma según condiciones.
 *
 * Reglas:
 * - Alarma ON si GNSS tiene FIX y la velocidad supera el umbral.
 * - Alarma OFF en caso contrario.
 *
 * Frecuencia: cada 500 ms.
 *
 * @param pvParameters Parámetro no utilizado.
 */
void taskAlarm(void *pvParameters)
{
    for (;;)
    {
        if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE)
        {
            if (Data.gpsData.gnssReady && Data.gpsData.velocity > SPEED_THRESHOLD)
                buzzer.turnOnAlarm();
            else
                buzzer.turnOffAlarm();

            xSemaphoreGive(dataMutex);
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

/**
 * @brief Tarea de gestión del LED RGB según el estado del sistema.
 *
 * Colores asignados:
 * - Rojo: no hay WiFi.
 * - Azul: WiFi OK, GNSS sin FIX.
 * - Amarillo: WiFi y GNSS OK, MQTT desconectado.
 * - Verde: todo funcionando correctamente.
 *
 * Frecuencia: 200 ms.
 *
 * @param pvParameters Parámetro no utilizado.
 */
void taskLED(void *pvParameters)
{
    for (;;)
    {
        if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE)
        {
            bool wifi   = flags.connectionWifi;
            bool gnssOk = flags.gnssReady;
            bool broker = flags.connectionBroker;

            if (!wifi)
                led.setColor(COLOR_RED);
            else if (wifi && !gnssOk)
                led.setColor(COLOR_BLUE);
            else if (wifi && gnssOk && !broker)
                led.setColor(COLOR_MUSTARD);
            else
                led.setColor(COLOR_GREEN);

            xSemaphoreGive(dataMutex);
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

/**
 * @brief Tarea encargada de enviar telemetría al servidor MQTT.
 *
 * Funciones:
 * - Convierte la estructura `Data` a JSON usando msgToJson().
 * - Ejecuta sendData() para enviar al broker.
 * - Actualiza la bandera `flags.connectionBroker`.
 *
 * Frecuencia: cada 30 segundos.
 *
 * @param pvParameters Parámetro no utilizado.
 */
void taskSend(void *pvParameters)
{
    for (;;)
    {
        if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE)
        {
            String msg = msgToJson(Data);
            flags.connectionBroker = sendData(msg);

            Serial.println(flags.connectionBroker ?
                           "Datos enviados.\n" :
                           "Datos no enviados.\n");

            xSemaphoreGive(dataMutex);
        }

        vTaskDelay(pdMS_TO_TICKS(30000));
    }
}

/**
 * @brief Tarea que maneja el portal cautivo mientras el ESP32 está en modo AP.
 *
 * Atiende solicitudes HTTP y resuelve redirecciones DNS para permitir
 * que el usuario configure credenciales WiFi.
 *
 * Frecuencia: 5 ms.
 */
void taskWiFiPortal(void *pvParameters)
{
    for (;;)
    {
        dns.processNextRequest();
        server.handleClient();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

/**
 * @brief Tarea que monitorea el estado de la conexión WiFi.
 *
 * Actualiza periódicamente la bandera:
 * - `flags.connectionWifi = true/false`
 *
 * Frecuencia: 5 s.
 *
 * @param pvParameters Parámetro no utilizado.
 */
void taskWifiMonitor(void *pvParameters)
{
    for (;;)
    {
        flags.connectionWifi = (WiFi.status() == WL_CONNECTED);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}


// ===============================================================
// SETUP
// ===============================================================

/**
 * @brief Configuración principal del sistema.
 *
 * Tareas realizadas:
 * - Inicializa Serial e I2C.
 * - Inicia GNSS, IMU, LED y alarma.
 * - Crea el mutex para acceso concurrente.
 * - Gestiona la conexión WiFi o activa el portal cautivo.
 * - Inicializa MQTT/TLS mediante init_communications().
 * - Crea todas las tareas del sistema.
 */
void setup()
{
    Serial.begin(115200);
    Wire.begin(45, 35);

    // GNSS
    gnss.begin();
    delay(2000);

    // IMU
    imu.begin(&Wire, 0x68);

    // LED y buzzer
    led.begin();
    led.setColor(COLOR_BLUE);
    buzzer.begin();

    // Mutex
    dataMutex = xSemaphoreCreateMutex();

    // === Gestión WiFi y portal cautivo ===
    if (!connectToWiFi())
    {
        flags.connectionWifi = false;
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
        flags.connectionWifi = true;
        init_communications();
    }

    // === Creación de tareas ===
    xTaskCreatePinnedToCore(taskSensors,      "Task Sensors",   4096, NULL, 2, NULL, 1);
    xTaskCreatePinnedToCore(taskAlarm,        "Task Alarm",     2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(taskLED,          "Task LED",       2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(taskSend,         "Task Send",      8192, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(taskWifiMonitor,  "Task monitor",   2048, NULL, 1, NULL, 0);
}

/**
 * @brief Bucle principal (vacío debido a FreeRTOS).
 */
void loop() {}
