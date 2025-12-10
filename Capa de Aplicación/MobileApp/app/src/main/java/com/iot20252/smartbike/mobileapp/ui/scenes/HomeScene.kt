package com.iot20252.smartbike.mobileapp.ui.scenes

import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import com.iot20252.smartbike.mobileapp.dataClasses.DeviceData
import com.iot20252.smartbike.mobileapp.ui.components.StatsDashboard

@Composable
fun HomeScene(deviceData: DeviceData) {
    val gps = deviceData.gps
    val imu = deviceData.imu
    val angles = deviceData.angles

    val speed = (gps.velocity)?.toFloat() ?: 0f
    val accel = (imu.accel_x)?.toFloat() ?: 0f // or magnitude of accel vector
    val inclination = (angles.pitch_deg)?.toFloat() ?: 0f

    Column { Modifier.fillMaxSize().padding(6.dp)
        Box(Modifier.fillMaxSize(), contentAlignment = Alignment.Center){
            StatsDashboard(
                speed = speed,
                accel = accel,
                inclination = inclination
            )
        }
    }
}