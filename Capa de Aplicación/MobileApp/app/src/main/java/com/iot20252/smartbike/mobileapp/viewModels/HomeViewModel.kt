package com.iot20252.smartbike.mobileapp.viewModels

import androidx.compose.runtime.State
import androidx.compose.runtime.mutableStateListOf
import androidx.compose.runtime.mutableStateOf
import androidx.lifecycle.ViewModel
import com.iot20252.smartbike.mobileapp.ui.components.BottomNavItem

data class AppNotification(val id: Long, val message: String)

class HomeViewModel: ViewModel() {
    private val _selectedBottomNavItem = mutableStateOf(BottomNavItem.Home)
    val selectedBottomNavItem: State<BottomNavItem> = _selectedBottomNavItem
    var userId = mutableStateOf("Julián")
    var deviceId = mutableStateOf("esp32_Julián")
    var maxVelocity = mutableStateOf(30f)
    var notifications = mutableStateListOf<AppNotification>()


    fun updateSettings(newUserId:String, newDeviceId:String, newMaxVelocity: Float){
        userId.value = newUserId
        deviceId.value = newDeviceId
        maxVelocity.value = newMaxVelocity
    }

    fun selectedItem(item: BottomNavItem){
        _selectedBottomNavItem.value = item
    }

    fun addNotification(message: String) {
        notifications.add(AppNotification(System.currentTimeMillis(), message))
    }

    fun removeNotification(id: Long) {
        notifications.removeAll { it.id == id }
    }
}