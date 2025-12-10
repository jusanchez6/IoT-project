package com.iot20252.smartbike.mobileapp.ui.scenes

import android.content.Context
import android.widget.Toast
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import com.iot20252.smartbike.mobileapp.ui.components.SettingsForm
import com.iot20252.smartbike.mobileapp.viewModels.DeviceViewModel
import com.iot20252.smartbike.mobileapp.viewModels.HomeViewModel

@Composable
fun SettingsScene(context: Context, homeViewModel: HomeViewModel, deviceViewModel: DeviceViewModel) {
    Box(Modifier.fillMaxSize(), contentAlignment = Alignment.Center) {
        SettingsForm(
            userId = homeViewModel.userId.value,
            deviceId = homeViewModel.deviceId.value,
            maxVelocity = homeViewModel.maxVelocity.value,
            onSubmit = { uid, did, maxVel ->
                deviceViewModel.validateAndListen(uid, did) { success ->
                    if (success) {
                        // Only update HomeViewModel if Firestore path has data
                        homeViewModel.updateSettings(uid, did, maxVel)
                        Toast.makeText(context, "New settings applied: $uid / $did", Toast.LENGTH_SHORT).show()
                    } else {
                        // Reject change
                        Toast.makeText(context, "Invalid route: no data found", Toast.LENGTH_SHORT).show()
                    }
                }
            }
        )
    }
}