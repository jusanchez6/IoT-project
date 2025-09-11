#ifndef IMU_HPP
#define IMU_HPP

#include <Arduino.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define WIN_SIZE 35
#define MOVING_AVG_SIZE 15

typedef struct
{
    float velocity;
    float prev_acc;
    float Window[WIN_SIZE];
} imu_data_t;

class IMU
{
public:
    IMU();
    
    bool begin(TwoWire *wire = &Wire, uint8_t addr = 0x68);

    void config();

    void update();


    float getAccX () const {return accX; }

    float getVelMeasured() const {return vel_measured; }
    
    float getVelKalman() const {return vel_kalman; }


private:

    Adafruit_MPU6050 mpu;
    imu_data_t imu_data;
    unsigned long lastTime;

    // ======= KALMAN =========
    float P;
    float Q;
    float R;
    float K;

    // ===== FILTRO DE MEDIA MOVIL ====

    float acc_buffer[MOVING_AVG_SIZE];
    int acc_index;
    bool buffer_full;

    // ===== ESTIMACIÓN DE VELOCIDAD =====
    float moving_average(float new_sample);
    void estimate_velocity_imu(imu_data_t *imu_data, float acceleration, float time_interval);

    // ==== VARIABLES DE SALIDA =====
    float accX;
    float vel_measured;
    float vel_kalman;

};

#endif // IMU_HPP