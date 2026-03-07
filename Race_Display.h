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
#include "colors.h"            // Definizione colori LED
#include "MatrixDriver.h"      // Driver matrici 8x8
#include "RingDriver.h"        // Driver anelli LED
#include "AnimationEngine.h"   // Motore animazioni non bloccanti
#include "FlagManager.h"       // Logica bandiere e routing per ID


// ------------------------------------------------------------
//  Classe principale RaceDisplay
// ------------------------------------------------------------

class RaceDisplay {

public:

    // --------------------------------------------------------
    //  Setup generale del dispositivo
    // --------------------------------------------------------
    void begin() {
        #if DEVICE_TYPE == DEVICE_TYPE_MATRIX
            MatrixSetup();
        #elif DEVICE_TYPE == DEVICE_TYPE_PIT
            PitRingSetup();
        #elif DEVICE_TYPE == DEVICE_TYPE_SEMAFORO
            SemaforoRingSetup();
        #endif
    }


    // --------------------------------------------------------
    //  Loop principale (non bloccante)
    // --------------------------------------------------------
    void update() {
        AnimationEngine::update();
        FlagManager::update();
    }


    // --------------------------------------------------------
    //  Gestione comandi esterni (seriale, radio, CAN, ecc.)
    // --------------------------------------------------------
    void handleCommand(const String& cmd) {
        FlagManager::handleCommand(cmd);
    }

};

#endif
