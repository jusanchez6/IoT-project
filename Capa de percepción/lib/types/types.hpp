#if !defined(TYPES_HPP)
#define TYPES_HPP


typedef struct
{
    float latitude;     ///< Latitud actual [grados]
    float longitude;    ///< Longitud actual [grados]
    float altitude;     ///< Altitud actual [m]
    float velocity;     ///< Velocidad GNSS [km/h]
    bool gnssReady;     ///< Estado de GNSS (true = fix disponible)
    String localTime;   ///< Hora local 

} gpsData_t;

typedef struct
{
    float aX;
    float aY;
    float aZ;
    float gX;
    float gY;
    float gZ;
    float vibraciones;

} imuData_t;

typedef struct
{
    float roll;
    float picht;

} anglesData_t;

typedef struct
{
    String deviceId = "esp32_Julián";
    unsigned long timeStamp = 0;
    unsigned long sequenceId = 0;

    gpsData_t gpsData;

    imuData_t imuData;

    anglesData_t anglesData;

} Telemetry_t;

#endif // TYPES_HPP
