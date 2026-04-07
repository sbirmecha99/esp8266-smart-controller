# 🛰️ ESP8266 Smart Device Controller

WiFi-enabled embedded system implementing a Finite State Machine (FSM) for real-time device control.

---

## 🚀 Key Features

- **Non-blocking Execution**: Uses `millis()` instead of `delay()` to maintain responsiveness
- **State Machine Design**:
  - `OFF`
  - `ON`
  - `AUTO_TIMEOUT`
- **REST-style API**:  
  - `/api/state` returns current state in JSON
- **Dynamic UI**:
  - Web interface updates in real-time using API polling
- **Lightweight Web Server**:
  - Runs directly on ESP8266

---

## 🛠️ Tech Stack

- **Hardware**: ESP8266 (NodeMCU)
- **Firmware**: C++ (Arduino framework)
- **Communication**: HTTP
- **Frontend**: HTML, CSS, JavaScript

---

## 🔌 API Reference

| Endpoint     | Method | Description |
|--------------|--------|------------|
| `/api/state` | GET    | Returns current state |
| `/on`        | GET    | Set state to ON |
| `/off`       | GET    | Set state to OFF |

---

## 🧠 System Architecture

- **WiFi Layer**: Connects ESP8266 to local network
- **Server Layer**: Handles HTTP requests
- **Logic Layer**: Implements FSM and timing using `millis()`

---

## 🔧 Setup

1. Clone the repo  
2. Add WiFi credentials in `.ino` file  
3. Upload using Arduino IDE  
4. Open Serial Monitor to get IP  
5. Access via browser  

---

## 🎥 Demo

https://drive.google.com/file/d/16QziZQ29N9CDDHXkpdHTcYWKCPKWFhA3/view?usp=sharing

---

## 📈 Future Improvements

- MQTT-based communication
- Sensor-based automation
- OTA firmware updates