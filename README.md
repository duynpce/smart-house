# Smart House IoT System Architecture (C++)

## 1. Project Directory & Categorized Structure

```
smart-house/
├── config/
│   ├── Config.h                  # Dynamic selector (switches between local and prod)
│   ├── Config.h.local            # Direct hardcoded values for local debugging / Arduino IDE
│   └── Config.h.prod             # Macro-injected variables from Docker / PlatformIO
├── network/
│   ├── WiFiManager.h             # WiFi connectivity & auto-reconnect
│   └── ApiClient.h               # REST API client with API Key auth (POST/GET)
├── security/
│   └── DoorController.h          # Password buffer, API login check, lock relay
├── sensors/
│   ├── HazardDetector.h          # Fire & Gas monitor, local alarm, incident alert
│   └── EnvironmentMonitor.h      # Temp, Humidity & Rain telemetry reporter
├── actuators/
│   └── ApplianceController.h     # Light & Fan relay control & remote sync
├── core/
│   └── SmartHouseSystem.h        # Master orchestrator facade (begin & update)
├── platformio.ini                # PlatformIO build environments & compile-time flags
├── Dockerfile                    # Containerized PlatformIO build image
├── docker-compose.yml            # Docker Compose pipeline to inject environment variables
├── .env.example                  # Template for production environment variables
├── smart-house.ino               # Arduino entrypoint
└── README.md
```

---

## 2. Running in Arduino IDE

### Step 1: Open the Project
1. Open **Arduino IDE** (version 2.x or 1.8.x).
2. Go to **File > Open...** and select:
   ```
   d:/GithubRepository/smart-house-adruno/smart-house/smart-house.ino
   ```

### Step 2: Configure Credentials (`Config.h.local`)
When building inside Arduino IDE, [`Config.h`](file:///d:/GithubRepository/smart-house-adruno/smart-house/config/Config.h) automatically loads **[`config/Config.h.local`](file:///d:/GithubRepository/smart-house-adruno/smart-house/config/Config.h.local)**.
Open [`config/Config.h.local`](file:///d:/GithubRepository/smart-house-adruno/smart-house/config/Config.h.local) and edit your local network and backend parameters:
```cpp
const char* wifiSsid = "YOUR_HOME_WIFI";
const char* wifiPassword = "YOUR_WIFI_PASSWORD";
const char* backendUrl = "http://192.168.1.100:8080/api/v1";
const char* apiKey = "your-api-key";
```

### Step 3: Install the ESP32 Board Package
1. In Arduino IDE, go to **File > Preferences** (or `Ctrl + ,`).
2. Add this URL into **Additional Boards Manager URLs**:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Open **Tools > Board > Boards Manager...**, search for `esp32` by **Espressif Systems**, and click **Install**.

### Step 4: Install Required Libraries
Open **Tools > Manage Libraries...** (`Ctrl + Shift + I`) and install:
- **ArduinoJson** by *Benoît Blanchon*
- **DHT sensor library** by *Adafruit*
- **Adafruit Unified Sensor** by *Adafruit*

### Step 5: Select Board & Upload
1. Connect your ESP32 board via USB.
2. Select **Tools > Board > esp32 > DOIT ESP32 DEVKIT V1** (or **ESP32 Dev Module**).
3. Select **Tools > Port** and choose the connected COM port (e.g., `COM3`, `COM4`).
4. Click **Upload (➡️)** to compile and flash the firmware.
5. Open **Tools > Serial Monitor** and set baud rate to **`115200`** to inspect logs.

---

## 3. Running with PlatformIO & Docker Compose

### Step 1: PlatformIO Configuration (`platformio.ini`)
PlatformIO reads system/Docker environment variables and injects them as preprocessor macros during compilation:
```ini
[env:esp32-prod]
platform = espressif32
board = esp32dev
framework = arduino
build_flags = 
    -D PROD_BUILD
    -D WIFI_SSID=\"${sysenv.WIFI_SSID}\"
    -D WIFI_PASSWORD=\"${sysenv.WIFI_PASSWORD}\"
    -D BACKEND_URL=\"${sysenv.BACKEND_URL}\"
    -D API_KEY=\"${sysenv.API_KEY}\"
```

### Step 2: C++ Header Switching
- [`Config.h.local`](file:///d:/GithubRepository/smart-house-adruno/smart-house/config/Config.h.local): Hardcoded values for direct local development.
- [`Config.h.prod`](file:///d:/GithubRepository/smart-house-adruno/smart-house/config/Config.h.prod): Reads macros (`WIFI_SSID`, `API_KEY`, etc.) passed by compiler flags.
- [`Config.h`](file:///d:/GithubRepository/smart-house-adruno/smart-house/config/Config.h): Automatically selects between `Config.h.prod` and `Config.h.local`.

### Step 3: Build using Docker Compose
1. Copy the environment file template:
   ```bash
   cp .env.example .env
   ```
2. Edit `.env` with your production parameters:
   ```env
   WIFI_SSID="MyHomeNetwork"
   WIFI_PASSWORD="MySecurePassword"
   BACKEND_URL="https://api.myserver.com/v1"
   API_KEY="my-secret-device-api-key"
   ```
3. Run Docker Compose to compile:
   ```bash
   docker compose up --build
   ```
4. Output binary will be placed at `.pio/build/esp32-prod/firmware.bin`.

---

## 4. Module & Class Responsibilities

| Directory | Header File | Class / Struct | Key Responsibilities |
| :--- | :--- | :--- | :--- |
| `config/` | [`Config.h`](file:///d:/GithubRepository/smart-house-adruno/smart-house/config/Config.h) | `struct AppConfig` | Hardware pin definitions, WiFi credentials, server endpoints, intervals, thresholds |
| `network/` | [`WiFiManager.h`](file:///d:/GithubRepository/smart-house-adruno/smart-house/network/WiFiManager.h) | [`WiFiManager`](file:///d:/GithubRepository/smart-house-adruno/smart-house/network/WiFiManager.h#L9) | Wireless connection management and automatic non-blocking reconnection |
| `network/` | [`ApiClient.h`](file:///d:/GithubRepository/smart-house-adruno/smart-house/network/ApiClient.h) | [`ApiClient`](file:///d:/GithubRepository/smart-house-adruno/smart-house/network/ApiClient.h#L28) | HTTP/REST requests (POST/GET) with `x-api-key` authentication headers |
| `security/` | [`DoorController.h`](file:///d:/GithubRepository/smart-house-adruno/smart-house/security/DoorController.h) | [`DoorController`](file:///d:/GithubRepository/smart-house-adruno/smart-house/security/DoorController.h#L11) | Keypad PIN input buffer, API login authorization, relay unlock & auto-lock |
| `sensors/` | [`HazardDetector.h`](file:///d:/GithubRepository/smart-house-adruno/smart-house/sensors/HazardDetector.h) | [`HazardDetector`](file:///d:/GithubRepository/smart-house-adruno/smart-house/sensors/HazardDetector.h#L21) | Fire & Gas sensor reading, local alarm (buzzer/LED), and instant API alerts |
| `sensors/` | [`EnvironmentMonitor.h`](file:///d:/GithubRepository/smart-house-adruno/smart-house/sensors/EnvironmentMonitor.h) | [`EnvironmentMonitor`](file:///d:/GithubRepository/smart-house-adruno/smart-house/sensors/EnvironmentMonitor.h#L11) | DHT (temp/humidity) & rain sensor reading, scheduled telemetry reporting |
| `actuators/`| [`ApplianceController.h`](file:///d:/GithubRepository/smart-house-adruno/smart-house/actuators/ApplianceController.h) | [`ApplianceController`](file:///d:/GithubRepository/smart-house-adruno/smart-house/actuators/ApplianceController.h#L11) | Light & Fan relay switching and backend remote command synchronization |
| `core/` | [`SmartHouseSystem.h`](file:///d:/GithubRepository/smart-house-adruno/smart-house/core/SmartHouseSystem.h) | [`SmartHouseSystem`](file:///d:/GithubRepository/smart-house-adruno/smart-house/core/SmartHouseSystem.h#L17) | Master facade orchestrating all subsystems in `begin()` and `update()` |

---

## 5. Teammate Implementation Guide (.cpp Files)

1. **`network/WiFiManager.cpp`**: Implement `WiFi.begin(ssid, password)` and non-blocking reconnection.
2. **`network/ApiClient.cpp`**: Use `HTTPClient` and `ArduinoJson` to send HTTP requests with `x-api-key`.
3. **`security/DoorController.cpp`**: Drive relay pin `HIGH`/`LOW` and manage auto-relock timers with `millis()`.
4. **`sensors/HazardDetector.cpp`**: Poll digital flame and analog MQ gas sensors; trigger sirens and incident API.
5. **`sensors/EnvironmentMonitor.cpp`**: Read DHT and rain sensors; send telemetry every `intervalMs`.
6. **`actuators/ApplianceController.cpp`**: Switch relays for Light and Fan; poll server for commands.
7. **`core/SmartHouseSystem.cpp`**: Instantiate sub-controllers and invoke their `begin()` and `update()` methods.
