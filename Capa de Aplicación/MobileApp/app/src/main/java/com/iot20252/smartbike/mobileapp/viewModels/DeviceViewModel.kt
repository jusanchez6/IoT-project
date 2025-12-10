package com.iot20252.smartbike.mobileapp.viewModels

import android.util.Log
import androidx.compose.runtime.State
import androidx.compose.runtime.mutableStateOf
import androidx.lifecycle.ViewModel
import com.google.firebase.firestore.FirebaseFirestore
import com.google.firebase.firestore.ListenerRegistration
import com.google.firebase.firestore.Query
import com.iot20252.smartbike.mobileapp.dataClasses.DeviceData

class DeviceViewModel(
    private val homeViewModel: HomeViewModel
) : ViewModel() {
    private val db = FirebaseFirestore.getInstance()
    private val _deviceData = mutableStateOf(DeviceData())
    val deviceData: State<DeviceData> = _deviceData

    fun validateAndListen(userId: String, deviceId: String, onResult: (Boolean) -> Unit) {
        db.collection("users")
            .document(userId)
            .collection("devices")
            .document(deviceId)
            .collection("readings")
            .limit(1)
            .get()
            .addOnSuccessListener { snapshot ->
                if (snapshot != null && !snapshot.isEmpty) {
                    listenToDevice(userId, deviceId)
                    onResult(true)
                } else {
                    onResult(false)
                }
            }
            .addOnFailureListener {
                onResult(false)
            }
    }

    private var listenerRegistration: ListenerRegistration? = null

    fun listenToDevice(userId: String, deviceId: String) {
        listenerRegistration?.remove() // stop old listener

        listenerRegistration = db.collection("users")
            .document(userId)
            .collection("devices")
            .document(deviceId)
            .collection("readings")
            .orderBy("timestamp", Query.Direction.DESCENDING)
            .limit(1)
            .addSnapshotListener { snapshot, error ->
                if (error != null) {
                    Log.w("Firestore", "Listen failed.", error)
                    return@addSnapshotListener
                }

                if (snapshot != null && !snapshot.isEmpty) {
                    val doc = snapshot.documents.first()
                    val data = doc.toObject(DeviceData::class.java)
                    data?.let {
                        _deviceData.value = it

                        val velocity = (it.gps.velocity ?: 0f).toFloat()
                        if (velocity > homeViewModel.maxVelocity.value) {
                            homeViewModel.addNotification(
                                "Velocity exceeded: $velocity km/h (limit ${homeViewModel.maxVelocity.value} on ${it.timestamp.toDate()})"
                            )
                        }
                    }
                }
            }
    }


    override fun onCleared() {
        super.onCleared()
        listenerRegistration?.remove()
    }
}