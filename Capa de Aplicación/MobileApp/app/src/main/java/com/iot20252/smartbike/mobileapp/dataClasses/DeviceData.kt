package com.iot20252.smartbike.mobileapp.dataClasses

import com.google.firebase.Timestamp

data class DeviceData(
    val device_id: String = "",
    val timestamp: Timestamp = Timestamp.now(),
    val sequence_id: Long = 0,
    val gps: GpsData = GpsData(),
    val imu: ImuData = ImuData(),
    val angles: AngleData = AngleData()
)
