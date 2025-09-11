#include <wifi_funcs.hpp>

void ask_credentials(String &ssid, String &pass)
{
    Serial.println("\nConfiguración WiFi:");
    Serial.print("Ingrese SSID: ");
    while (ssid.length() == 0)
    {
        if (Serial.available())
        {
            ssid = Serial.readStringUntil('\n');
            ssid.trim();
        }
    }
    Serial.print("\nIngrese Contraseña: ");
    while (pass.length() == 0)
    {
        if (Serial.available())
        {
            pass = Serial.readStringUntil('\n');
            pass.trim();
        }
    }
}

bool connect_to_wifi(const String &ssid, const String &pass)
{

    Serial.println("\nConectando a WiFi...");

    WiFi.begin(ssid.c_str(), pass.c_str());
    int timeout = 0;

    while (WiFi.status() != WL_CONNECTED && timeout < 20)
    {
        delay(500);
        Serial.print(".");
        timeout++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("\nConectado a WiFi!");
        Serial.print("Dirección IP: ");
        Serial.println(WiFi.localIP());
        return true;
    } else {
        Serial.println("\nError al conectar a WiFi.");
        return false;
    }
}
