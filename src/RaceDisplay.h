#ifndef RACE_DISPLAY_H
#define RACE_DISPLAY_H

//
// ============================================================
//  RaceDisplay.h
//  Libreria principale del sistema RaceDisplay
//  Strutturata per repository Git (chiara, modulare, stabile)
// ============================================================
//

// ------------------------------------------------------------
//  Inclusione moduli interni
// ------------------------------------------------------------

#include "FlagSettings.h"      // Configurazione hardware e parametri
#include "FlagTypes.h"         // Enumerazioni bandiere e stati
#include "Colors.h"            // Definizione colori LED
#include "MatrixDriver.h"      // Driver matrici 8x8
#include "RingDriver.h"        // Driver anelli LED
#include "AnimationEngine.h"   // Motore animazioni non bloccanti
#include "FlagManager.h"       // Logica bandiere e routing per ID

// ------------------------------------------------------------
//  Abilitazione/disabilitazione pannelli LED
// ------------------------------------------------------------
#ifndef ENABLE_PANELS
#define ENABLE_PANELS true   // true = pannelli attivi, false = disattivati
#endif

#if ENABLE_PANELS
#pragma message("RaceDisplay: LED panels ENABLED")
#else
#pragma message("RaceDisplay: LED panels DISABLED")
#endif


// ------------------------------------------------------------
//  Classe principale RaceDisplay
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
            AnimationEngine::update();
            FlagManager::update();
        }
    #else
        inline void update() {
            // pannelli disattivati → nessun rendering
        }
    #endif



    // --------------------------------------------------------
    //  Gestione comandi esterni (seriale, radio, CAN, ecc.)
    // --------------------------------------------------------
    void handleCommand(const String& cmd) {
        FlagManager::handleCommand(cmd);
    }

};

#endif
