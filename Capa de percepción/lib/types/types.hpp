#if !defined(TYPES_HPP)
#define TYPES_HPP

/**
 * @typedef SensorData
 * @brief Estructura para almacenar datos de GNSS e IMU.
 */
typedef struct
{
    float latitude;     ///< Latitud actual [grados]
    float longitude;    ///< Longitud actual [grados]
    float altitude;     ///< Altitud actual [m]
    float velocity;     ///< Velocidad GNSS [km/h]
    bool gnssReady;     ///< Estado de GNSS (true = fix disponible)
    float vibraciones;  ///< Valor RMS de vibración de la IMU

    String localTime;
} SensorData_t;


#endif // TYPES_HPP


