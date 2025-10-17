#include <wifi_funcs.hpp>

const char *ssid = "iPhone de Julián ";
const char *password = "sanchez06";

const char *mqtt_server = "raspberry.local";
const int mqtt_port = 8883;
const char *topic = "bike/sensors";

WiFiClientSecure espClient;
PubSubClient client(espClient);

const char *ca_cert =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIEJzCCAw+gAwIBAgIUNNt+BIOKvD3FIg/4TiU+aDQvcjswDQYJKoZIhvcNAQEL\n"
    "BQAwgaIxCzAJBgNVBAYTAkNPMRIwEAYDVQQIDAlBbnRpb3F1aWExETAPBgNVBAcM\n"
    "CE1lZGVsbGluMQ0wCwYDVQQKDARVZGVBMRcwFQYDVQQLDA5DQS1CaWtlVHJhY2tl\n"
    "cjEYMBYGA1UEAwwPcmFzcGJlcnJ5LmxvY2FsMSowKAYJKoZIhvcNAQkBFhtjZXNh\n"
    "cmEubW9udG95YTFAdWRlYS5lZHUuY28wHhcNMjUxMDE3MTQyNDA4WhcNMjYxMDE3\n"
    "MTQyNDA4WjCBojELMAkGA1UEBhMCQ08xEjAQBgNVBAgMCUFudGlvcXVpYTERMA8G\n"
    "A1UEBwwITWVkZWxsaW4xDTALBgNVBAoMBFVkZUExFzAVBgNVBAsMDkNBLUJpa2VU\n"
    "cmFja2VyMRgwFgYDVQQDDA9yYXNwYmVycnkubG9jYWwxKjAoBgkqhkiG9w0BCQEW\n"
    "G2Nlc2FyYS5tb250b3lhMUB1ZGVhLmVkdS5jbzCCASIwDQYJKoZIhvcNAQEBBQAD\n"
    "ggEPADCCAQoCggEBALgk5RIKX2lnxZwcwdD6K8kbJzwv0BcKJW4Yl0y/9YMDE6rt\n"
    "x5I5vqlSb/vBCihu56w8MpceULFM5142vLwc26HgPFQ+l4tuObpZ50Wmb3T5sY9+\n"
    "wDQ5S/INtTEV0Wql0UK7+Eikut9KYf59ZgXC2UBxXioGM2j9DRmqTAc2iSOu1vGT\n"
    "5kOuR5jK8oQMSrciMEonL2xdKfnlJE9nKBEhteMUFlS8XhPOnftTdD+yyVBPTwEI\n"
    "C5Z/8y9cvcb7FPWKvKVE90sbNmNuW/HHfEKAH+I7W8CmXpCBzw4Osd0qlszp+5Yn\n"
    "VhJ+386TtkT+4gIsUv0G23o2MG3ISYFG+zKJIbcCAwEAAaNTMFEwHQYDVR0OBBYE\n"
    "FDGTlzeNqXp+mor8SfB9z89l0MFUMB8GA1UdIwQYMBaAFDGTlzeNqXp+mor8SfB9\n"
    "z89l0MFUMA8GA1UdEwEB/wQFMAMBAf8wDQYJKoZIhvcNAQELBQADggEBAKFL28PS\n"
    "K9B8b5yRe5gXPqaGx5gKBQv8bGxKreq3Mh6F5Kl5yrssjyN+ezd29wu7hw/HtETb\n"
    "nnp79S+119NFnJeMGYm/3KIpdRdYSWYHGbifsTfDdmlBc3SyT96NV8GFCmmE+bXZ\n"
    "GnwWxg2SCffZAgQKZFpI6+Z3GBZkiguUwdqe6v0Zmdwu6L06wF4uLBLvDfBI0LeT\n"
    "AAUJEDoxwu7YteCmscTjPXiLIecB4jSvmSHWiR91+0s/AfC7HxRSLhQUVnWnjGx3\n"
    "YHUSRTcqPjE3+BA1AnlbYHfsbk+eeUbv7uZxnBOIQ5szTm1JhJ2UeBVTdbwDUPXq\n"
    "NJTPHALEWsGlEsY=\n"
    "-----END CERTIFICATE-----\n";

void setup_wifi()
{
    Serial.println();
    Serial.print("Conectando a Wifi: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }

    Serial.print("\nWiFi conectado IP: ");
    Serial.println(WiFi.localIP());
}

void syncTime()
{
    configTime(0, 0, "time.google.com", "co.pool.ntp.org");
    Serial.print("Esperando sincronización NTP");
    time_t now = time(nullptr);

    while (now < 8 * 3600 * 2)
    {
        delay(500);
        Serial.print(".");
        now = time(nullptr);
    }

    Serial.println("\nHora Sincronizada");
}

void reconnect()
{

    while (!client.connected())
    {
        Serial.print("Conectando al broker MQTT...");

        if (client.connect("ESP32Client"))
        {
            Serial.println("Conectado");
        }
        else
        {
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
    setup_wifi();
    syncTime();

    espClient.setCACert(ca_cert);
    client.setServer(mqtt_server, mqtt_port);
    reconnect();
}