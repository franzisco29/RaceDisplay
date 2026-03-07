#ifndef ANIMATION_ENGINE_H
#define ANIMATION_ENGINE_H

//
// ============================================================
//  AnimationEngine.h
//  Motore animazioni non bloccanti per RaceDisplay
//  Strutturato per repository Git (chiaro, modulare, stabile)
// ============================================================
//
//  Questo file gestisce:
//   - Lampeggi (Yellow, Red, Blue, Wet)
//   - Sequenze SC (S↔C) e VSC (V→S→C)
//   - Sequenze semaforo (1→5, lights out)
//   - Checkered (dispari ↔ pari)
//   - Aggiornamento non bloccante via millis()
//   - Nessun delay() in tutto il sistema
//
//  Non interpreta bandiere: esegue solo animazioni richieste
//  da FlagManager.
//
// ============================================================
//

#include <FastLED.h>
#include "FlagSettings.h"
#include "FlagTypes.h"
#include "colors.h"
#include "MatrixDriver.h"
#include "RingDriver.h"



// ------------------------------------------------------------
//  Stato interno animazioni
// ------------------------------------------------------------

namespace AnimationEngine {

    static unsigned long lastUpdate = 0;
    static bool toggle = false;

    static FlagType activeFlag = FLAG_NONE;
    static SemaforoState activeSemState = SEM_NONE;

    static uint8_t scStep = 0;     // S ↔ C
    static uint8_t vscStep = 0;    // V → S → C
}



// ------------------------------------------------------------
//  Avvia animazione bandiera
// ------------------------------------------------------------

static void AnimationStart(FlagType flag) {
    AnimationEngine::activeFlag = flag;
    AnimationEngine::scStep = 0;
    AnimationEngine::vscStep = 0;
}



// ------------------------------------------------------------
//  Avvia animazione semaforo
// ------------------------------------------------------------

static void AnimationStartSem(SemaforoState state) {
    AnimationEngine::activeSemState = state;
}



// ------------------------------------------------------------
//  Aggiornamento animazioni (non bloccante)
// ------------------------------------------------------------

static void AnimationUpdate() {

    unsigned long now = millis();

    // Nessuna animazione attiva
    if (AnimationEngine::activeFlag == FLAG_NONE &&
        AnimationEngine::activeSemState == SEM_NONE)
        return;


    // --------------------------------------------------------
    //  Animazioni bandiere
    // --------------------------------------------------------

    switch(AnimationEngine::activeFlag) {


        // ----------------------------------------------------
        //  Yellow (300 ms)
        // ----------------------------------------------------
        case FLAG_YELLOW_S1:
        case FLAG_YELLOW_S2:
        case FLAG_YELLOW_S3:
        case FLAG_YELLOW_FS:
        case FLAG_YELLOW_ST:
        case FLAG_YELLOW_TF:

            if (now - AnimationEngine::lastUpdate >= BLINK_YELLOW_MS) {
                AnimationEngine::lastUpdate = now;
                AnimationEngine::toggle = !AnimationEngine::toggle;

                if (DEVICE_TYPE == DEVICE_TYPE_SEMAFORO)
                    SemaforoShowFlag(AnimationEngine::toggle ? AnimationEngine::activeFlag : FLAG_NONE);
                else
                    MatrixShowFlag(AnimationEngine::toggle ? AnimationEngine::activeFlag : FLAG_NONE);
            }
            break;



        // ----------------------------------------------------
        //  Red flag (500 ms)
        // ----------------------------------------------------
        case FLAG_RED:

            if (now - AnimationEngine::lastUpdate >= BLINK_RED_MS) {
                AnimationEngine::lastUpdate = now;
                AnimationEngine::toggle = !AnimationEngine::toggle;

                if (DEVICE_TYPE == DEVICE_TYPE_SEMAFORO)
                    SemaforoShowFlag(AnimationEngine::toggle ? FLAG_RED : FLAG_NONE);
                else
                    MatrixShowFlag(AnimationEngine::toggle ? FLAG_RED : FLAG_NONE);
            }
            break;



        // ----------------------------------------------------
        //  Blue flag (500 ms)
        // ----------------------------------------------------
        case FLAG_BLUE:

            if (now - AnimationEngine::lastUpdate >= BLINK_BLUE_MS) {
                AnimationEngine::lastUpdate = now;
                AnimationEngine::toggle = !AnimationEngine::toggle;

                if (DEVICE_TYPE == DEVICE_TYPE_SEMAFORO)
                    SemaforoShowFlag(AnimationEngine::toggle ? FLAG_BLUE : FLAG_NONE);
                else
                    MatrixShowFlag(AnimationEngine::toggle ? FLAG_BLUE : FLAG_NONE);
            }
            break;



        // ----------------------------------------------------
        //  Wet Race (500 ms)
        // ----------------------------------------------------
        case FLAG_WET:

            if (now - AnimationEngine::lastUpdate >= BLINK_WET_MS) {
                AnimationEngine::lastUpdate = now;
                AnimationEngine::toggle = !AnimationEngine::toggle;

                if (DEVICE_TYPE == DEVICE_TYPE_SEMAFORO)
                    SemaforoShowFlag(AnimationEngine::toggle ? FLAG_WET : FLAG_NONE);
                else
                    MatrixShowFlag(AnimationEngine::toggle ? FLAG_WET : FLAG_NONE);
            }
            break;



        // ----------------------------------------------------
        //  Checkered (500 ms)
        // ----------------------------------------------------
        case FLAG_CHECKERED:

            if (now - AnimationEngine::lastUpdate >= BLINK_CHECK_MS) {
                AnimationEngine::lastUpdate = now;
                AnimationEngine::toggle = !AnimationEngine::toggle;

                if (DEVICE_TYPE == DEVICE_TYPE_SEMAFORO)
                    SemaforoShowFlag(FLAG_CHECKERED);
                else
                    MatrixShowFlag(FLAG_CHECKERED);
            }
            break;



        // ----------------------------------------------------
        //  SC (S ↔ C)
        // ----------------------------------------------------
        case FLAG_SC:

            if (now - AnimationEngine::lastUpdate >= BLINK_SC_MS) {
                AnimationEngine::lastUpdate = now;

                AnimationEngine::scStep ^= 1; // 0→1→0→1…

                if (DEVICE_TYPE == DEVICE_TYPE_SEMAFORO)
                    SemaforoShowFlag(FLAG_SC);
                else
                    drawLetter(AnimationEngine::scStep ? 'S' : 'C', COLOR_YELLOW);

                FastLED.show();
            }
            break;



        // ----------------------------------------------------
        //  VSC (V → S → C)
        // ----------------------------------------------------
        case FLAG_VSC:

            if (now - AnimationEngine::lastUpdate >= BLINK_VSC_MS) {
                AnimationEngine::lastUpdate = now;

                AnimationEngine::vscStep = (AnimationEngine::vscStep + 1) % 3;

                char seq = (AnimationEngine::vscStep == 0) ? 'V' :
                           (AnimationEngine::vscStep == 1) ? 'S' : 'C';

                if (DEVICE_TYPE != DEVICE_TYPE_SEMAFORO)
                    drawLetter(seq, COLOR_YELLOW);

                FastLED.show();
            }
            break;



        default:
            break;
    }



    // --------------------------------------------------------
    //  Animazioni semaforo
    // --------------------------------------------------------

    switch(AnimationEngine::activeSemState) {

        case SEM_START_SEQUENCE:
            // Gestito da FlagManager (sequenza 1→5)
            break;

        case SEM_LIGHTS_OUT:
            SemaforoShowLightsOut();
            break;

        case SEM_FORMATION_LAP:
            SemaforoShowFormationLap();
            break;

        default:
            break;
    }
}



#endif
