#include <wifi_funcs.hpp>

const char *ssid = "iPhone de Julián ";
const char *password = "sanchez06";

const char *mqtt_server = "raspberry.local";
const int mqtt_port = 8883;
const char *topic = "bike/mtls";

WiFiClientSecure espClient;
PubSubClient client(espClient);

const char *ca_cert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIEATCCAumgAwIBAgIUYhVuRgeR247Wbu/gyz+zO+tYj8EwDQYJKoZIhvcNAQEL\n"
    "BQAwgY8xCzAJBgNVBAYTAkNPMRIwEAYDVQQIDAlBbnRpb3F1aWExETAPBgNVBAcM\n"
    "CE1lZGVsbGluMQ0wCwYDVQQKDARVZGVBMQwwCgYDVQQLDANJb1QxEDAOBgNVBAMM\n"
    "B0NBLUJpa2UxKjAoBgkqhkiG9w0BCQEWG2p1bGlhbi5zYW5jaGV6NkB1ZGVhLmVk\n"
    "dS5jbzAeFw0yNTEwMzEyMjA2MTNaFw0yNjEwMzEyMjA2MTNaMIGPMQswCQYDVQQG\n"
    "EwJDTzESMBAGA1UECAwJQW50aW9xdWlhMREwDwYDVQQHDAhNZWRlbGxpbjENMAsG\n"
    "A1UECgwEVWRlQTEMMAoGA1UECwwDSW9UMRAwDgYDVQQDDAdDQS1CaWtlMSowKAYJ\n"
    "KoZIhvcNAQkBFhtqdWxpYW4uc2FuY2hlejZAdWRlYS5lZHUuY28wggEiMA0GCSqG\n"
    "SIb3DQEBAQUAA4IBDwAwggEKAoIBAQCWuR8Rqof2OgVuTwBI8itA5tSHxgcdVF3J\n"
    "/G4G/hdGOkKqnRwHTU+zCgxqlvwrIw7y2nM0yoKFSHoiRDPBfdvevSA1kpDwmft6\n"
    "53Q1nFzAi6hVOPuo0iPuXrK2BVDYO9WicpEwhCc6I28O+iwLDlyvfbrvhw5WRiq9\n"
    "X+7KKBfOx+a0LWqnhgklIepNpGJvnrjCaniVB7LSHlZAgfeH/6u8LYBLT+7XnJoR\n"
    "H6Eggjbr8nn7q0iXs8Opdq2UgDIqpN0g/fjDJwcFr6BY2zZUqKoqQNBT3zsQyyEJ\n"
    "JX5HMA2JQ5yv5QirQjLFyvd1826dPG6AYZfhqoIoj+UMTqN2MwVXAgMBAAGjUzBR\n"
    "MB0GA1UdDgQWBBSMxCBRXfleNZiE2ztlgK2BfBE5xTAfBgNVHSMEGDAWgBSMxCBR\n"
    "XfleNZiE2ztlgK2BfBE5xTAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3DQEBCwUA\n"
    "A4IBAQAeT4tQX3OjDfH3WC7s0KEoz8n7U3apj1aKLJvLVRSl3VGLCcI/cHnwQV4z\n"
    "91NnN2H5q8PhQymA5ReZHbW0pCYI1HrcU7fHqvJ4G+pXbj+tdM34ndoMxtMciiqx\n"
    "U3GOWGi8xCAr9A1cHHlrx24a3uHtxsgu7Lvo9AtOQzmVl1TOQMPdCiUKsuOQtBLk\n"
    "mjLQMN9es9WLd9j6u5xzYdrnfo7DgEPzQbwCtlfeUaLhomW59PnC4w0Dgd8WVE/A\n"
    "aPFqx8bWdpqoE3pmGVxtjWCf+BcCqQ4BivmCPLC9SWrQ0+8YSUWEQ7lgeAO4xXY6\n"
    "06Nd7WDq1xm5KteSGi9BIE+ZizTg\n"
    "-----END CERTIFICATE-----\n";

const char *client_cert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDrDCCApQCFH5LA+TPsNHvyHhjZ3k75hF0FAq6MA0GCSqGSIb3DQEBCwUAMIGP\n"
    "MQswCQYDVQQGEwJDTzESMBAGA1UECAwJQW50aW9xdWlhMREwDwYDVQQHDAhNZWRl\n"
    "bGxpbjENMAsGA1UECgwEVWRlQTEMMAoGA1UECwwDSW9UMRAwDgYDVQQDDAdDQS1C\n"
    "aWtlMSowKAYJKoZIhvcNAQkBFhtqdWxpYW4uc2FuY2hlejZAdWRlYS5lZHUuY28w\n"
    "HhcNMjUxMTAxMDQwMDM0WhcNMjYxMTAxMDQwMDM0WjCBlDELMAkGA1UEBhMCQ08x\n"
    "EjAQBgNVBAgMCUFudGlvcXVpYTERMA8GA1UEBwwITWVkZWxsaW4xDTALBgNVBAoM\n"
    "BFVkZUExDDAKBgNVBAsMA0lvVDEVMBMGA1UEAwwMRVNQMzItQ2xpZW50MSowKAYJ\n"
    "KoZIhvcNAQkBFhtqdWxpYW4uc2FuY2hlejZAdWRlYS5lZHUuY28wggEiMA0GCSqG\n"
    "SIb3DQEBAQUAA4IBDwAwggEKAoIBAQDiIu+l1ah8VvMdHNclpn2AQDG3a7UhNIkh\n"
    "evOMVnh7ivnOULl4+VEAFzE5FnmOHPnvrxO5zUC7sLzESubeHJUyc/jXDk5Djhix\n"
    "UzDYu0x6hDzWo7UbFmlv66I1CZuigE1GFAGVcaF6fdj4U82HkGAb6kd0PSBTwQPI\n"
    "eRh3GEj9SljY2s/wNqWoaxuFHMYfkmCoP8weV4LtXlvACFslITu8K1aDfGmLBQQ3\n"
    "7y/T0jFTIK7QlSsF2Vgxclzptt9LbZi0siEq5LyvkE52AmQrzBkoqSbbXszRSMLB\n"
    "pieQIGyY/28yiArvDg4wJfT+B1TQM6+dI7RCJNLZmwZ8XSiHt4LzAgMBAAEwDQYJ\n"
    "KoZIhvcNAQELBQADggEBADbMWJR3hqLhrV2oHZ/wRbRn2LISTN0B41HATi1wskx+\n"
    "fnueAnR/Cr34+GA+kSBY6SBe7g3jeZfRK5NtngTcq+vCKl+4oc3GwaSQ3xDYUljS\n"
    "9h1WgyyjoYB+7POCuNvpslK+6u9IkFQZDTBoz2f4NaNusxwIgKfoe+7nh/GNw8SY\n"
    "8Osm1FOWxyNYnlFi+AtXM3oHD6KrSOIomzGOWaH7U9zEbmGbh//CjLJ+y7AKJ/al\n"
    "3s4ylrclvnxfCsDCc8yrDLS/ZnY7To1GEa43L2SYzWbWuXQwQAxlvSluO7h8imPZ\n"
    "NJRDP2/Fq2Aq4oKt3ffDzZjOKBqUHJEJ6UKTgiCLRMY=\n"
    "-----END CERTIFICATE-----\n";

const char *client_key =
    "-----BEGIN PRIVATE KEY-----\n"
    "MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDiIu+l1ah8VvMd\n"
    "HNclpn2AQDG3a7UhNIkhevOMVnh7ivnOULl4+VEAFzE5FnmOHPnvrxO5zUC7sLzE\n"
    "SubeHJUyc/jXDk5DjhixUzDYu0x6hDzWo7UbFmlv66I1CZuigE1GFAGVcaF6fdj4\n"
    "U82HkGAb6kd0PSBTwQPIeRh3GEj9SljY2s/wNqWoaxuFHMYfkmCoP8weV4LtXlvA\n"
    "CFslITu8K1aDfGmLBQQ37y/T0jFTIK7QlSsF2Vgxclzptt9LbZi0siEq5LyvkE52\n"
    "AmQrzBkoqSbbXszRSMLBpieQIGyY/28yiArvDg4wJfT+B1TQM6+dI7RCJNLZmwZ8\n"
    "XSiHt4LzAgMBAAECggEAFXtcVarrnWg37xkpLQfyxLpHnshQeOeM0VpLN/0s5w7B\n"
    "A7YY1tYKuYRg5i66Ff1I48Ad339dxpaxaY10ZT9edi0DKhBo8EzxQ1y6REepEIhu\n"
    "xeff6ym6LJ1VsU3vQKtWHxg4CeWbmxwvMaPp+RaGssNfzbofnIuc6H0fpr/VWAF7\n"
    "lwyiIG2V6VmYkNQLOEcQRPTgj+45PocpnsNk0szqQQ8CmAwdT+YjJyro93sIsw2N\n"
    "UEn30/C4scxzwcEh7U5QwmYzn7Ut3mPxGf5Og8KX4lGK9FC/7GJRrktcdMLHfe6z\n"
    "/LWDaZsnL2+zsBx6c3u8kx/srR6EAQwUpkm/wvkilQKBgQD7zwQQbWwDUFjBBPUA\n"
    "o3PpWcHtGRYEYX1/8HWqXSimC36eEKZQCjTS7PBvsgDodNfwtKbE7p3nTYh/ovDP\n"
    "yccT1gtNjuVcHby2QkwuASlJgGPvgF7mmVnUMx9CSy+bBi5MEgHP+z1Pe28vkblo\n"
    "tzbllLbVDzZc5anKQS0OYl+jfQKBgQDl5oc8TqSPRF3iztE5dV0bKyF7dfUwb+RS\n"
    "tCqBc9BCpUDIZHsk5Z3Y7AncFOfltEL6npLuWW3WCDIfPZ3hBeC9+OiG6HfTl2XZ\n"
    "nCTA+ezQEncoTZ547b9JpzWfytE8hY5VQ5ZU+fOwYEZaQYMnSo7XKgD5KQWdSRxF\n"
    "fy3+qASrLwKBgE2urbhLqXjUPq09VEAgLNAeap80kABF+yuPCR3Ho5SmEye8RzKG\n"
    "E4nGot4ycelEsqGFebVjT79dwfBuWhAgVZQfAlmo7cUfP2qSxWGj5hLQ0wO9/Ijz\n"
    "Yiz+oHQZMdSHcZzyKTD6dw2gLMrmD+boLr1gxY6xJLo/98N3m+jGHG8ZAoGBAIQQ\n"
    "VBk5l+BQxpn2ckvOdoiX0ykHzqxfakp0AoLQk47lPPygc/jzCiTFGrjZLYlPbrZn\n"
    "B0JyDt1HYft+OsY7ySl35aMxwTvEGZbS0m4kY3v3T4nQZLtU33Cg4/YB3QYmNmgW\n"
    "ZjI0T18/Psq0HXIhq0JAd8SpYvdCO0JU14gU3nGzAoGBAO+OsZsNDV4OfM6DSmGv\n"
    "jBrTl9Za9bThcVdRNV2HFFONoXqE1fVwqHrYxYTtiUD2X91YiZIv/AQ8cc/1HXj/\n"
    "y80JDmKi4lA9SiT6WuTRvu6h4F6EzWHtacAHVAI38b4TE6QYSDGcyE8t8k6eio4B\n"
    "91UBK/Kn+zLedVoxE7ddCfvb\n"
    "-----END PRIVATE KEY-----\n";

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


void setup_wifi() {
    Serial.println();
    Serial.print("Cone3ctando a Wifi: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }

    Serial.print("\nWiFi conectado IP: ");
    Serial.println(WiFi.localIP());

    init_communications();

}

void syncTime() {
    configTime(0, 0, "time.google.com", "co.pool.ntp.org");
    Serial.print("Esperando sincronización NTP");
    time_t now = time(nullptr);

    while (now < 8 * 3600 * 2) {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }

    Serial.println("\nHora Sincronizada");
}

void reconnect () {
    while (!client.connected()) {
        Serial.println("Conectando al broker MQTT...");
        
        if (client.connect("ESP32-Client")) {
            Serial.println("Conectado");
        } else {
            Serial.print("Fallo, rc=");
            Serial.print(client.state());
            Serial.println(" Reintentando en 5 segundos...");
            delay(5000);
        }

    }
}

void sendData(SensorData_t &sensor_data)
{
    if (!client.connected())
        reconnect();

    client.loop();

    StaticJsonDocument<200> doc;

    doc["latitude"] = sensor_data.latitude;
    doc["longitude"] = sensor_data.longitude;
    doc["altitude"] = sensor_data.altitude;
    doc["velocity"] = sensor_data.velocity;
    doc["vibration"] = sensor_data.vibraciones;

    char json_msg[200];
    serializeJson(doc, json_msg);

    client.publish(topic, json_msg);

    
}

void init_communications()
{    
    syncTime();

    espClient.setCACert(ca_cert);
    espClient.setCertificate(client_cert);
    espClient.setPrivateKey(client_key);
    client.setServer(mqtt_server, mqtt_port);
    
    reconnect();
}