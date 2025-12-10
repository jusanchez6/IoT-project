package com.iot20252.smartbike.mobileapp

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import com.google.android.gms.maps.MapsInitializer
import com.google.firebase.FirebaseApp
import com.iot20252.smartbike.mobileapp.ui.HomeScreen
import com.iot20252.smartbike.mobileapp.ui.theme.MobileAppTheme
import com.iot20252.smartbike.mobileapp.viewModels.HomeViewModel

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        MapsInitializer.initialize(applicationContext)
        val homeViewModel = HomeViewModel()
        val context = this

        enableEdgeToEdge()
        setContent {
            MobileAppTheme {
                HomeScreen(homeViewModel, context)
            }
        }
    }
}