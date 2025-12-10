package com.iot20252.smartbike.mobileapp.ui.components

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color

@Composable
fun StatsDashboard(speed: Float, accel: Float, inclination: Float) {
    Column (
        modifier = Modifier.fillMaxWidth(),
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.SpaceEvenly
    ) {
        CircularStat(
            label = "Speed",
            value = speed,
            maxValue = 100f, // adjust max speed
            unit = "km/h",
            color = Color.Blue
        )
        CircularStat(
            label = "Acceleration",
            value = accel,
            maxValue = 20f, // adjust max acceleration
            unit = "m/s²",
            color = Color.Red
        )
        CircularStat(
            label = "Inclination",
            value = inclination,
            maxValue = 90f, // degrees
            unit = "°",
            color = Color.Magenta
        )
    }
}