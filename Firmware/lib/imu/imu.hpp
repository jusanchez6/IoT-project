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
 * @version 2.0
 *
 */

#ifndef IMU_HPP
#define IMU_HPP

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define WIN_SIZE 35        ///< Tamaño de la ventana para integración de la velocidad
#define MOVING_AVG_SIZE 15 ///< Tamaño del buffer para el filtro de media móvil

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
     * @brief Obtiene el valor RMS de la vibración.
     */
    float getVibRMS() const { return vibrationRMS; }

    float getAccX() const { return accX; }
    float getAccY() const { return accY; }
    float getAccZ() const { return accZ; }

    float getGyroX() const { return gyroX; }
    float getGyroY() const { return gyroY; }
    float getGyroZ() const { return gyroZ; }

    float getPitch() const { return pitch; }
    float getRoll() const { return roll; }

private:
    Adafruit_MPU6050 mpu;   ///< Objeto de la librería Adafruit para el MPU6050.
    unsigned long lastTime; ///< Marca de tiempo de la última actualización.

    // Variables para calculo de la vibración
    float accX, accY, accZ;
    float gyroX, gyroY, gyroZ;

    float pitch, roll;

    float hpX, hpY, hpZ;                   ///< Componentes de alta frecuencia de la aceleración.
    float accX_prev, accY_prev, accZ_prev; ///< Últimas aceleraciones registradas.

    float vibrationRMS;           ///< Valor RMS de la vibración calculada.
    unsigned long vib_start_time; ///< Marca de tiempo para cálculo de vibración.
    float vib_sumSq;              ///< Suma de cuadrados para cálculo de vibración.
    int vib_count;                ///< Contador de muestras para cálculo de vibración.

    static constexpr float alpha = 0.9f;      ///< Coeficiente para filtro pasa-bajos.
    static constexpr int SAMPLE_WINDOW = 200; ///< Intervalo de tiempo fijo para integración (200 ms).
};

#endif // IMU_HPP
