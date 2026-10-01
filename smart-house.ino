#include <Arduino.h>
#include "config/Config.h"
#include "core/SmartHouseSystem.h"

// Instantiate global configuration and system
AppConfig config;
SmartHouseSystem smartHouse(config);

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n[SYSTEM] Starting Smart House System...");

    // Initialize all modules
    smartHouse.begin();
}

void loop() {
    // Non-blocking update of all subsystems
    smartHouse.update();
}
