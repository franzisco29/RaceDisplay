#ifndef ANIMATION_ENGINE_H
#define ANIMATION_ENGINE_H

#include <FastLED.h>
#include "FlagSettings.h"
#include "FlagTypes.h"
#include "Colors.h"

// Driver functions (dichiarate altrove)
static void MatrixShowFlag(FlagType flag, bool toggle);
static void drawLetter(char letter, CRGB color);
static void SemaforoShowFlag(FlagType flag);
static void SemaforoShowLightsOut();
static void SemaforoShowFormationLap();


// ------------------------------------------------------------
//  Stato interno animazioni
// ------------------------------------------------------------
namespace AnimationEngine {

    static unsigned long lastUpdate = 0;
    static bool toggle = false;

    static FlagType activeFlag = FLAG_NONE;
    static SemaforoState activeSemState = SEM_NONE;

    static uint8_t scStep = 0;
    static uint8_t vscStep = 0;
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
//  Funzione di supporto: mostra gialla SOLO se settore corretto
// ------------------------------------------------------------
static bool MatrixShouldShowYellow(FlagType flag) {

    switch(flag) {

        case FLAG_YELLOW_S1:
            return (DEVICE_ID == 0);

        case FLAG_YELLOW_S2:
            return (DEVICE_ID == 1);

        case FLAG_YELLOW_S3:
            return (DEVICE_ID == 2);

        case FLAG_YELLOW_FS:   // S1 + S2
            return (DEVICE_ID == 0 || DEVICE_ID == 1);

        case FLAG_YELLOW_ST:   // S2 + S3
            return (DEVICE_ID == 1 || DEVICE_ID == 2);

        case FLAG_YELLOW_TF:   // S3 + S1
            return (DEVICE_ID == 2 || DEVICE_ID == 0);

        default:
            return false;
    }
}


// ------------------------------------------------------------
//  Aggiornamento animazioni (non bloccante)
// ------------------------------------------------------------
static void AnimationUpdate() {

    unsigned long now = millis();

    if (AnimationEngine::activeFlag == FLAG_NONE &&
        AnimationEngine::activeSemState == SEM_NONE)
        return;


    // --------------------------------------------------------
    //  Animazioni bandiere
    // --------------------------------------------------------
    switch(AnimationEngine::activeFlag) {


        // ----------------------------------------------------
        //  Yellow (300 ms) — con logica settore corretta
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

                if (DEVICE_TYPE == DEVICE_TYPE_SEMAFORO) {
                    SemaforoShowFlag(AnimationEngine::toggle ? AnimationEngine::activeFlag : FLAG_NONE);
                }
                else if (DEVICE_TYPE == DEVICE_TYPE_MATRIX) {

                    if (MatrixShouldShowYellow(AnimationEngine::activeFlag)) {
                        MatrixShowFlag(AnimationEngine::toggle ? AnimationEngine::activeFlag : FLAG_NONE, false);
                    } else {
                        MatrixShowFlag(FLAG_NONE, false);
                    }
                }
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
                    MatrixShowFlag(AnimationEngine::toggle ? FLAG_RED : FLAG_NONE, false);
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
                    MatrixShowFlag(AnimationEngine::toggle ? FLAG_BLUE : FLAG_NONE, false);
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
                    MatrixShowFlag(AnimationEngine::toggle ? FLAG_WET : FLAG_NONE, false);
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
                    MatrixShowFlag(FLAG_CHECKERED, AnimationEngine::toggle);
            }
            break;



        // ----------------------------------------------------
        //  SC (S ↔ C)
        // ----------------------------------------------------
        case FLAG_SC:

            if (now - AnimationEngine::lastUpdate >= 500) {
                AnimationEngine::lastUpdate = now;
                AnimationEngine::toggle = !AnimationEngine::toggle;

                if (DEVICE_TYPE == DEVICE_TYPE_MATRIX) {

                    MatrixShowFlag(FLAG_SC, AnimationEngine::toggle);
                    FastLED.show();
                }
                else {
                    SemaforoShowFlag(FLAG_SC);
                }
            }
            break;






        // ----------------------------------------------------
        //  VSC (V → S → C)
        // ----------------------------------------------------
        case FLAG_VSC:

            if (now - AnimationEngine::lastUpdate >= 500) {
                AnimationEngine::lastUpdate = now;
                AnimationEngine::toggle = !AnimationEngine::toggle;

                if (DEVICE_TYPE == DEVICE_TYPE_MATRIX) {

                    MatrixShowFlag(FLAG_VSC, AnimationEngine::toggle);
                    FastLED.show();
                }
                else {
                    SemaforoShowFlag(FLAG_VSC);
                }
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
            break;

        case SEM_LIGHTS_OUT:
            SemaforoShowLightsOut();
            break;

        case SEM_FORMATION_LAP:

            if (DEVICE_TYPE == DEVICE_TYPE_SEMAFORO) {
                // Semaforo: animazione dedicata
                SemaforoShowFormationLap();
            }
            else if (DEVICE_TYPE == DEVICE_TYPE_MATRIX) {

                // Determina la gialla corretta per il settore
                FlagType sectorYellow =
                    (DEVICE_ID == 0) ? FLAG_YELLOW_S1 :
                    (DEVICE_ID == 1) ? FLAG_YELLOW_S2 :
                                    FLAG_YELLOW_S3;

                // Lampeggio gialla settore-corretta
                if (now - AnimationEngine::lastUpdate >= BLINK_YELLOW_MS) {
                    AnimationEngine::lastUpdate = now;
                    AnimationEngine::toggle = !AnimationEngine::toggle;

                    MatrixShowFlag(AnimationEngine::toggle ? sectorYellow : FLAG_NONE, false);
                }
            }
            break;


        default:
            break;
    }
    
}


// API pubblica
namespace AnimationEngine {
    static inline void start(FlagType flag) { AnimationStart(flag); }
    static inline void startSem(SemaforoState state) { AnimationStartSem(state); }
    static inline void update() { AnimationUpdate(); }
}

#endif
