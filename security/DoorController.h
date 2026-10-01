#ifndef DOOR_CONTROLLER_H
#define DOOR_CONTROLLER_H

#include <Arduino.h>
#include "../network/ApiClient.h"

/**
 * @class DoorController
 * @brief Manages door locking mechanism, passcode input, and remote authentication.
 */
class DoorController {
private:
    uint8_t lockRelayPin;
    uint8_t buzzerPin;
    unsigned long unlockDurationMs;
    unsigned long unlockedTimestamp;
    bool isUnlocked;
    String enteredPasswordBuffer;
    ApiClient* apiClient;

    /**
     * @brief Plays audio beep feedback on buzzer
     * @param frequency Tone frequency in Hz
     * @param durationMs Beep duration in milliseconds
     */
    void playFeedbackTone(unsigned int frequency, unsigned long durationMs);

public:
    /**
     * @brief Constructor
     * @param lockRelayPin GPIO connected to electronic lock relay or servo
     * @param buzzerPin GPIO connected to status buzzer
     * @param unlockDurationMs How long to keep door unlocked before auto-relock (ms)
     * @param apiClient Pointer to ApiClient instance for backend authentication
     */
    DoorController(uint8_t lockRelayPin, uint8_t buzzerPin, unsigned long unlockDurationMs, ApiClient* apiClient);

    /**
     * @brief Destructor
     */
    ~DoorController();

    /**
     * @brief Initializes GPIO pins
     */
    void begin();

    /**
     * @brief Appends a key character to the password buffer
     * @param key Character pressed on keypad
     */
    void inputKey(char key);

    /**
     * @brief Clears current entered password buffer
     */
    void clearPasswordBuffer();

    /**
     * @brief Submits currently buffered password to backend API to unlock
     * @return true if access granted and door unlocked, false if denied
     */
    bool submitPassword();

    /**
     * @brief Directly unlocks the door
     */
    void unlock();

    /**
     * @brief Directly locks the door
     */
    void lock();

    /**
     * @brief Checks lock status
     * @return true if currently unlocked, false if locked
     */
    bool isDoorUnlocked() const;

    /**
     * @brief Non-blocking state handler to auto-relock door after duration expires.
     * Call inside main loop().
     */
    void update();
};

#endif // DOOR_CONTROLLER_H
