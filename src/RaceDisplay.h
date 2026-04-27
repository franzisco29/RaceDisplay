#ifndef RACE_DISPLAY_H
#define RACE_DISPLAY_H

// ============================================================
//  RaceDisplay.h
//  Dispatcher principale della libreria RaceDisplay
//  Se il device è LED → include e definisce RaceDisplay LED
//  Se il device è Sender → include RacePanel e usa alias
// ============================================================

#include "FlagSettings.h"   // contiene DEVICE_TYPE

// ------------------------------------------------------------
//  Caso speciale: DeviceSender → includi solo RacePanel
// ------------------------------------------------------------
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
#ifndef ENABLE_PANELS
#define ENABLE_PANELS true
#endif

#if ENABLE_PANELS
#pragma message("RaceDisplay: LED panels ENABLED")
#else
#pragma message("RaceDisplay: LED panels DISABLED")
#endif


// ------------------------------------------------------------
//  Classe RaceDisplay (solo dispositivi LED)
// ------------------------------------------------------------
class RaceDisplay {

public:

    // --------------------------------------------------------
    //  Setup generale del dispositivo
    // --------------------------------------------------------
    void begin() {

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

    // --------------------------------------------------------
    //  Loop principale (non bloccante)
    // --------------------------------------------------------
    #if ENABLE_PANELS
        void update() {
            FlagManager::update();
        }
    #else
        inline void update() {
            // pannelli disattivati → nessun rendering
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

#endif // RACE_DISPLAY_H