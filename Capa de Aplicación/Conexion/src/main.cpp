#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>

#define DNS_PORT 53

Preferences prefs;
WebServer server(80);
DNSServer dns;

// =============================================================
//                     HTML FORMULARIO
// =============================================================
String htmlPage(const String& currentSSID, const String& currentName) {
  return R"(
  <!DOCTYPE html>
  <html>
  <head>
    <meta charset="utf-8">
    <title>Configuración WiFi</title>
    <style>
      body { font-family: Arial; text-align: center; margin-top: 40px; }
      input { padding: 10px; margin: 8px; width: 240px; }
      button { padding: 12px 30px; margin-top: 15px; font-size: 16px; }
      .box { width: 350px; margin: auto; padding: 20px; border-radius: 12px;
             border: 1px solid #ddd; background: #fafafa; }
    </style>
  </head>
  <body>
    <div class="box">
      <h2>Configuración del Dispositivo</h2>

      <p><b>SSID actual:</b> )" + currentSSID + R"(</p>
      <p><b>Nombre actual:</b> )" + currentName + R"(</p>

      <form action="/save" method="post">
        <input name="device" placeholder="Nombre del dispositivo"><br>
        <input name="ssid" placeholder="SSID WiFi"><br>
        <input name="pass" type="password" placeholder="Contraseña"><br>
        <button type="submit">Guardar</button>
      </form>
    </div>
  </body>
  </html>
  )";
}

// =============================================================
//          RUTAS "CAPTIVE PORTAL" PARA ANDROID/IOS/WINDOWS
// =============================================================
void setupCaptiveRedirects() {
  auto redirectToRoot = []() {
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
  };
  
  server.on("/generate_204", redirectToRoot);          // Android
  server.on("/gen_204", redirectToRoot);               // Otros Android
  server.on("/hotspot-detect.html", redirectToRoot);   // iOS / macOS
  server.on("/library/test/success.html", redirectToRoot);
  server.on("/fwlink", redirectToRoot);                // Windows
}

// =============================================================
//                        GUARDADO
// =============================================================
void handleSave() {
  String name = server.arg("device");
  String ssid = server.arg("ssid");
  String pass = server.arg("pass");

  if (ssid.isEmpty() || pass.isEmpty()) {
    server.send(400, "text/html", "<h3>Error: SSID o contraseña vacíos.</h3>");
    return;
  }

  if (name.isEmpty()) name = "ESP32";

  prefs.begin("wifi", false);
  prefs.putString("ssid", ssid);
  prefs.putString("pass", pass);
  prefs.putString("name", name);
  prefs.end();

  server.send(200, "text/html",
              "<h3>Datos guardados correctamente.<br>Reiniciando...</h3>");

  delay(2000);
  ESP.restart();
}

// =============================================================
//                   MODO ACCESS POINT
// =============================================================
void startAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP32_Setup", "12345678");

  IPAddress apIP = WiFi.softAPIP();
  dns.start(DNS_PORT, "*", apIP);

  Serial.println("\n=== MODO CONFIGURACIÓN ===");
  Serial.println("AP: ESP32_Setup");
  Serial.print("IP: ");
  Serial.println(apIP);

  // Rutas del portal cautivo
  setupCaptiveRedirects();

  // Ruta principal
  server.on("/", []() {
    prefs.begin("wifi", true);
    String ssid = prefs.getString("ssid", "(no configurado)");
    String name = prefs.getString("name", "(sin nombre)");
    prefs.end();

    server.send(200, "text/html", htmlPage(ssid, name));
  });

  // Ruta de guardado
  server.on("/save", handleSave);

  server.begin();
  Serial.println("Servidor HTTP y DNS iniciados.");
}

// =============================================================
//              CONECTARSE A WIFI GUARDADO
// =============================================================
bool connectToWiFi() {
  prefs.begin("wifi", true);
  String ssid = prefs.getString("ssid", "");
  String pass = prefs.getString("pass", "");
  String name = prefs.getString("name", "ESP32");
  prefs.end();

  if (ssid.isEmpty()) return false;

  Serial.printf("Intentando conectar a '%s'...\n", ssid.c_str());

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), pass.c_str());

  for (int i = 0; i < 20; i++) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Conexión exitosa!");
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
      Serial.printf("Nombre: %s\n", name.c_str());
      return true;
    }
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nNo se pudo conectar.");
  return false;
}

// =============================================================
void setup() {
  Serial.begin(115200);
  delay(300);

  Serial.println("\n=== Inicio del sistema ===");

  if (!connectToWiFi()) {
    startAP();
  }
}

// =============================================================
void loop() {
  dns.processNextRequest();
  server.handleClient();
}
