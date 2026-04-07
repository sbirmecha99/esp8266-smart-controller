#include <ESP8266WiFi.h>

// WiFi credentials
const char* ssid = "LIBRARY_03";
const char* password = "library_03";

WiFiServer server(80);

// -------- STATE MACHINE --------
enum DeviceState {
  OFF,
  ON,
  AUTO_TIMEOUT
};

DeviceState currentState = OFF;

// timing (non-blocking)
unsigned long lastOnTime = 0;
const unsigned long TIMEOUT = 10000; // 10 seconds

// -------- SETUP --------
void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

// -------- STATE HANDLER --------
void handleState() {
  if (currentState == ON) {
    if (millis() - lastOnTime > TIMEOUT) {
      currentState = AUTO_TIMEOUT;
    }
  }

  if (currentState == AUTO_TIMEOUT) {
    currentState = OFF;
  }
}

// -------- REQUEST HANDLER --------
void handleRequest(String request, WiFiClient &client) {

  if (request.indexOf("/on") != -1) {
    currentState = ON;
    lastOnTime = millis();
    Serial.println("State: ON");
  }

  else if (request.indexOf("/off") != -1) {
    currentState = OFF;
    Serial.println("State: OFF");
  }

  // -------- API ENDPOINT --------
  else if (request.indexOf("/api/state") != -1) {

    String stateStr;
    switch (currentState) {
      case OFF: stateStr = "OFF"; break;
      case ON: stateStr = "ON"; break;
      case AUTO_TIMEOUT: stateStr = "AUTO_TIMEOUT"; break;
    }

    String json = "{\"state\":\"" + stateStr + "\"}";

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: application/json");
    client.println("Connection: close");
    client.println();
    client.println(json);

    return; // IMPORTANT: stop further HTML response
  }
}

// -------- HTML RESPONSE --------
String getHTML() {
  String stateStr;

  switch (currentState) {
    case OFF: stateStr = "OFF"; break;
    case ON: stateStr = "ON"; break;
    case AUTO_TIMEOUT: stateStr = "AUTO_TIMEOUT"; break;
  }

  String html = "<!DOCTYPE html><html><head>";
html += "<meta charset='utf-8'>";
html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
html += "<title>ESP8266 Control</title>";

// -------- STYLING --------
html += "<style>";
html += "body { font-family: Arial, sans-serif; text-align: center; background: #f4f6f8; margin-top: 50px; }";
html += "h1 { color: #333; }";
html += ".card { background: white; padding: 30px; margin: auto; width: 300px; border-radius: 12px; box-shadow: 0 4px 12px rgba(0,0,0,0.1); }";
html += ".state { font-size: 22px; margin: 20px 0; }";
html += ".on { color: green; font-weight: bold; }";
html += ".off { color: red; font-weight: bold; }";
html += "button { padding: 12px 25px; font-size: 16px; margin: 10px; border: none; border-radius: 8px; cursor: pointer; }";
html += ".btn-on { background-color: #4CAF50; color: white; }";
html += ".btn-off { background-color: #f44336; color: white; }";
html += "button:hover { opacity: 0.85; }";
html += "</style>";

html += "</head><body>";

// -------- CONTENT --------
html += "<div class='card'>";
html += "<h1>Device Control</h1>";

html += "<p style='font-size:13px; color:#888;'>API endpoint: /api/state</p>";

// Dynamic state color
String stateClass = (stateStr == "ON") ? "on" : "off";

html += "<p class='state'>Current State: <span class='" + stateClass + "'>" + stateStr + "</span></p>";

html += "<a href=\"/on\"><button class='btn-on'>Turn ON</button></a>";
html += "<a href=\"/off\"><button class='btn-off'>Turn OFF</button></a>";

html += "</div>";

html += "<script>";
html += "setInterval(async () => {";
html += "  let res = await fetch('/api/state');";
html += "  let data = await res.json();";
html += "  document.querySelector('.state span').innerText = data.state;";
html += "}, 2000);";
html += "</script>";

html += "</body></html>";

return html;
}

// -------- LOOP --------
void loop() {
  WiFiClient client = server.available();
  if (!client) {
    handleState(); // keep state machine running
    return;
  }

  String request = client.readStringUntil('\r');
  client.flush();

  handleRequest(request, client);
  handleState();

  String response = getHTML();

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println(response);

  delay(1);
}
