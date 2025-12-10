package com.iot20252.smartbike.mobileapp.ui.scenes

import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.remember
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import com.google.android.gms.maps.CameraUpdateFactory
import com.google.android.gms.maps.model.CameraPosition
import com.google.android.gms.maps.model.LatLng
import com.google.maps.android.compose.GoogleMap
import com.google.maps.android.compose.Marker
import com.google.maps.android.compose.MarkerState
import com.google.maps.android.compose.rememberCameraPositionState
import com.google.maps.android.compose.rememberUpdatedMarkerState
import com.iot20252.smartbike.mobileapp.dataClasses.DeviceData

@Composable
fun MapScene(deviceData: DeviceData) {
    val gps = deviceData.gps
    val position = LatLng(gps.latitude ?: 0.0, gps.longitude ?: 0.0)
    val zoom = 17f
    val cameraPositionState = rememberCameraPositionState()
    val markerState = remember { MarkerState(position = position) }

    // Update camera position directly
    LaunchedEffect(position, zoom) {
        markerState.position = position
        cameraPositionState.move(
            update = CameraUpdateFactory.newLatLngZoom(position, zoom)
        )
    }

    Column (Modifier.fillMaxSize()) {

        Text("This is your position! Lat: ${gps.latitude}, Long: ${gps.longitude}",
            modifier = Modifier.align(Alignment.CenterHorizontally))

        Box (Modifier.fillMaxSize().weight(1f),
            contentAlignment = Alignment.Center)
        {
            GoogleMap(
                modifier = Modifier.fillMaxSize(),
                cameraPositionState = cameraPositionState
            ) {
                Marker (
                    state = markerState,
                    title = deviceData.device_id,
                    snippet = "Device at ${position.latitude}, ${position.longitude}"
                )
            }
        }
    }
}