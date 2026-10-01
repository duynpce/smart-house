#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>

/**
 * @class WiFiManager
 * @brief Manages wireless network connectivity and status monitoring.
 */
class WiFiManager {
private:
    String ssid;
    String password;
    unsigned long lastReconnectAttempt;
    const unsigned long reconnectInterval = 10000; // 10 seconds

public:
    /**
     * @brief Constructor
     * @param ssid WiFi Network SSID
     * @param password WiFi Network Password
     */
    WiFiManager(const String& ssid, const String& password);

    /**
     * @brief Destructor
     */
    ~WiFiManager();

    /**
     * @brief Initializes and connects to WiFi
     * @return true if connected successfully, false otherwise
     */
    bool connect();

    /**
     * @brief Disconnects from WiFi
     */
    void disconnect();

    /**
     * @brief Checks if WiFi is currently connected
     * @return true if connected, false otherwise
     */
    bool isConnected() const;

    /**
     * @brief Call inside main loop() to auto-reconnect if connection drops
     */
    void handleReconnect();

    /**
     * @brief Gets local IP address
     * @return IP address as String
     */
    String getIpAddress() const;

    /**
     * @brief Gets current WiFi signal strength (RSSI)
     * @return RSSI in dBm
     */
    int getRSSI() const;
};

#endif // WIFI_MANAGER_H
