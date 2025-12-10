package com.iot20252.smartbike.mobileapp.ui

import android.content.Context
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Icon
import androidx.compose.material3.NavigationBar
import androidx.compose.material3.NavigationBarItem
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.ui.Modifier
import androidx.lifecycle.viewmodel.compose.viewModel
import com.iot20252.smartbike.mobileapp.factory.DeviceViewModelFactory
import com.iot20252.smartbike.mobileapp.ui.components.BottomNavItem
import com.iot20252.smartbike.mobileapp.ui.scenes.HomeScene
import com.iot20252.smartbike.mobileapp.ui.scenes.MapScene
import com.iot20252.smartbike.mobileapp.ui.scenes.NotificationScene
import com.iot20252.smartbike.mobileapp.ui.scenes.SettingsScene
import com.iot20252.smartbike.mobileapp.viewModels.DeviceViewModel
import com.iot20252.smartbike.mobileapp.viewModels.HomeViewModel

@Composable
fun HomeScreen(homeViewModel: HomeViewModel, context: Context) {
    val selectedBottomNavItem by homeViewModel.selectedBottomNavItem
    val deviceViewModel: DeviceViewModel = viewModel(
        key = "${homeViewModel.userId.value}_${homeViewModel.deviceId.value}", // 👈 unique key
        factory = DeviceViewModelFactory(homeViewModel = homeViewModel)
    )
    deviceViewModel.listenToDevice(homeViewModel.userId.value, homeViewModel.deviceId.value)
    val deviceData = deviceViewModel.deviceData.value

    Scaffold(
        bottomBar = {
            NavigationBar {
                BottomNavItem.entries.forEach { item ->
                    NavigationBarItem(
                        icon = { Icon(item.icon, contentDescription = item.label) },
                        label = { Text(item.label) },
                        selected = item == selectedBottomNavItem,
                        onClick = {homeViewModel.selectedItem(item)}
                    )
                }
            }
        }
    ) { innerPadding ->
        Box(modifier = Modifier
            .padding(innerPadding)
            .fillMaxSize()){
            when (selectedBottomNavItem){
                BottomNavItem.Home -> HomeScene(deviceData)
                BottomNavItem.Map -> MapScene(deviceData)
                BottomNavItem.Notifications -> NotificationScene(homeViewModel)
                BottomNavItem.Settings -> SettingsScene(context, homeViewModel, deviceViewModel)
            }
        }
    }
}