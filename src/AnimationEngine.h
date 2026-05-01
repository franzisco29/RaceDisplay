#ifndef ANIMATION_ENGINE_H
#define ANIMATION_ENGINE_H

#include <FastLED.h>
#include "FlagSettings.h"
#include "FlagTypes.h"
#include "Colors.h"
#include "DeviceRuntime.h"



// Driver functions (dichiarate altrove)
static void MatrixShowFlag(FlagType flag, bool toggle);
static void drawLetter(char letter, CRGB color);
static void SemaforoShowFlag(FlagType flag, bool toggle);
static void SemaforoShowLightsOut();
static void SemaforoShowFormationLap();


// ------------------------------------------------------------
//  Stato interno animazioni
// ------------------------------------------------------------
namespace AnimationEngine {

    static unsigned long lastUpdate = 0;
    static bool toggle = false;
    static unsigned long vscToggle = 0;

    static FlagType activeFlag = FLAG_NONE;
    static SemaforoState activeSemState = SEM_NONE;

    static uint8_t scStep = 0;
    static uint8_t vscStep = 0;

    // Stato sequenza semaforo auto (SA)
    static bool semStartDelayActive = false;
    static unsigned long semStartDelayEnd = 0;

    // GREEN FLAG temporizzata
    static bool greenActive = false;
    static unsigned long greenStartTime = 0;
}


// ------------------------------------------------------------
//  Avvia animazione bandiera
// ------------------------------------------------------------
static void AnimationStart(FlagType flag) {

    // GREEN FLAG → gestita qui (globale)
    if (flag == FLAG_GREEN ||
        flag == FLAG_GREEN_S1 ||
        flag == FLAG_GREEN_S2 ||
        flag == FLAG_GREEN_S3 ||
        flag == FLAG_GREEN_FS ||
        flag == FLAG_GREEN_ST ||
        flag == FLAG_GREEN_TF) {

        AnimationEngine::greenActive = true;
        AnimationEngine::greenStartTime = millis();
        AnimationEngine::activeFlag = flag;

        if (RT_DEVICE_TYPE == DEVICE_TYPE_MATRIX) {
            MatrixShowFlag(flag, false);
        } 
        else if (RT_DEVICE_TYPE == DEVICE_TYPE_SEMAFORO) {
            // Solo la green globale va al semaforo
            if (flag == FLAG_GREEN)
                SemaforoShowFlag(FLAG_GREEN);
        }

        FastLED.show();
        return;
    }


    // Se arriva un'altra bandiera → interrompi green
    AnimationEngine::greenActive = false;

    AnimationEngine::activeFlag = flag;
    AnimationEngine::scStep = 0;
    AnimationEngine::vscStep = 0;
}


// ------------------------------------------------------------
//  Avvia animazione semaforo
// ------------------------------------------------------------
static void AnimationStartSem(SemaforoState state) {
    AnimationEngine::activeSemState = state;

    // Reset robusto per consentire riavvii multipli della sequenza automatica
    if (state == SEM_START_SEQUENCE) {
        AnimationEngine::scStep = 0;
        AnimationEngine::semStartDelayActive = false;
        AnimationEngine::semStartDelayEnd = 0;
    }
}


// ------------------------------------------------------------
//  Funzioni di supporto settoriali
// ------------------------------------------------------------
static bool MatrixShouldShowYellow(FlagType flag) {

    switch(flag) {

        case FLAG_YELLOW_S1: return (RT_DEVICE_ID == 0);
        case FLAG_YELLOW_S2: return (RT_DEVICE_ID == 1);
        case FLAG_YELLOW_S3: return (RT_DEVICE_ID == 2);

        case FLAG_YELLOW_FS: return (RT_DEVICE_ID == 0 || RT_DEVICE_ID == 1);
        case FLAG_YELLOW_ST: return (RT_DEVICE_ID == 1 || RT_DEVICE_ID == 2);
        case FLAG_YELLOW_TF: return (RT_DEVICE_ID == 2 || RT_DEVICE_ID == 0);

        default: return false;
    }
}

static bool MatrixShouldShowBlue(FlagType flag) {

    switch(flag) {

        case FLAG_BLUE_S1: return (RT_DEVICE_ID == 0);
        case FLAG_BLUE_S2: return (RT_DEVICE_ID == 1);
        case FLAG_BLUE_S3: return (RT_DEVICE_ID == 2);

        default: return false;
    }
}

static bool MatrixShouldShowGreen(FlagType flag) {

    switch(flag) {

        case FLAG_GREEN_S1: return (RT_DEVICE_ID == 0);
        case FLAG_GREEN_S2: return (RT_DEVICE_ID == 1);
        case FLAG_GREEN_S3: return (RT_DEVICE_ID == 2);

        case FLAG_GREEN_FS: return (RT_DEVICE_ID == 0 || RT_DEVICE_ID == 1); // S1 + S2
        case FLAG_GREEN_ST: return (RT_DEVICE_ID == 1 || RT_DEVICE_ID == 2); // S2 + S3
        case FLAG_GREEN_TF: return (RT_DEVICE_ID == 2 || RT_DEVICE_ID == 0); // S3 + S1

        default: return false;
    }
}


// ------------------------------------------------------------
//  Aggiornamento animazioni (non bloccante)
// ------------------------------------------------------------
static void AnimationUpdate() {

    unsigned long now = millis();

    // --------------------------------------------------------
    //  GREEN FLAG temporizzata (3 secondi)
    // --------------------------------------------------------
    if (AnimationEngine::greenActive) {

        // La green temporizzata vale per TUTTI i verdi (globali, settoriali, doppi)
        if (!(AnimationEngine::activeFlag == FLAG_GREEN     ||
            AnimationEngine::activeFlag == FLAG_GREEN_S1  ||
            AnimationEngine::activeFlag == FLAG_GREEN_S2  ||
            AnimationEngine::activeFlag == FLAG_GREEN_S3  ||
            AnimationEngine::activeFlag == FLAG_GREEN_FS  ||
            AnimationEngine::activeFlag == FLAG_GREEN_ST  ||
            AnimationEngine::activeFlag == FLAG_GREEN_TF))
        {
            AnimationEngine::greenActive = false;
            return;
        }

        // Spegnimento dopo 3 secondi
        if (now - AnimationEngine::greenStartTime >= 3000) {

            AnimationEngine::greenActive = false;
            AnimationEngine::activeFlag = FLAG_NONE;

            if (RT_DEVICE_TYPE == DEVICE_TYPE_MATRIX) {
                MatrixClear();
            }
            else if (RT_DEVICE_TYPE == DEVICE_TYPE_SEMAFORO) {
                SemaforoShowLightsOut();
            }

            FastLED.show();
        }

        return;
    }




    // --------------------------------------------------------
    //  Se non c'è nulla da animare
    // --------------------------------------------------------
    if (AnimationEngine::activeFlag == FLAG_NONE &&
        AnimationEngine::activeSemState == SEM_NONE)
        return;


    // --------------------------------------------------------
    //  Animazioni bandiere
    // --------------------------------------------------------
    switch(AnimationEngine::activeFlag) {

        // ----------------------------------------------------
        //  YELLOW SETTORIALI
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

                if (RT_DEVICE_TYPE == DEVICE_TYPE_SEMAFORO) {
                    SemaforoShowFlag(AnimationEngine::toggle ? AnimationEngine::activeFlag : FLAG_NONE);
                }
                else if (RT_DEVICE_TYPE == DEVICE_TYPE_MATRIX) {

                    if (MatrixShouldShowYellow(AnimationEngine::activeFlag)) {
                        MatrixShowFlag(AnimationEngine::toggle ? AnimationEngine::activeFlag : FLAG_NONE, false);
                    } else {
                        MatrixShowFlag(FLAG_NONE, false);
                    }
                }
            }
            break;


        // ----------------------------------------------------
        //  GREEN SETTORIALI (singoli + doppi)
        // ----------------------------------------------------
        case FLAG_GREEN_S1:
        case FLAG_GREEN_S2:
        case FLAG_GREEN_S3:
        case FLAG_GREEN_FS:
        case FLAG_GREEN_ST:
        case FLAG_GREEN_TF:

            if (RT_DEVICE_TYPE == DEVICE_TYPE_MATRIX) {
                if (MatrixShouldShowGreen(AnimationEngine::activeFlag)) {
                    MatrixShowFlag(AnimationEngine::activeFlag, false);
                } else {
                    MatrixShowFlag(FLAG_NONE, false);
                }
            }
            break;



        // ----------------------------------------------------
        //  RED
        // ----------------------------------------------------
        case FLAG_RED:
            if (now - AnimationEngine::lastUpdate >= BLINK_RED_MS) {
                AnimationEngine::lastUpdate = now;
                AnimationEngine::toggle = !AnimationEngine::toggle;

                if (RT_DEVICE_TYPE == DEVICE_TYPE_SEMAFORO)
                    SemaforoShowFlag(FLAG_RED, AnimationEngine::toggle);
                else
                    MatrixShowFlag(AnimationEngine::toggle ? FLAG_RED : FLAG_NONE, false);
            }
            break;


        // ----------------------------------------------------
        //  BLUE (solo matrici)
        // ----------------------------------------------------
        case FLAG_BLUE:
        case FLAG_BLUE_S1:
        case FLAG_BLUE_S2:
        case FLAG_BLUE_S3:

            // Le bandiere blu sono ESCLUSIVE delle matrici
            if (RT_DEVICE_TYPE != DEVICE_TYPE_MATRIX) {
                break;   // Non fare nulla
            }

            // Solo le matrici arrivano qui
            if (now - AnimationEngine::lastUpdate >= BLINK_BLUE_MS) {

                AnimationEngine::lastUpdate = now;
                AnimationEngine::toggle = !AnimationEngine::toggle;

                // Blue globale
                if (AnimationEngine::activeFlag == FLAG_BLUE) {
                    MatrixShowFlag(AnimationEngine::toggle ? FLAG_BLUE : FLAG_NONE, false);
                }
                // Blue settoriale
                else if (MatrixShouldShowBlue(AnimationEngine::activeFlag)) {
                    MatrixShowFlag(AnimationEngine::toggle ? AnimationEngine::activeFlag : FLAG_NONE, false);
                }
                else {
                    MatrixShowFlag(FLAG_NONE, false);
                }
            }
            break;



        // ----------------------------------------------------
        //  WET
        // ----------------------------------------------------
        case FLAG_WET:
            if (now - AnimationEngine::lastUpdate >= BLINK_WET_MS) {
                AnimationEngine::lastUpdate = now;
                AnimationEngine::toggle = !AnimationEngine::toggle;

                if (RT_DEVICE_TYPE == DEVICE_TYPE_SEMAFORO)
                    SemaforoShowFlag(FLAG_WET, AnimationEngine::toggle);
                else
                    MatrixShowFlag(AnimationEngine::toggle ? FLAG_WET : FLAG_NONE, false);
            }
            break;

        case FLAG_DRY:
            if (RT_DEVICE_TYPE == DEVICE_TYPE_SEMAFORO) {
                SemaforoShowFlag(FLAG_DRY, false);
            }
            else {
                MatrixShowFlag(FLAG_NONE, false);
            }
            break;


        // ----------------------------------------------------
        //  CHECKERED
        // ----------------------------------------------------
        case FLAG_CHECKERED:
            if (now - AnimationEngine::lastUpdate >= BLINK_CHECK_MS) {
                AnimationEngine::lastUpdate = now;
                AnimationEngine::toggle = !AnimationEngine::toggle;

                if (RT_DEVICE_TYPE == DEVICE_TYPE_SEMAFORO)
                    SemaforoShowFlag(FLAG_CHECKERED, AnimationEngine::toggle);
                else
                    MatrixShowFlag(FLAG_CHECKERED, AnimationEngine::toggle);
            }
            break;


        // ----------------------------------------------------
        //  SAFETY CAR
        // ----------------------------------------------------
        case FLAG_SC:
            if (now - AnimationEngine::lastUpdate >= 500) {
                AnimationEngine::lastUpdate = now;
                AnimationEngine::toggle = !AnimationEngine::toggle;

                if (RT_DEVICE_TYPE == DEVICE_TYPE_MATRIX) {
                    MatrixShowFlag(FLAG_SC, AnimationEngine::toggle);
                    FastLED.show();
                }
                else {
                    SemaforoShowFlag(FLAG_SC, AnimationEngine::toggle);
                    FastLED.show();
                }
            }
            break;


        // ----------------------------------------------------
        //  VIRTUAL SAFETY CAR
        // ----------------------------------------------------
        case FLAG_VSC:
            if (now - AnimationEngine::lastUpdate >= 500) {
                AnimationEngine::lastUpdate = now;
                AnimationEngine::toggle = !AnimationEngine::toggle;

                if (RT_DEVICE_TYPE == DEVICE_TYPE_MATRIX) {
                    MatrixShowFlag(FLAG_VSC, AnimationEngine::toggle);
                    FastLED.show();
                }
                else {
                    SemaforoShowFlag(AnimationEngine::toggle ? FLAG_VSC : FLAG_NONE, false);
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

        //  START AUTO (SA)
        //  Accensione automatica 1→5, poi random delay, poi LO
        // ----------------------------------------------------
        case SEM_START_SEQUENCE: {
            // 1) Accensione progressiva
            if (!AnimationEngine::semStartDelayActive) {

                if (now - AnimationEngine::lastUpdate >= 1000) {
                    AnimationEngine::lastUpdate = now;

                    if (AnimationEngine::scStep < SEMAFORO_RINGS) {
                        SemaforoShowStartAutoSequence(AnimationEngine::scStep);
                        AnimationEngine::scStep++;
                    }
                    else {
                        // Tutti accesi → avvia random delay
                        unsigned long span = START_MAX_DELAY_MS - START_MIN_DELAY_MS;
                        AnimationEngine::semStartDelayEnd = now + START_MIN_DELAY_MS + random(span + 1);
                        AnimationEngine::semStartDelayActive = true;
                    }
                }
            }
            else {
                // 2) Random delay
                if ((long)(now - AnimationEngine::semStartDelayEnd) >= 0) {
                    // 3) Lights Out
                    SemaforoShowLightsOut();
                    AnimationEngine::activeSemState = SEM_NONE;
                    AnimationEngine::semStartDelayActive = false;
                }
            }

            break;
        }

        // ----------------------------------------------------
        //  START AUTO MANUALE (SP)
        //  Accensione progressiva 1→5, senza random delay
        // ----------------------------------------------------
        case SEM_START_SEQUENCE_MODE:
        case SEM1:
        case SEM2:
        case SEM3:
        case SEM4:
        case SEM5:
            SemaforoShowStartSequence(AnimationEngine::activeSemState);
            break;

        // ----------------------------------------------------
        //  PRE-GARA (accensione totale o parziale)
        // ----------------------------------------------------

        case SEM_PRE_RACE:
        case SEM_PRE_10:
        case SEM_PRE_5:
        case SEM_PRE_2:
        case SEM_PRE_1:
        case SEM_FORMATION_LAP:
            SemaforoPreRaceProcedure(AnimationEngine::activeSemState);
            break;

        // ----------------------------------------------------
        //  LIGHTS OUT (LO)
        // ----------------------------------------------------
        case SEM_LIGHTS_OUT:
            SemaforoShowLightsOut();
            AnimationEngine::activeSemState = SEM_NONE;
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