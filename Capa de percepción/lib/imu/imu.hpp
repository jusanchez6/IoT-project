/**
 * @file imu.hpp
 * @brief Definición de la clase IMU para adquisición y procesamiento de datos del sensor MPU6050.
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

#ifndef IMU_HPP
#define IMU_HPP

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define WIN_SIZE 35          ///< Tamaño de la ventana para integración de la velocidad
#define MOVING_AVG_SIZE 15   ///< Tamaño del buffer para el filtro de media móvil

/**
 * @struct imu_data_t
 * @brief Estructura para almacenar datos del IMU y su procesamiento.
 */
typedef struct
{
    float velocity;              ///< Velocidad estimada (cm/s).
    float prev_acc;              ///< Última aceleración registrada (m/s^2).
    float Window[WIN_SIZE];      ///< Ventana de datos para integración y filtrado.
} imu_data_t;

/**
 * @class IMU
 * @brief Clase para manejo del sensor MPU6050 con estimación de velocidad.
 *
 * Ofrece funciones para inicializar y configurar el sensor, adquirir datos de aceleración,
 * aplicar filtros y estimar la velocidad mediante integración numérica y filtros Kalman
 * y de media móvil.
 */
class IMU
{
public:
    /**
     * @brief Constructor de la clase IMU.
     */
    IMU();
    
    /**
     * @brief Inicializa el sensor MPU6050.
     * 
     * @param wire Bus I2C a utilizar (por defecto `&Wire`).
     * @param addr Dirección I2C del dispositivo (por defecto 0x68).
     * @return `true` si el sensor fue encontrado y configurado correctamente, `false` en caso contrario.
     */
    bool begin(TwoWire *wire = &Wire, uint8_t addr = 0x68);

    /**
     * @brief Configura parámetros adicionales del MPU6050 (rango, filtros, etc).
     */
    void config();

    /**
     * @brief Actualiza las lecturas del sensor y procesa los datos de aceleración.
     */
    void update();

    /**
     * @brief Obtiene la aceleración en el eje X.
     * @return Aceleración medida en X (m/s^2).
     */
    float getAccX () const {return accX; }

    /**
     * @brief Obtiene la velocidad estimada por integración directa de la aceleración.
     * @return Velocidad estimada (cm/s).
     */
    float getVelMeasured() const {return vel_measured; }
    
    /**
     * @brief Obtiene la velocidad filtrada mediante Kalman.
     * @return Velocidad filtrada (cm/s).
     */
    float getVelKalman() const {return vel_kalman; }

private:
    Adafruit_MPU6050 mpu;     ///< Objeto de la librería Adafruit para el MPU6050.
    imu_data_t imu_data;      ///< Datos del IMU y ventana de integración.
    unsigned long lastTime;   ///< Marca de tiempo de la última actualización.

    // ======= KALMAN =========
    float P;  ///< Covarianza del error de estimación.
    float Q;  ///< Varianza del ruido del proceso.
    float R;  ///< Varianza del ruido de la medición.
    float K;  ///< Ganancia de Kalman.

    // ===== FILTRO DE MEDIA MOVIL ====
    float acc_buffer[MOVING_AVG_SIZE];  ///< Buffer circular de aceleraciones.
    int acc_index;                      ///< Índice actual en el buffer.
    bool buffer_full;                   ///< Indica si el buffer está lleno.

    // ===== ESTIMACIÓN DE VELOCIDAD =====
    /**
     * @brief Calcula el promedio móvil de un nuevo dato de aceleración.
     * @param new_sample Nueva muestra de aceleración.
     * @return Aceleración filtrada.
     */
    float moving_average(float new_sample);

    /**
     * @brief Estima la velocidad a partir de la aceleración usando integración.
     * @param imu_data Estructura con datos previos y ventana.
     * @param acceleration Nueva muestra de aceleración.
     * @param time_interval Intervalo de tiempo desde la última actualización (s).
     */
    void estimate_velocity_imu(imu_data_t *imu_data, float acceleration, float time_interval);

    // ==== VARIABLES DE SALIDA =====
    float accX;          ///< Aceleración medida en el eje X.
    float vel_measured;  ///< Velocidad estimada por integración directa.
    float vel_kalman;    ///< Velocidad filtrada con Kalman.
};

#endif // IMU_HPP
