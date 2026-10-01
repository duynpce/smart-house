#ifndef SMART_HOUSE_SYSTEM_H
#define SMART_HOUSE_SYSTEM_H

#include "../config/Config.h"
#include "../network/WiFiManager.h"
#include "../network/ApiClient.h"
#include "../security/DoorController.h"
#include "../sensors/HazardDetector.h"
#include "../sensors/EnvironmentMonitor.h"
#include "../actuators/ApplianceController.h"

/**
 * @class SmartHouseSystem
 * @brief Master controller orchestrating all IoT smart house subsystems.
 */
class SmartHouseSystem {
private:
    AppConfig config;
    WiFiManager* wifiManager;
    ApiClient* apiClient;
    DoorController* doorController;
    HazardDetector* hazardDetector;
    EnvironmentMonitor* envMonitor;
    ApplianceController* applianceController;

public:
    /**
     * @brief Constructor
     * @param config Configuration parameters
     */
    SmartHouseSystem(const AppConfig& config);

    /**
     * @brief Destructor
     */
    ~SmartHouseSystem();

    /**
     * @brief Initialize all hardware modules, network, and sensors.
     * Call inside Arduino setup().
     */
    void begin();

    /**
     * @brief Main system loop executing all sub-module update tasks.
     * Call inside Arduino loop().
     */
    void update();

    // Accessors for subsystems
    WiFiManager* getWiFiManager() const;
    ApiClient* getApiClient() const;
    DoorController* getDoorController() const;
    HazardDetector* getHazardDetector() const;
    EnvironmentMonitor* getEnvironmentMonitor() const;
    ApplianceController* getApplianceController() const;
};

#endif // SMART_HOUSE_SYSTEM_H
