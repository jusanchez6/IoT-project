package com.iot20252.smartbike.mobileapp.ui.components

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.text.KeyboardOptions
import androidx.compose.material3.Button
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.input.KeyboardType
import androidx.compose.ui.unit.dp

@Composable
fun SettingsForm (
    userId: String,
    deviceId: String,
    maxVelocity: Float,
    onSubmit: (String, String, Float) -> Unit
) {
    var userId:String by remember { mutableStateOf(userId) }
    var deviceId:String by remember { mutableStateOf(deviceId) }
    var maxVelocity:String by remember { mutableStateOf(maxVelocity.toString()) }

    Column (
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp),
        verticalArrangement = Arrangement.spacedBy(12.dp)
    ) {
        Text("Settings", style = MaterialTheme.typography.titleLarge)

        OutlinedTextField(
            value = userId,
            onValueChange = {userId = it},
            label = { Text("User ID") },
            singleLine = true,
            modifier = Modifier.fillMaxWidth().padding(horizontal = 4.dp)
        )

        OutlinedTextField(
            value = deviceId,
            onValueChange = {deviceId = it},
            label = { Text("Device Name") },
            singleLine = true,
            modifier = Modifier.fillMaxWidth().padding(horizontal = 4.dp)
        )


        OutlinedTextField(
            value = maxVelocity,
            onValueChange = {maxVelocity = it},
            label = { Text("Max Speed (km/h)") },
            singleLine = true,
            keyboardOptions = KeyboardOptions(keyboardType = KeyboardType.Number),
            modifier = Modifier.fillMaxWidth().padding(horizontal = 4.dp)
        )

        Button (
            onClick = {
                val velocity = maxVelocity.toFloatOrNull() ?: 30f
                onSubmit(userId, deviceId, velocity)
            },
            modifier = Modifier.fillMaxWidth().padding(horizontal = 4.dp)
        ) {
            Text("Submit")
        }
    }
}