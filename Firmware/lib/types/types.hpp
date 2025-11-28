/**
 * @file types.hpp
 * @brief Definiciones de estructuras de datos para GNSS, IMU, ángulos y telemetría general.
 *
 * Este archivo contiene los tipos utilizados para almacenar información de sensores,
 * estados del sistema y paquetes de telemetría enviados por el dispositivo.
 * 
 * @author Julián Sánchez
 * @date 15/09/2025
 * @version 3.0
 */

#if !defined(TYPES_HPP)
#define TYPES_HPP

/**
 * @struct flags_t
 * @brief Indicadores de estado del sistema.
 *
 * Contiene banderas booleanas que permiten conocer si los subsistemas
 * principales están operativos (GNSS, WiFi, Broker MQTT).
 */
typedef struct {
    bool gnssReady;        /**< Indica si el GNSS tiene fix válido. */
    bool connectionWifi;   /**< Indica si el dispositivo está conectado a WiFi. */
    bool connectionBroker; /**< Indica si el dispositivo está conectado al broker MQTT. */
} flags_t;

/**
 * @struct gpsData_t
 * @brief Información GNSS procesada.
 *
 * Almacena los datos principales obtenidos del módulo GNSS, incluyendo posición,
 * velocidad, estado de disponibilidad y hora local.
 */
typedef struct
{
    float latitude;     /**< Latitud actual [grados]. */
    float longitude;    /**< Longitud actual [grados]. */
    float altitude;     /**< Altitud actual [m]. */
    float velocity;     /**< Velocidad medida por GNSS [km/h]. */
    bool gnssReady;     /**< Estado del GNSS (true = fix disponible). */
    String localTime;   /**< Hora local obtenida del sistema o del GNSS. */
} gpsData_t;

/**
 * @struct imuData_t
 * @brief Datos obtenidos de la IMU (acelerómetro + giroscopio).
 *
 * Incluye aceleraciones lineales, velocidades angulares y magnitud de vibración.
 */
typedef struct
{
    float aX;           /**< Aceleración eje X [m/s²]. */
    float aY;           /**< Aceleración eje Y [m/s²]. */
    float aZ;           /**< Aceleración eje Z [m/s²]. */
    float gX;           /**< Velocidad angular eje X [°/s]. */
    float gY;           /**< Velocidad angular eje Y [°/s]. */
    float gZ;           /**< Velocidad angular eje Z [°/s]. */
    float vibraciones;  /**< Magnitud calculada de vibraciones. */
} imuData_t;

/**
 * @struct anglesData_t
 * @brief Ángulos calculados del sistema.
 *
 * Incluye roll y pitch provenientes del filtrado (Madgwick, Kalman, etc.).
 */
typedef struct
{
    float roll;   /**< Ángulo de roll [grados]. */
    float picht;  /**< Ángulo de pitch [grados]. */
} anglesData_t;

/**
 * @struct Telemetry_t
 * @brief Paquete completo de telemetría enviado a la nube.
 *
 * Contiene información del dispositivo, metadatos de envío, y todos los datos
 * relevantes del GNSS, IMU y ángulos.
 */
typedef struct
{
    String deviceId = "esp32_Julián";  /**< Identificador del dispositivo. */
    String userId = "Julián";          /**< Identificador del usuario. */
    unsigned long timeStamp = 0;       /**< Timestamp UNIX del envío. */
    unsigned long sequenceId = 0;      /**< Contador incremental para mensajes. */

    gpsData_t gpsData;                 /**< Datos GNSS completos. */
    imuData_t imuData;                 /**< Datos de IMU. */
    anglesData_t anglesData;           /**< Ángulos calculados. */

} Telemetry_t;

#endif // TYPES_HPP
