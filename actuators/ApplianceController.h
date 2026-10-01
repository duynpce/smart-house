#ifndef APPLIANCE_CONTROLLER_H
#define APPLIANCE_CONTROLLER_H

#include <Arduino.h>
#include "../network/ApiClient.h"

/**
 * @class ApplianceController
 * @brief Controls physical home appliances (Light, Fan) and synchronizes state with backend server.
 */
class ApplianceController {
private:
    uint8_t lightPin;
    uint8_t fanPin;
    bool isLightOn;
    bool isFanOn;
    
    unsigned long pollIntervalMs;
    unsigned long lastPollTime;
    
    ApiClient* apiClient;

    /**
     * @brief Writes digital pin state according to relay logic (active LOW/HIGH)
     * @param pin Relay pin
     * @param state true for ON, false for OFF
     */
    void applyRelayState(uint8_t pin, bool state);

public:
    /**
     * @brief Constructor
     * @param lightPin GPIO connected to Light relay
     * @param fanPin GPIO connected to Fan relay
     * @param pollIntervalMs Interval to fetch remote control commands from backend
     * @param apiClient Pointer to ApiClient instance for syncing state
     */
    ApplianceController(uint8_t lightPin, uint8_t fanPin, unsigned long pollIntervalMs, ApiClient* apiClient);

    /**
     * @brief Destructor
     */
    ~ApplianceController();

    /**
     * @brief Initializes GPIO pins and sets default OFF state
     */
    void begin();

    /**
     * @brief Turns Light ON or OFF
     * @param state true for ON, false for OFF
     */
    void setLight(bool state);

    /**
     * @brief Turns Fan ON or OFF
     * @param state true for ON, false for OFF
     */
    void setFan(bool state);

    /**
     * @brief Toggles current light state
     */
    void toggleLight();

    /**
     * @brief Toggles current fan state
     */
    void toggleFan();

    /**
     * @brief Gets current light status
     */
    bool getLightState() const;

    /**
     * @brief Gets current fan status
     */
    bool getFanState() const;

    /**
     * @brief Gets combined appliance state struct
     */
    ApplianceState getState() const;

    /**
     * @brief Applies full state from server
     * @param state Desired appliance state
     */
    void applyState(const ApplianceState& state);

    /**
     * @brief Periodic update routine to poll remote commands from backend server.
     * Call inside main loop().
     */
    void update();
};

#endif // APPLIANCE_CONTROLLER_H
