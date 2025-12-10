package com.iot20252.smartbike.mobileapp.ui.scenes

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Delete
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
import com.iot20252.smartbike.mobileapp.viewModels.HomeViewModel

@Composable
fun NotificationScene(homeViewModel: HomeViewModel) {
    Column(Modifier.fillMaxSize().padding(16.dp)) {
        Text("Notifications", style = MaterialTheme.typography.titleLarge)

        if (homeViewModel.notifications.isEmpty()) {
            Text("No notifications", Modifier.padding(top = 8.dp))
        } else {
            homeViewModel.notifications.forEach { notification ->
                Card(
                    Modifier
                        .fillMaxWidth()
                        .padding(vertical = 4.dp),
                    colors = CardDefaults.cardColors(containerColor = Color(0xFFE3F2FD))
                ) {
                    Row(
                        Modifier.padding(12.dp),
                        horizontalArrangement = Arrangement.SpaceBetween,
                        verticalAlignment = Alignment.CenterVertically
                    ) {
                        Text(notification.message, Modifier.weight(1f))
                        IconButton(onClick = { homeViewModel.removeNotification(notification.id) }) {
                            Icon(Icons.Default.Delete, contentDescription = "Delete")
                        }
                    }
                }
            }
        }
    }
}