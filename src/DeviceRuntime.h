#ifndef DEVICE_RUNTIME_H
#define DEVICE_RUNTIME_H

/* =========================================================
   DeviceRuntime.h
   Variabili globali runtime per RaceDisplay.

   Popolato da DeviceConfig::begin() al boot.
   Letto da MatrixDriver, RingDriver, AnimationEngine, FlagManager.

   In modalità statica (HC_RUNTIME_CONFIG NON definito),
   i valori vengono inizializzati dai #define di FlagSettings.h
   e il comportamento è identico a prima.
   ========================================================= */

#include "FlagSettings.h"

namespace DeviceRuntime {

  #ifdef HC_RUNTIME_CONFIG
    // Valori runtime — popolati da DeviceConfig::begin()
    inline uint8_t deviceID     = 0;
    inline uint8_t deviceType   = DEVICE_TYPE_MATRIX;
    inline uint8_t brightness   = 200;
    inline uint8_t rotation     = 0;
    inline bool    reverseOrder = false;
  #else
    // Valori statici — inizializzati dai #define classici
    inline uint8_t deviceID     = DEVICE_ID;
    inline uint8_t deviceType   = DEVICE_TYPE;
    inline uint8_t brightness   = LED_BRIGHTNESS;
    inline uint8_t rotation     = MATRIX_ROTATION;
    inline bool    reverseOrder = false;
  #endif

}

// ------------------------------------------------------------
//  Shorthand usati dai moduli
//  Sostituiscono DEVICE_ID, DEVICE_TYPE, ecc. ovunque
// ------------------------------------------------------------
#define RT_DEVICE_ID    DeviceRuntime::deviceID
#define RT_DEVICE_TYPE  DeviceRuntime::deviceType
#define RT_BRIGHTNESS   DeviceRuntime::brightness
#define RT_ROTATION     DeviceRuntime::rotation
#define RT_REVERSE      DeviceRuntime::reverseOrder

// ------------------------------------------------------------
//  Helper inline — evitano di scrivere confronti ovunque
// ------------------------------------------------------------
inline bool isMatrix()   { return DeviceRuntime::deviceType == DEVICE_TYPE_MATRIX;   }
inline bool isPit()      { return DeviceRuntime::deviceType == DEVICE_TYPE_PIT;      }
inline bool isSemaforo() { return DeviceRuntime::deviceType == DEVICE_TYPE_SEMAFORO; }
inline bool isSender()   { return DeviceRuntime::deviceType == DEVICE_TYPE_SENDER;   }

#endif