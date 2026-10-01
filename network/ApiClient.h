#ifndef API_CLIENT_H
#define API_CLIENT_H

#include <Arduino.h>

/**
 * @struct TelemetryData
 * @brief Environmental sensor readings payload
 */
struct TelemetryData {
    float temperature;
    float humidity;
    bool isRaining;
    int rainRawValue;
};

/**
 * @struct ApplianceState
 * @brief Status of light and fan
 */
struct ApplianceState {
    bool lightOn;
    bool fanOn;
};

/**
 * @class ApiClient
 * @brief Handles HTTP/REST API communications with the backend server.
 */
class ApiClient {
private:
    String baseUrl;
    String apiKey;

    /**
     * @brief Internal helper to execute HTTP POST requests
     * @param endpoint API relative endpoint
     * @param jsonPayload JSON formatted request body
     * @param response Reference to store server response body
     * @return HTTP status code (e.g. 200, 401, 500), or negative on network error
     */
    int httpPost(const String& endpoint, const String& jsonPayload, String& response);

    /**
     * @brief Internal helper to execute HTTP GET requests
     * @param endpoint API relative endpoint
     * @param response Reference to store server response body
     * @return HTTP status code
     */
    int httpGet(const String& endpoint, String& response);

public:
    /**
     * @brief Constructor
     * @param baseUrl Backend server root URL (e.g., https://api.myserver.com/v1)
     * @param apiKey Authentication API key header
     */
    ApiClient(const String& baseUrl, const String& apiKey);

    /**
     * @brief Destructor
     */
    ~ApiClient();

    /**
     * @brief Verifies door unlock password with backend server
     * @param inputPassword Entered PIN or password
     * @return true if password is valid and unlock is authorized
     */
    bool authenticateDoorUnlock(const String& inputPassword);

    /**
     * @brief Sends fire or gas incident alert to backend
     * @param hazardType Type of incident ("FIRE", "GAS", "FIRE_AND_GAS")
     * @param sensorValue Numerical sensor reading triggering the alarm
     * @param message Detailed incident description
     * @return true if alert was successfully received by backend
     */
    bool sendIncidentAlert(const String& hazardType, int sensorValue, const String& message);

    /**
     * @brief Sends temperature, humidity, and rain sensor data to backend
     * @param data Telemetry data struct
     * @return true if data was received by server
     */
    bool sendTelemetry(const TelemetryData& data);

    /**
     * @brief Fetches latest light and fan state commands from backend server
     * @param state Output parameter populated with remote state
     * @return true if state was retrieved successfully
     */
    bool fetchApplianceCommands(ApplianceState& state);

    /**
     * @brief Reports local light/fan state changes back to server
     * @param state Current state of appliances
     * @return true if synced successfully
     */
    bool syncApplianceState(const ApplianceState& state);
};

#endif // API_CLIENT_H
