#include <WiFi.h>
#include <WebServer.h>

// --- 1. Wi-Fi Credentials ---
const char* ssid = "*******"; // Exact name with the space included
const char* password = "*******";

// --- 2. Hardware Pins ---
const int RELAY_PIN = 26;
const int SOUND_SENSOR_PIN = 18;

// --- 3. System State Variables ---
bool relayIsOn = false;
unsigned long lastClapTime = 0;
const unsigned long CLAP_COOLDOWN = 1500; // 1.5s cooldown to ignore relay click echo

unsigned long lastWifiCheck = 0;
const unsigned long WIFI_RETRY_INTERVAL = 10000;
bool serverStarted = false;

WebServer server(80);

// --- Helper Function: Changes Relay State Safely from Any Source ---
void setRelayState(bool newState, const char* source) {
  relayIsOn = newState;
  digitalWrite(RELAY_PIN, relayIsOn ? HIGH : LOW);
  
  // Reset cooldown timer whether triggered by Phone OR Clap
  // so the relay's physical click sound doesn't re-trigger the mic!
  lastClapTime = millis(); 
  
  Serial.print("Relay turned ");
  Serial.print(relayIsOn ? "ON" : "OFF");
  Serial.print(" (via ");
  Serial.print(source);
  Serial.println(")");
}

// --- 1. Main Web Page UI (With Live Auto-Sync) ---
void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<style>body{text-align:center;font-family:Arial,sans-serif;margin-top:15%;background:#f4f4f9;color:#333;} ";
  html += ".card{background:white;padding:30px;border-radius:16px;display:inline-block;box-shadow:0 4px 12px rgba(0,0,0,0.1);min-width:260px;} ";
  html += ".btn{display:inline-block;padding:18px 40px;font-size:22px;border-radius:10px;color:white;text-decoration:none;margin-top:15px;font-weight:bold;cursor:pointer;border:none;} ";
  html += ".on{background-color:#e53935;} .off{background-color:#43a047;}</style></head><body>";
  
  html += "<div class=\"card\"><h1>Smart Switch</h1>";
  html += "<p>Current State: <strong id=\"statusText\" style=\"color:";
  html += (relayIsOn ? "#43a047;\">ON" : "#e53935;\">OFF");
  html += "</strong></p>";
  
  if (relayIsOn) {
    html += "<a id=\"toggleBtn\" href=\"/toggle\" class=\"btn on\">Turn OFF</a>";
  } else {
    html += "<a id=\"toggleBtn\" href=\"/toggle\" class=\"btn off\">Turn ON</a>";
  }
  html += "<p style=\"font-size:12px;color:#888;margin-top:20px;\">Clap or tap button to toggle</p></div>";

  // JavaScript to automatically update the phone screen if you clap!
  html += "<script>";
  html += "setInterval(function() {";
  html += "  fetch('/state').then(response => response.text()).then(state => {";
  html += "    const statusText = document.getElementById('statusText');";
  html += "    const btn = document.getElementById('toggleBtn');";
  html += "    if (state === 'ON') {";
  html += "      statusText.innerText = 'ON'; statusText.style.color = '#43a047';";
  html += "      btn.innerText = 'Turn OFF'; btn.className = 'btn on';";
  html += "    } else {";
  html += "      statusText.innerText = 'OFF'; statusText.style.color = '#e53935';";
  html += "      btn.innerText = 'Turn ON'; btn.className = 'btn off';";
  html += "    }";
  html += "  });";
  html += "}, 1500);"; // Checks for claps every 1.5 seconds
  html += "</script></body></html>";

  server.send(200, "text/html", html);
}

// --- 2. Phone Button Action ---
void handleToggle() {
  setRelayState(!relayIsOn, "Phone");
  server.sendHeader("Location", "/");
  server.send(303);
}

// --- 3. Background State Check for Phone Sync ---
void handleState() {
  server.send(200, "text/plain", relayIsOn ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);
  delay(500);
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Start safely OFF
  pinMode(SOUND_SENSOR_PIN, INPUT);
  
  Serial.println("\nSystem Ready! Listening for both Claps and Phone commands...");

  // Safe & stable Wi-Fi configuration
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false); 
  WiFi.begin(ssid, password);
  WiFi.setTxPower(WIFI_POWER_15dBm); // Keeps ESP32 cool while maintaining good range

  // Set up web server routes
  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.on("/state", handleState);
}

void loop() {
  // 1. Check for a Clap at full speed (no delay blocking the sensor!)
  if (digitalRead(SOUND_SENSOR_PIN) == HIGH) {
    if (millis() - lastClapTime > CLAP_COOLDOWN) {
      setRelayState(!relayIsOn, "Clap");
    }
  }

  // 2. Handle Phone Web Server & Wi-Fi Connection
  if (WiFi.status() == WL_CONNECTED) {
    if (!serverStarted) {
      Serial.println("\n==========================================");
      Serial.println("Wi-Fi Connected!");
      Serial.print("Phone Control Link: http://");
      Serial.println(WiFi.localIP());
      Serial.println("==========================================\n");
      server.begin();
      serverStarted = true;
    }
    server.handleClient();
  } else {
    if (millis() - lastWifiCheck > WIFI_RETRY_INTERVAL) {
      lastWifiCheck = millis();
      Serial.println("Wi-Fi disconnected... Reconnecting in background (Clap sensor still active).");
      WiFi.reconnect();
    }
  }
}