/**
 * @file gnss.hpp
 * @brief Librería para manejar un receptor GNSS con ESP32 usando TinyGPS++.
 *
 * Esta clase encapsula la configuración y lectura de datos de un módulo GNSS 
 * conectado por UART, permitiendo obtener latitud, longitud, altitud y hora local.
 * 
 * @author Julian Sanchez
 * @date 12/09/2025
 * @version 1.0
 * 
 */

#ifndef GNSS_HPP
#define GNSS_HPP

#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>

#define UTC 0
#define BOGOTA_UTC -5


/**
 * @class GNSS
 * @brief Clase para gestionar la comunicación y extracción de datos GNSS.
 *
 * Esta clase utiliza la librería TinyGPSPlus para decodificar las tramas NMEA
 * recibidas desde un módulo GNSS. Permite inicializar la comunicación, actualizar
 * los datos y acceder a coordenadas, altitud y hora local.
 */
class GNSS
{
public:
    /**
     * @brief Constructor de la clase GNSS.
     * 
     * @param serial Referencia al puerto serial hardware que se usará (ej. Serial1).
     * @param rxPin Pin RX del ESP32 para recibir datos del módulo GNSS.
     * @param txPin Pin TX del ESP32 para enviar datos al módulo GNSS.
     * @param baudRate Velocidad en baudios para la comunicación UART.
     */
    GNSS(HardwareSerial &serial, int rxPin, int txPin, uint32_t baudRate);

    /**
     * @brief Inicializa la comunicación con el módulo GNSS.
     *
     * Configura el puerto serial con los parámetros establecidos en el constructor.
     */
    void begin();

    /**
     * @brief Procesa los datos entrantes desde el GNSS.
     *
     * Lee los caracteres disponibles en el puerto serial y los envía a TinyGPS++
     * para decodificar las sentencias NMEA. Establece la bandera interna
     * `dataReady` cuando hay datos válidos disponibles.
     */
    void update();

    /**
     * @brief Indica si hay nuevos datos GNSS listos para leer.
     * 
     * @return true si se han recibido y decodificado datos válidos, false en caso contrario.
     */
    bool isReady() { return dataReady; }

    /**
     * @brief Obtiene la latitud actual.
     * 
     * @return Latitud en grados decimales, o NAN si no hay datos válidos.
     */
    double latitude() { return gps.location.isValid() ? gps.location.lat() : NAN; }

    /**
     * @brief Obtiene la longitud actual.
     * 
     * @return Longitud en grados decimales, o NAN si no hay datos válidos.
     */
    double longitude() { return gps.location.isValid() ? gps.location.lng() : NAN; }

    /**
     * @brief Obtiene la altitud actual.
     * 
     * @return Altitud en metros, o NAN si no hay datos válidos.
     */
    double altitude() { return gps.altitude.isValid() ? gps.altitude.meters() : NAN; }

    /**
     * @brief Obtiene la hora local ajustada con un offset horario.
     * 
     * @param offsetHours Diferencia horaria respecto a UTC (ej. -5 para Colombia).
     * @return Cadena con la hora local en formato HH:MM:SS, o "Invalid" si no hay datos válidos.
     */
    String localTime(int offsetHours = BOGOTA_UTC);

private:
    HardwareSerial &gnssSerial; ///< Referencia al puerto serial usado por el GNSS.
    TinyGPSPlus gps;            ///< Instancia de TinyGPSPlus para decodificación NMEA.
    int rx;                     ///< Pin RX del ESP32 conectado al TX del GNSS.
    int tx;                     ///< Pin TX del ESP32 conectado al RX del GNSS.
    uint32_t baud;              ///< Velocidad en baudios para la comunicación serial.
    bool dataReady = false;     ///< Bandera que indica si hay datos válidos disponibles.

    /**
     * @brief Configura la tasa de actualización del GNSS.
     *
     * Envía un comando al módulo GNSS para ajustar la frecuencia de entrega
     * de datos (ej. 1 Hz, 5 Hz, 10 Hz). Se llama internamente tras la inicialización.
     * 
     * @param serial Puerto serial asociado al módulo GNSS.
     */
    void setGnssRate(HardwareSerial &serial);
};

#endif // GNSS_HPP
