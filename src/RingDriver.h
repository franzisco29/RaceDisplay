#ifndef RING_DRIVER_H
#define RING_DRIVER_H

#include <FastLED.h>
#include "FlagSettings.h"
#include "FlagTypes.h"
#include "Colors.h"

// ------------------------------------------------------------
//  Parametri anelli
// ------------------------------------------------------------

// PIT: 2 anelli
#define PIT_TOTAL_LEDS      (PIT_RINGS * RING_LEDS)

// Semaforo: 5 anelli
#define SEMAFORO_TOTAL_LEDS (SEMAFORO_RINGS * RING_LEDS)

// Buffer unico: allochiamo per il caso peggiore (semaforo)
static CRGB ringLeds[SEMAFORO_TOTAL_LEDS];


// ------------------------------------------------------------
//  Helpers per lavorare a livello di anello
// ------------------------------------------------------------
static void SetRingColor(uint8_t ringIndex, const CRGB &color) {

#ifdef REVERSE_ORDER
    ringIndex = (SEMAFORO_RINGS - 1) - ringIndex;
#endif

    uint16_t start = ringIndex * RING_LEDS;

    for (uint8_t i = 0; i < RING_LEDS; i++) {
        ringLeds[start + i] = color;
    }
}



static void ClearAll(uint16_t count) {
    for (uint16_t i = 0; i < count; i++)
        ringLeds[i] = COLOR_BLACK;
}


// ------------------------------------------------------------
//  Setup anelli
// ------------------------------------------------------------

static void PitRingSetup() {
    FastLED.addLeds<NEOPIXEL, LED_PIN>(ringLeds, PIT_TOTAL_LEDS);
    FastLED.clear(true);
}

static void SemaforoRingSetup() {
    FastLED.addLeds<NEOPIXEL, LED_PIN>(ringLeds, SEMAFORO_TOTAL_LEDS);
    FastLED.clear(true);
}


// ------------------------------------------------------------
//  Clear anelli
// ------------------------------------------------------------

static void RingClear() {
    #if DEVICE_TYPE == DEVICE_TYPE_PIT
        ClearAll(PIT_TOTAL_LEDS);
    #elif DEVICE_TYPE == DEVICE_TYPE_SEMAFORO
        ClearAll(SEMAFORO_TOTAL_LEDS);
    #endif
}


static void FlagRingClear() {
    SetRingColor(0, COLOR_BLACK);
    SetRingColor(2, COLOR_BLACK);
    SetRingColor(4, COLOR_BLACK);
}

static void WetRingClear() {
    SetRingColor(3, COLOR_BLACK);
}

static void PitRingClear() {
    //SetRingColor(0, COLOR_BLACK);
    SetRingColor(1, COLOR_BLACK);
}

// ------------------------------------------------------------
//  Rendering PIT (ID 3–4)
// ------------------------------------------------
//
//  Anello 1 = Open (verde)  → ringIndex 0
//  Anello 2 = Close (rosso) → ringIndex 1
//

static void PitShow(PitState state) {

    if (DEVICE_TYPE == DEVICE_TYPE_MATRIX)
        return;

    RingClear();

    if(DEVICE_TYPE == DEVICE_TYPE_PIT) {

        switch(state) {

            case PIT_OPEN:
                SetRingColor(0, PIT_OPEN_COLOR);
                break;

            case PIT_CLOSE:
                SetRingColor(1, PIT_CLOSE_COLOR);
                break;

            case PIT_VALID:
                SetRingColor(0, PIT_VALID_COLOR);
                SetRingColor(1, PIT_VALID_COLOR);
                break;

            case PIT_OFF:
                SetRingColor(0, PIT_OFF_COLOR);
                SetRingColor(1, PIT_OFF_COLOR);
                 break;
            default:
                break;
        }
    }

    if (DEVICE_TYPE == DEVICE_TYPE_SEMAFORO) {
        // Per sicurezza: il PIT sul semaforo è sempre anello 2 rosso
        switch(state) {

            case PIT_OPEN:
                SetRingColor(1, PIT_OPEN_COLOR);
                break;

            case PIT_CLOSE:
                SetRingColor(1, PIT_CLOSE_COLOR);
                break;

            case PIT_VALID:
                SetRingColor(1, PIT_VALID_COLOR);
                break;

            case PIT_OFF:
                SetRingColor(1, PIT_OFF_COLOR);
                break;
            default:
                break;
        }
    }    

    FastLED.show();
}


// ------------------------------------------------------------
//  Rendering Semaforo (ID 5)
// ------------------------------------------------------------
//
//  Anelli logici (0-based):
//    0 = Settore 1
//    1 = PIT
//    2 = Settore 2
//    3 = Wet Race
//    4 = Settore 3
//

static void SemaforoShowFlag(FlagType flag, bool toggle = false) {

    if (DEVICE_TYPE != DEVICE_TYPE_SEMAFORO)
        return;


    //RingClear()
    
    switch(flag) {

        
        case FLAG_NONE:
                FlagRingClear();
               
            break;
    
        case FLAG_DRY:
                WetRingClear();
            break;
        
        // Green → tutti accesi
        case FLAG_GREEN:
            for (int i = 0; i < SEMAFORO_RINGS; i++)
                SetRingColor(i, SEM_GREEN_COLOR);
            break;

        // Red → tutti (animazione altrove)
        case FLAG_RED:
            for (int i = 0; i < SEMAFORO_RINGS; i++)
                SetRingColor(i, toggle ? SEM_RED_COLOR : COLOR_BLACK);
            break;

        // Blue settoriale
        case FLAG_BLUE_S1:
            SetRingColor(0, SEM_BLUE_COLOR);
            break;
        case FLAG_BLUE_S2:
            SetRingColor(2, SEM_BLUE_COLOR);
            break;
        case FLAG_BLUE_S3:
            SetRingColor(4, SEM_BLUE_COLOR);
            break;  //DA AGGIUNGERE DELETE BLUE

        // Yellow settori
        case FLAG_YELLOW_S1:
            SetRingColor(0, SEM_YELLOW_COLOR);
            break;
        case FLAG_YELLOW_S2:
            SetRingColor(2, SEM_YELLOW_COLOR);
            break;
        case FLAG_YELLOW_S3:
            SetRingColor(4, SEM_YELLOW_COLOR);
            break;

        case FLAG_YELLOW_FS:
            SetRingColor(0, SEM_YELLOW_COLOR);
            SetRingColor(2, SEM_YELLOW_COLOR);
            break;

        case FLAG_YELLOW_ST:
            SetRingColor(2, SEM_YELLOW_COLOR);
            SetRingColor(4, SEM_YELLOW_COLOR);
            break;

        case FLAG_YELLOW_TF:
            SetRingColor(4, SEM_YELLOW_COLOR);
            SetRingColor(0, SEM_YELLOW_COLOR);
            break;

        // Checkered → dispari/pari a livello di anello
        case FLAG_CHECKERED:
            if (!toggle){
                for (int i = 0; i < SEMAFORO_RINGS; i++)
                    SetRingColor(i, (i % 2 == 0) ? FLAG_CHECKER_WHITE : FLAG_CHECKER_BLACK);
            }else {
                for (int i = 0; i < SEMAFORO_RINGS; i++)
                    SetRingColor(i, (i % 2 == 0) ? FLAG_CHECKER_BLACK : FLAG_CHECKER_WHITE);
            }
            
            break;

        // Wet → solo anello 3 (index 3)
        case FLAG_WET:
            if(toggle){
                SetRingColor(3, SEM_WET_RED);
            }else {
                SetRingColor(3, SEM_WET_YELLOW);
            }                
            break;

        // SC / VSC → animazioni altrove
        case FLAG_SC:
                if (toggle) {
                    SetRingColor(0, SEM_SC_COLOR);
                    SetRingColor(2, COLOR_BLACK);
                    SetRingColor(4, SEM_SC_COLOR);
                } else {
                    SetRingColor(0, COLOR_BLACK);
                    SetRingColor(2, SEM_SC_COLOR);
                    SetRingColor(4, COLOR_BLACK);
                }
            break;
        case FLAG_VSC:
                SetRingColor(0, SEM_VSC_COLOR);
                SetRingColor(2, SEM_VSC_COLOR);
                SetRingColor(4, SEM_VSC_COLOR);

            break;

        default:
            break;
    }

    FastLED.show();
}



// ------


static void SemaforoShowStartSequence(SemaforoState semState) {
        if (DEVICE_TYPE != DEVICE_TYPE_SEMAFORO)
            return;
    
    
        switch(semState) {
    
                case SEM_START_SEQUENCE:
                    RingClear();
                    break;
                case SEM_START_SEQUENCE_MODE:
                    RingClear();
                    break;
                case SEM1:
                    SetRingColor(0, SEM_RED_COLOR);
                    break;
                case SEM2:
                    SetRingColor(1, SEM_RED_COLOR);
                    break;
                case SEM3:
                    SetRingColor(2, SEM_RED_COLOR); 
                    break;
                case SEM4:
                    SetRingColor(3, SEM_RED_COLOR);
                    break;
                case SEM5:
                    SetRingColor(4, SEM_RED_COLOR);
                    break;
                default:
                    break;
        }
    FastLED.show();
}


static void SemaforoPreRaceProcedure(SemaforoState semState) {
    if (DEVICE_TYPE != DEVICE_TYPE_SEMAFORO)
        return;
    switch (semState)
    {
        case SEM_PRE_RACE:
                for (int i = 0; i < SEMAFORO_RINGS; i++)
                    SetRingColor(i, SEM_RED_COLOR);
                break;
        case SEM_PRE_10:
                //Serial.println("Pre10");
                SetRingColor(4, SEM_NONE_COLOR);
                break;
        case SEM_PRE_5:
                //Serial.println("Pre5");
                SetRingColor(3, SEM_NONE_COLOR);
                break;
        case SEM_PRE_2:
                //Serial.println("Pre2");            
                SetRingColor(2, SEM_NONE_COLOR);
                break;
        case SEM_PRE_1:
                //Serial.println("Pre1");
                SetRingColor(1, SEM_NONE_COLOR);
                break;
        case SEM_FORMATION_LAP:
            RingClear();
            SetRingColor(1, SEM_RED_COLOR);
            SetRingColor(3, SEM_RED_COLOR);
            break;
        default:
            break;
    }

    
    FastLED.show(); 
}



static void SemaforoShowStartAutoSequence(int ring_on) {
    SetRingColor(ring_on, SEM_RED_COLOR);
    FastLED.show();
}

// ------------------------------------------------------------
//  Funzioni speciali semaforo
// ------------------------------------------------------------

static void SemaforoShowLightsOut() {
    RingClear();
    FastLED.show();
}





#endif
