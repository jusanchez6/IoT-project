package com.iot20252.smartbike.mobileapp.ui.components.functions

import com.hoho.android.usbserial.driver.UsbSerialPort

fun sendConfigToEsp32(
    deviceId: String,
    maxVelocity: Float,
    wifiSsid: String,
    wifiPassword: String,
    serialPort: UsbSerialPort
) {
    // Build a simple command string
    val command = "CONFIG:$deviceId,$maxVelocity,$wifiSsid,$wifiPassword\n"

    // Send over serial
    serialPort.write(command.toByteArray(), 1000)
}