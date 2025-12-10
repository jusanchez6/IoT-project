package com.iot20252.smartbike.mobileapp.dataClasses

data class ImuData(
    val accel_x: Double? = null,
    val accel_y: Double? = null,
    val accel_z: Double? = null,
    val gyro_x: Double? = null,
    val gyro_y: Double? = null,
    val gyro_z: Double? = null,
    val vibration: Double? = null
)
