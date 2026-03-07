#ifndef RING_DRIVER_H
#define RING_DRIVER_H

//
// ============================================================
//  RingDriver.h
//  Driver per anelli LED (PIT: 2 anelli, Semaforo: 5 anelli)
//  Strutturato per repository Git (chiaro, modulare, stabile)
// ============================================================
//
//  Questo file gestisce:
//   - Inizializzazione anelli LED
//   - Rendering PIT (open/close/valid/off)
//   - Rendering semaforo (5 anelli)
//   - Rendering bandiere su semaforo
//   - Animazioni SC/VSC/Yellow/Checkered/Wet
//   - Nessuna logica di routing (gestita da FlagManager)
//
// ============================================================
//

#include <FastLED.h>
#include "FlagSettings.h"
#include "FlagTypes.h"
#include "colors.h"
#include "AnimationEngine.h"



// ------------------------------------------------------------
//  Buffer LED per anelli
// ------------------------------------------------------------

#if DEVICE_TYPE == DEVICE_TYPE_PIT
    static CRGB ringLeds[PIT_RINGS];
#elif DEVICE_TYPE == DEVICE_TYPE_SEMAFORO
    static CRGB ringLeds[SEMAFORO_RINGS];
#endif



// ------------------------------------------------------------
//  Setup anelli
// ------------------------------------------------------------

static void PitRingSetup() {
    FastLED.addLeds<NEOPIXEL, LED_PIN>(ringLeds, PIT_RINGS);
    FastLED.clear(true);
}

static void SemaforoRingSetup() {
    FastLED.addLeds<NEOPIXEL, LED_PIN>(ringLeds, SEMAFORO_RINGS);
    FastLED.clear(true);
}



// ------------------------------------------------------------
//  Clear anelli
// ------------------------------------------------------------

static void RingClear() {
    #if DEVICE_TYPE == DEVICE_TYPE_PIT
        for (int i = 0; i < PIT_RINGS; i++)
            ringLeds[i] = COLOR_BLACK;
    #elif DEVICE_TYPE == DEVICE_TYPE_SEMAFORO
        for (int i = 0; i < SEMAFORO_RINGS; i++)
            ringLeds[i] = COLOR_BLACK;
    #endif
}



// ------------------------------------------------------------
//  Rendering PIT (ID 3–4)
// ------------------------------------------------------------
//
//  Anello 1 = Open (verde)
//  Anello 2 = Close (rosso)
//  Entrambi = Valid (viola)
//

static void PitShow(PitState state) {

    if (DEVICE_TYPE != DEVICE_TYPE_PIT)
        return;

    RingClear();

    switch(state) {

        case PIT_OPEN:
            ringLeds[0] = PIT_OPEN_COLOR;
            break;

        case PIT_CLOSE:
            ringLeds[1] = PIT_CLOSE_COLOR;
            break;

        case PIT_VALID:
            ringLeds[0] = PIT_VALID_COLOR;
            ringLeds[1] = PIT_VALID_COLOR;
            break;

        case PIT_OFF:
        default:
            break;
    }

    FastLED.show();
}



// ------------------------------------------------------------
//  Rendering Semaforo (ID 5)
// ------------------------------------------------------------
//
//  Anelli:
//    1 = Settore 1
//    2 = PIT
//    3 = Settore 2
//    4 = Wet Race
//    5 = Settore 3
//

static void SemaforoShowFlag(FlagType flag) {

    if (DEVICE_TYPE != DEVICE_TYPE_SEMAFORO)
        return;

    RingClear();

    switch(flag) {

        // ----------------------------------------------------
        //  Green flag → tutti accesi fissi
        // ----------------------------------------------------
        case FLAG_GREEN:
            for (int i = 0; i < SEMAFORO_RINGS; i++)
                ringLeds[i] = SEM_GREEN_COLOR;
            break;


        // ----------------------------------------------------
        //  Red flag → tutti lampeggianti (gestito da AnimationEngine)
        // ----------------------------------------------------
        case FLAG_RED:
            for (int i = 0; i < SEMAFORO_RINGS; i++)
                ringLeds[i] = SEM_RED_COLOR;
            break;


        // ----------------------------------------------------
        //  Blue flag → tutti blu lampeggianti
        // ----------------------------------------------------
        case FLAG_BLUE:
            for (int i = 0; i < SEMAFORO_RINGS; i++)
                ringLeds[i] = SEM_BLUE_COLOR;
            break;


        // ----------------------------------------------------
        //  Yellow settori (1,3,5)
        // ----------------------------------------------------
        case FLAG_YELLOW_S1:
            ringLeds[0] = SEM_YELLOW_COLOR;
            break;

        case FLAG_YELLOW_S2:
            ringLeds[2] = SEM_YELLOW_COLOR;
            break;

        case FLAG_YELLOW_S3:
            ringLeds[4] = SEM_YELLOW_COLOR;
            break;

        case FLAG_YELLOW_FS:
            ringLeds[0] = SEM_YELLOW_COLOR;
            ringLeds[2] = SEM_YELLOW_COLOR;
            break;

        case FLAG_YELLOW_ST:
            ringLeds[2] = SEM_YELLOW_COLOR;
            ringLeds[4] = SEM_YELLOW_COLOR;
            break;

        case FLAG_YELLOW_TF:
            ringLeds[4] = SEM_YELLOW_COLOR;
            ringLeds[0] = SEM_YELLOW_COLOR;
            break;


        // ----------------------------------------------------
        //  Checkered → dispari ↔ pari (animazione)
        // ----------------------------------------------------
        case FLAG_CHECKERED:
            for (int i = 0; i < SEMAFORO_RINGS; i++)
                ringLeds[i] = (i % 2 == 0) ? FLAG_CHECKER_WHITE : FLAG_CHECKER_BLACK;
            break;


        // ----------------------------------------------------
        //  Wet Race → solo anello 4
        // ----------------------------------------------------
        case FLAG_WET:
            ringLeds[3] = SEM_WET_YELLOW;
            break;


        // ----------------------------------------------------
        //  SC / VSC → animazioni gestite da AnimationEngine
        // ----------------------------------------------------
        case FLAG_SC:
        case FLAG_VSC:
            // L’animazione viene gestita altrove
            break;


        default:
            break;
    }

    FastLED.show();
}



// ------------------------------------------------------------
//  Funzioni speciali semaforo
// ------------------------------------------------------------

static void SemaforoShowFormationLap() {
    RingClear();
    ringLeds[1] = SEM_GREEN_COLOR;   // Anello 2
    ringLeds[3] = SEM_GREEN_COLOR;   // Anello 4
    FastLED.show();
}

static void SemaforoShowLightsOut() {
    RingClear();
    FastLED.show();
}



#endif
