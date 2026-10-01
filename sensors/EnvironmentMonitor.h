#ifndef ENVIRONMENT_MONITOR_H
#define ENVIRONMENT_MONITOR_H

#include <Arduino.h>
#include "../network/ApiClient.h"

/**
 * @class EnvironmentMonitor
 * @brief Reads environmental sensors (temperature, humidity, rain) and sends telemetry data to backend.
 */
class EnvironmentMonitor {
private:
    uint8_t dhtPin;
    uint8_t dhtType;
    uint8_t rainPin;
    int rainThreshold;
    unsigned long intervalMs;
    unsigned long lastTelemetrySentTime;
    
    TelemetryData lastTelemetryData;
    ApiClient* apiClient;

public:
    /**
     * @brief Constructor
     * @param dhtPin GPIO connected to DHT sensor
     * @param dhtType Type of DHT sensor (e.g. 11 for DHT11, 22 for DHT22)
     * @param rainPin GPIO connected to rain sensor (analog/digital)
     * @param rainThreshold ADC threshold to consider it raining
     * @param intervalMs How often to send data to server (in milliseconds)
     * @param apiClient Pointer to ApiClient instance for transmitting telemetry
     */
    EnvironmentMonitor(uint8_t dhtPin, uint8_t dhtType, uint8_t rainPin, int rainThreshold, unsigned long intervalMs, ApiClient* apiClient);

    /**
     * @brief Destructor
     */
    ~EnvironmentMonitor();

    /**
     * @brief Initializes sensors and pins
     */
    void begin();

    /**
     * @brief Reads temperature from DHT sensor
     * @return Temperature in Celsius
     */
    float readTemperature();

    /**
     * @brief Reads humidity from DHT sensor
     * @return Relative humidity percentage (%)
     */
    float readHumidity();

    /**
     * @brief Reads rain sensor
     * @return true if rain is detected, false otherwise
     */
    bool checkRain();

    /**
     * @brief Reads raw rain sensor analog value
     * @return Raw analog reading
     */
    int readRainRaw();

    /**
     * @brief Reads all sensors into a TelemetryData struct
     * @return Updated TelemetryData struct
     */
    TelemetryData readAllSensors();

    /**
     * @brief Main non-blocking update routine. Periodically gathers data and sends to backend.
     * Call inside main loop().
     */
    void update();

    /**
     * @brief Returns latest cached sensor reading
     */
    TelemetryData getLatestData() const;
};

#endif // ENVIRONMENT_MONITOR_H
