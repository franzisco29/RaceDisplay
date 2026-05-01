#ifndef RACE_DISPLAY_H
#define RACE_DISPLAY_H

// ============================================================
//  RaceDisplay.h
//  Dispatcher principale della libreria RaceDisplay
//  Se il device è LED → include e definisce RaceDisplay LED
//  Se il device è Sender → include RacePanel e usa alias
// ============================================================

#include "FlagSettings.h"   // contiene DEVICE_TYPE
#include "DeviceRuntime.h"

// ------------------------------------------------------------
//  NO_PANEL — esclude tutta la compilazione LED
//  Definisci nel main.ino prima degli include per device
//  che non hanno mai LED (es. cronometro puro)
// ------------------------------------------------------------
#ifdef NO_PANEL

#pragma message("[RaceDisplay] NO_PANEL definito: LED system completamente escluso")
  #include "Commands.h" // solo per definire handleCommand
class RaceDisplay {
public:
  void begin(bool enablePanels = false) {}
  void update() {}
  void handleCommand(const String& cmd) {}
};

#else // NO_PANEL non definito — comportamento normale

// ------------------------------------------------------------
//  Caso speciale: DeviceSender → includi solo RacePanel
// ------------------------------------------------------------
  #pragma message("[RaceDisplay] NO_PANEL non definito: comportamento completo (LED system incluso)")   
#if DEVICE_TYPE == DEVICE_TYPE_SENDER

#include "RacePanel.h"
#pragma message("RaceDisplay: DeviceSender mode (RacePanel enabled, LED system disabled)")

// Alias per mantenere API identica
typedef RacePanel RaceDisplay;

#else

// ------------------------------------------------------------
//  Inclusione moduli LED (solo per MATRIX, PIT, SEMAFORO)
// ------------------------------------------------------------
#include "FlagTypes.h"
#include "Colors.h"
#include "MatrixDriver.h"
#include "RingDriver.h"
#include "AnimationEngine.h"
#include "FlagManager.h"

// ------------------------------------------------------------
//  Abilitazione/disabilitazione pannelli LED
// ------------------------------------------------------------
#ifndef HC_RUNTIME_CONFIG
  #ifndef ENABLE_PANELS
  #define ENABLE_PANELS true
  #endif

  #if ENABLE_PANELS
  #pragma message("RaceDisplay: LED panels ENABLED")
  #else
  #pragma message("RaceDisplay: LED panels DISABLED")
  #endif
#endif


// ------------------------------------------------------------
//  Classe RaceDisplay (solo dispositivi LED)
// ------------------------------------------------------------
class RaceDisplay {

public:

    // --------------------------------------------------------
    //  Setup generale del dispositivo
    // --------------------------------------------------------
    void begin(bool enablePanels = true) {

    #ifndef HC_RUNTIME_CONFIG
        // Modalità statica — compile time
        #if ENABLE_PANELS
            #if DEVICE_TYPE == DEVICE_TYPE_MATRIX
                MatrixSetup();
            #elif DEVICE_TYPE == DEVICE_TYPE_PIT
                PitRingSetup();
            #elif DEVICE_TYPE == DEVICE_TYPE_SEMAFORO
                SemaforoRingSetup();
            #endif
        #endif
    #else
        // Modalità runtime — legge RT_DEVICE_TYPE e enablePanels
        if (!enablePanels) return;
        if (RT_DEVICE_TYPE == DEVICE_TYPE_MATRIX)
            MatrixSetup();
        else if (RT_DEVICE_TYPE == DEVICE_TYPE_PIT)
            PitRingSetup();
        else if (RT_DEVICE_TYPE == DEVICE_TYPE_SEMAFORO)
            SemaforoRingSetup();
    #endif

    }

    // --------------------------------------------------------
    //  Loop principale (non bloccante)
    // --------------------------------------------------------
    #ifndef HC_RUNTIME_CONFIG
        #if ENABLE_PANELS
            void update() {
                FlagManager::update();
            }
        #else
            inline void update() {
                // pannelli disattivati → nessun rendering
            }
        #endif
    #else
        void update() {
            FlagManager::update();
        }
    #endif

    // --------------------------------------------------------
    //  Gestione comandi esterni
    // --------------------------------------------------------
    void handleCommand(const String& cmd) {
        FlagManager::handleCommand(cmd);
    }

};

#endif // DEVICE_TYPE != DEVICE_TYPE_SENDER

#endif // NO_PANEL non definito

#endif // RACE_DISPLAY_H