package com.iot20252.smartbike.mobileapp.factory

import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import com.iot20252.smartbike.mobileapp.viewModels.DeviceViewModel
import com.iot20252.smartbike.mobileapp.viewModels.HomeViewModel

class DeviceViewModelFactory(
    private val homeViewModel: HomeViewModel
) : ViewModelProvider.Factory {
    @Suppress("UNCHECKED_CAST")
    override fun <T : ViewModel> create(modelClass: Class<T>): T {
        if (modelClass.isAssignableFrom(DeviceViewModel::class.java)) {
            return DeviceViewModel(homeViewModel) as T
        }
        throw IllegalArgumentException("Unknown ViewModel class")
    }
}