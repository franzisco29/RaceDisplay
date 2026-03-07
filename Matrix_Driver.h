#ifndef MATRIX_DRIVER_H
#define MATRIX_DRIVER_H

//
// ============================================================
//  MatrixDriver.h
//  Driver per matrici LED 8×8 (ID 0–2)
//  Strutturato per repository Git (chiaro, modulare, stabile)
// ============================================================
//
//  Questo file gestisce:
//   - Inizializzazione matrice
//   - Rendering bandiere
//   - Rendering SC/VSC (lettere S, C, V)
//   - Rendering checkered
//   - Rendering numeri (solo ID 0)
//   - Rendering animazioni non bloccanti
//
//  Nessuna logica di routing: quella è in FlagManager.
//  Questo file si occupa solo di DISEGNARE.
//
// ============================================================
//

#include <FastLED.h>
#include "FlagSettings.h"
#include "FlagTypes.h"
#include "colors.h"
#include "AnimationEngine.h"


// ------------------------------------------------------------
//  Buffer LED matrice
// ------------------------------------------------------------

static CRGB matrixLeds[MATRIX_LEDS];



// ------------------------------------------------------------
//  Funzioni interne (statiche)
// ------------------------------------------------------------

// Converte coordinate (x,y) in indice lineare
static inline uint16_t XY(uint8_t x, uint8_t y) {
    return (y * MATRIX_WIDTH) + x;
}



// ------------------------------------------------------------
//  Setup matrice
// ------------------------------------------------------------

static void MatrixSetup() {

    FastLED.addLeds<NEOPIXEL, LED_PIN>(matrixLeds, MATRIX_LEDS);
    FastLED.setBrightness(LED_BRIGHTNESS);
    FastLED.clear(true);
}



// ------------------------------------------------------------
//  Clear matrice
// ------------------------------------------------------------

static void MatrixClear() {
    for (int i = 0; i < MATRIX_LEDS; i++)
        matrixLeds[i] = COLOR_BLACK;
}



// ------------------------------------------------------------
//  Disegna una lettera 5×7
// ------------------------------------------------------------
//
//  Lettere supportate: S, C, V
//

static void drawLetter(char letter, CRGB color) {

    MatrixClear();

    static const uint8_t LETTER_S[7] = {
        0b01110,
        0b10000,
        0b10000,
        0b01110,
        0b00001,
        0b00001,
        0b01110
    };

    static const uint8_t LETTER_C[7] = {
        0b01110,
        0b10001,
        0b10000,
        0b10000,
        0b10000,
        0b10001,
        0b01110
    };

    static const uint8_t LETTER_V[7] = {
        0b10001,
        0b10001,
        0b10001,
        0b10001,
        0b01010,
        0b01010,
        0b00100
    };

    const uint8_t* glyph = nullptr;

    switch(letter) {
        case 'S': glyph = LETTER_S; break;
        case 'C': glyph = LETTER_C; break;
        case 'V': glyph = LETTER_V; break;
        default: return;
    }

    for (uint8_t y = 0; y < 7; y++) {
        for (uint8_t x = 0; x < 5; x++) {
            if (glyph[y] & (1 << (4 - x))) {
                matrixLeds[XY(x + 1, y + 1)] = color;
            }
        }
    }
}



// ------------------------------------------------------------
//  Disegna numero (solo ID 0)
// ------------------------------------------------------------

static void drawNumber(uint8_t num, CRGB color) {

    MatrixClear();

    // Numeri 0–9 (5×7)
    static const uint8_t NUMBERS[10][7] = {
        {0b01110,0b10001,0b10011,0b10101,0b11001,0b10001,0b01110}, // 0
        {0b00100,0b01100,0b00100,0b00100,0b00100,0b00100,0b01110}, // 1
        {0b01110,0b10001,0b00001,0b00010,0b00100,0b01000,0b11111}, // 2
        {0b11110,0b00001,0b00001,0b01110,0b00001,0b00001,0b11110}, // 3
        {0b00010,0b00110,0b01010,0b10010,0b11111,0b00010,0b00010}, // 4
        {0b11111,0b10000,0b11110,0b00001,0b00001,0b10001,0b01110}, // 5
        {0b00110,0b01000,0b10000,0b11110,0b10001,0b10001,0b01110}, // 6
        {0b11111,0b00001,0b00010,0b00100,0b01000,0b01000,0b01000}, // 7
        {0b01110,0b10001,0b10001,0b01110,0b10001,0b10001,0b01110}, // 8
        {0b01110,0b10001,0b10001,0b01111,0b00001,0b00010,0b01100}  // 9
    };

    if (num > 9) return;

    for (uint8_t y = 0; y < 7; y++) {
        for (uint8_t x = 0; x < 5; x++) {
            if (NUMBERS[num][y] & (1 << (4 - x))) {
                matrixLeds[XY(x + 1, y + 1)] = color;
            }
        }
    }
}



// ------------------------------------------------------------
//  Rendering bandiere
// ------------------------------------------------------------

static void MatrixShowFlag(FlagType flag) {

    MatrixClear();

    switch(flag) {

        case FLAG_GREEN:
            fill_solid(matrixLeds, MATRIX_LEDS, FLAG_GREEN_COLOR);
            break;

        case FLAG_RED:
            fill_solid(matrixLeds, MATRIX_LEDS, FLAG_RED_COLOR);
            break;

        case FLAG_BLUE:
            fill_solid(matrixLeds, MATRIX_LEDS, FLAG_BLUE_COLOR);
            break;

        case FLAG_CHECKERED:
            for (int i = 0; i < MATRIX_LEDS; i++)
                matrixLeds[i] = (i % 2 == 0) ? FLAG_CHECKER_WHITE : FLAG_CHECKER_BLACK;
            break;

        case FLAG_WET:
            for (int i = 0; i < MATRIX_LEDS; i++)
                matrixLeds[i] = (i % 2 == 0) ? FLAG_WET_YELLOW : FLAG_WET_RED;
            break;

        case FLAG_SC:
            drawLetter('S', COLOR_YELLOW);
            break;

        case FLAG_VSC:
            drawLetter('V', COLOR_YELLOW);
            break;

        default:
            break;
    }

    FastLED.show();
}



#endif
