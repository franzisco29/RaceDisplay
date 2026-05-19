#ifndef RACE_DISPLAY_H
#define RACE_DISPLAY_H

// ============================================================
//  RaceDisplay.h
//  Dispatcher principale della libreria RaceDisplay
// ============================================================

#include "FlagSettings.h"
#include "DeviceRuntime.h"

// ============================================================
//  CASO 1: NO_PANEL — stub vuoto, zero FastLED
// ============================================================
#ifdef NO_PANEL

#pragma message("[RaceDisplay] NO_PANEL — LED system completamente escluso")

class RaceDisplay {
public:
  void begin(bool enablePanels = false) {}
  void update() {}
  void handleCommand(const String& cmd) {}
};

// ============================================================
//  CASO 2: Modalità STATICA — DEVICE_TYPE noto a compile time
// ============================================================
#elif !defined(HC_RUNTIME_CONFIG)

  #if DEVICE_TYPE == DEVICE_TYPE_SENDER
    // Button box — solo RacePanel
    #include "RacePanel.h"
    #pragma message("RaceDisplay: DeviceSender mode statico")
    typedef RacePanel RaceDisplay;

  #else
    // Matrix, PIT, Semaforo
    #include "FlagTypes.h"
    #include "Colors.h"
    #include "MatrixDriver.h"
    #include "RingDriver.h"
    #include "AnimationEngine.h"
    #include "FlagManager.h"

    #ifndef ENABLE_PANELS
    #define ENABLE_PANELS true
    #endif

    #if ENABLE_PANELS
    #pragma message("RaceDisplay: LED panels ENABLED")
    #else
    #pragma message("RaceDisplay: LED panels DISABLED")
    #endif

    class RaceDisplay {
    public:
      void begin(bool enablePanels = true) {
        #if ENABLE_PANELS
          #if DEVICE_TYPE == DEVICE_TYPE_MATRIX
            MatrixSetup();
          #elif DEVICE_TYPE == DEVICE_TYPE_PIT
            PitRingSetup();
          #elif DEVICE_TYPE == DEVICE_TYPE_SEMAFORO
            SemaforoRingSetup();
          #endif
        #endif
      }

      #if ENABLE_PANELS
        void update() { FlagManager::update(); }
      #else
        void update() {}
      #endif

      void handleCommand(const String& cmd) {
        FlagManager::handleCommand(cmd);
      }
    };

  #endif // DEVICE_TYPE

// ============================================================
//  CASO 3: Modalità RUNTIME (HC_RUNTIME_CONFIG) 
//  DEVICE_TYPE determinato a runtime da RT_DEVICE_TYPE
// ============================================================
#else

#include "FlagTypes.h"
#include "Colors.h"
#include "MatrixDriver.h"
#include "RingDriver.h"
#include "AnimationEngine.h"
#include "FlagManager.h"
#include "RacePanel.h"

#pragma message("[RaceDisplay] Modalita RUNTIME")

class RaceDisplay {
public:

  void begin(bool enablePanels = true) {
    if (RT_DEVICE_TYPE == DEVICE_TYPE_SENDER) {
      _panel.begin();
      return;
    }
    if (!enablePanels) return;
    if (RT_DEVICE_TYPE == DEVICE_TYPE_MATRIX)
      MatrixSetup();
    else if (RT_DEVICE_TYPE == DEVICE_TYPE_PIT)
      PitRingSetup();
    else if (RT_DEVICE_TYPE == DEVICE_TYPE_SEMAFORO)
      SemaforoRingSetup();
  }

  void update() {
    if (RT_DEVICE_TYPE == DEVICE_TYPE_SENDER) {
      _panel.update();
      return;
    }
    FlagManager::update();
  }

  void handleCommand(const String& cmd) {
    if (RT_DEVICE_TYPE == DEVICE_TYPE_SENDER) return;
    FlagManager::handleCommand(cmd);
  }

  RacePanel& panel() { return _panel; }

private:
  RacePanel _panel;
};

#endif // fine blocchi

#endif // RACE_DISPLAY_H
