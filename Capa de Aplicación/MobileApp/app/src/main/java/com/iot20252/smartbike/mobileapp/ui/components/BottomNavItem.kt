package com.iot20252.smartbike.mobileapp.ui.components

import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Home
import androidx.compose.material.icons.filled.Map
import androidx.compose.material.icons.filled.Notifications
import androidx.compose.material.icons.filled.Settings
import androidx.compose.ui.graphics.vector.ImageVector

enum class BottomNavItem(val label:String, val icon: ImageVector) {
    Home("Home", Icons.Default.Home),
    Map("Map", Icons.Default.Map),
    Notifications("Notifications", Icons.Default.Notifications),
    Settings("Settings", Icons.Default.Settings)
}