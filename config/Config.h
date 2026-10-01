#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

/**
 * @file Config.h
 * @brief Unified configuration header supporting both Local and Production/Docker compilation.
 */

// If PROD_ENV or build macros are defined via PlatformIO / Docker build_flags
#if defined(PROD_BUILD) || defined(API_KEY) || defined(WIFI_SSID)
    #include "Config.h.prod"
    typedef AppConfigProd AppConfig;
#else
    #include "Config.h.local"
    typedef AppConfigLocal AppConfig;
#endif

#endif // CONFIG_H
