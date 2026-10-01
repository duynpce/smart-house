#ifndef HAZARD_DETECTOR_H
#define HAZARD_DETECTOR_H

#include <Arduino.h>
#include "../network/ApiClient.h"

/**
 * @enum HazardStatus
 * @brief Current safety status
 */
enum class HazardStatus {
    SAFE,
    FIRE_DETECTED,
    GAS_DETECTED,
    FIRE_AND_GAS_DETECTED
};

/**
 * @class HazardDetector
 * @brief Monitors fire and gas sensors, activates local alarms, and sends server alerts.
 */
class HazardDetector {
private:
    uint8_t flamePin;
    uint8_t gasPin;
    uint8_t alarmBuzzerPin;
    uint8_t alarmLedPin;
    int gasThreshold;

    HazardStatus currentStatus;
    unsigned long lastAlertSentTime;
    const unsigned long alertCooldownMs = 15000; // Cooldown between alert API calls (15s)
    
    ApiClient* apiClient;

    /**
     * @brief Triggers local buzzer and LED alarm patterns
     * @param active true to sound alarm, false to silence
     */
    void setLocalAlarm(bool active);

public:
    /**
     * @brief Constructor
     * @param flamePin GPIO connected to flame/fire sensor
     * @param gasPin Analog GPIO connected to MQ gas sensor
     * @param alarmBuzzerPin GPIO for emergency siren/buzzer
     * @param alarmLedPin GPIO for emergency alert LED
     * @param gasThreshold ADC threshold value for gas hazard detection
     * @param apiClient Pointer to ApiClient instance for backend alert reporting
     */
    HazardDetector(uint8_t flamePin, uint8_t gasPin, uint8_t alarmBuzzerPin, uint8_t alarmLedPin, int gasThreshold, ApiClient* apiClient);

    /**
     * @brief Destructor
     */
    ~HazardDetector();

    /**
     * @brief Initializes GPIO pins
     */
    void begin();

    /**
     * @brief Reads flame sensor value
     * @return true if flame is detected, false otherwise
     */
    bool checkFlame();

    /**
     * @brief Reads analog gas sensor value
     * @return Raw analog sensor value
     */
    int readGasLevel();

    /**
     * @brief Main non-blocking routine to inspect sensors, trigger alarms, and dispatch API alerts.
     * Call inside main loop().
     */
    void update();

    /**
     * @brief Gets current hazard state
     * @return Current HazardStatus enum value
     */
    HazardStatus getStatus() const;
};

#endif // HAZARD_DETECTOR_H
