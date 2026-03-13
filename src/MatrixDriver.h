#ifndef MATRIX_DRIVER_H
#define MATRIX_DRIVER_H

//
// ============================================================
//  MatrixDriver.h
//  Driver per matrici LED 8×8
// ============================================================

#include <FastLED.h>
#include "FlagSettings.h"
#include "FlagTypes.h"
#include "Colors.h"


// --- FIX COMPATIBILITÀ AVR / ESP / RISC-V -------------------
#if defined(ESP32) || defined(ESP8266)
// Su ESP32/ESP8266/ESP32-C3 PROGMEM è già gestito dal compilatore
    #ifndef PROGMEM
        #define PROGMEM
    #endif
    #ifndef pgm_read_byte
        #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
    #endif

#elif defined(ARDUINO_ARCH_AVR)
// Su AVR serve la libreria specifica
    #include <avr/pgmspace.h>

#else
// Altre architetture (RP2040, STM32, ecc.)
    #ifndef PROGMEM
        #define PROGMEM
    #endif
    #ifndef pgm_read_byte
        #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
    #endif
#endif
// -------------------------------------------------------------



// ------------------------------------------------------------
// Buffer LED
// ------------------------------------------------------------

static CRGB matrixLeds[MATRIX_LEDS];

int SCletter = 0;
int VSCletter = 0;
uint8_t SCframeCount = 0;
uint8_t VSCframeCount = 0;


// ------------------------------------------------------------
// XY mapping
// ------------------------------------------------------------

static inline uint16_t XY(uint8_t x, uint8_t y) {
    return (y * MATRIX_WIDTH) + x;
}


// ------------------------------------------------------------
// Setup
// ------------------------------------------------------------

static void MatrixSetup() {

    FastLED.addLeds<NEOPIXEL, LED_PIN>(matrixLeds, MATRIX_LEDS);
    FastLED.setBrightness(LED_BRIGHTNESS);

    FastLED.clear(true);
}


// ------------------------------------------------------------
// Clear
// ------------------------------------------------------------

static void MatrixClear() {

    for(uint8_t i=0;i<MATRIX_LEDS;i++)
        matrixLeds[i] = COLOR_BLACK;
}


// ------------------------------------------------------------
// FONT 5x7 (PROGMEM)
// ------------------------------------------------------------

static const uint8_t LETTER_S[6] PROGMEM = {
    0b011110,
    0b110000,
    0b111100,
    0b001111,
    0b000011,
    0b011110
};




static const uint8_t LETTER_C[6] PROGMEM = {
0b011110,
0b100001,
0b100000,
0b100000,
0b100001,
0b011110
};


static const uint8_t LETTER_V[6] PROGMEM = {
0b100001,
0b100001,
0b100001,
0b010010,
0b010010,
0b001100
};



// ------------------------------------------------------------
// Numeri 5x7
// ------------------------------------------------------------

static const uint8_t NUMBERS[10][7] PROGMEM = {

{0b01110,0b10001,0b10011,0b10101,0b11001,0b10001,0b01110}, //0
{0b00100,0b01100,0b00100,0b00100,0b00100,0b00100,0b01110}, //1
{0b01110,0b10001,0b00001,0b00010,0b00100,0b01000,0b11111}, //2
{0b11110,0b00001,0b00001,0b01110,0b00001,0b00001,0b11110}, //3
{0b00010,0b00110,0b01010,0b10010,0b11111,0b00010,0b00010}, //4
{0b11111,0b10000,0b11110,0b00001,0b00001,0b10001,0b01110}, //5
{0b00110,0b01000,0b10000,0b11110,0b10001,0b10001,0b01110}, //6
{0b11111,0b00001,0b00010,0b00100,0b01000,0b01000,0b01000}, //7
{0b01110,0b10001,0b10001,0b01110,0b10001,0b10001,0b01110}, //8
{0b01110,0b10001,0b10001,0b01111,0b00001,0b00010,0b01100}  //9

};

// ------------------------------------------------------------
//  Array Lettere
// ------------------------------------------------------------

char SC_LETTERS[] = {'S','C'};
char VSC_LETTERS[] = {'V','S','C'};

// ------------------------------------------------------------
// Disegna glyph 5x7
// ------------------------------------------------------------

static void drawGlyph(const uint8_t* glyph, CRGB color)
{
    for(uint8_t y = 0; y < 6; y++)
    {
        uint8_t row = pgm_read_byte(&glyph[y]);

        for(uint8_t x = 0; x < 6; x++)
        {
            if(row & (1 << (5 - x)))
            {
                matrixLeds[XY(x + 1, y + 1)] = color;
            }
        }
    }
}




// ------------------------------------------------------------
// Disegna lettera
// ------------------------------------------------------------

static void drawLetter(char letter, CRGB color)
{

    switch(letter)
    {

        case 'S':
            drawGlyph(LETTER_S,color);
            break;

        case 'C':
            drawGlyph(LETTER_C,color);
            break;

        case 'V':
            drawGlyph(LETTER_V,color);
            break;

        default:
            break;
    }

}


// ------------------------------------------------------------
// Disegna numero
// ------------------------------------------------------------

static void drawNumber(uint8_t num, CRGB color)
{

    if(num>9) return;

    drawGlyph(NUMBERS[num],color);

}

// ------------------------------------------------------------
// Avanza lettera ogni 4 frame (A-B-A-B)
// ------------------------------------------------------------
static inline void advanceLetterEvery4Frames(int& letterIndex, uint8_t lettersCount, uint8_t& frameCount)
{
    frameCount++;

    if(frameCount >= 4) {
        frameCount = 0;
        letterIndex++;

        if(letterIndex >= lettersCount) {
            letterIndex = 0;
        }
    }
}



// ------------------------------------------------------------
// Rendering bandiere
// ------------------------------------------------------------

static void MatrixShowFlag(FlagType flag, bool toggle = false)
{

    MatrixClear();

    switch(flag)
    {

        case FLAG_GREEN:
            fill_solid(matrixLeds,MATRIX_LEDS,FLAG_GREEN_COLOR);
            break;

        case FLAG_RED:
            fill_solid(matrixLeds,MATRIX_LEDS,FLAG_RED_COLOR);
            break;

        case FLAG_BLUE_S1:
            if (DEVICE_ID == 0)
                fill_solid(matrixLeds, MATRIX_LEDS, FLAG_BLUE_COLOR);
            break;

        case FLAG_BLUE_S2:
            if (DEVICE_ID == 1)
                fill_solid(matrixLeds, MATRIX_LEDS, FLAG_BLUE_COLOR);
            break;

        case FLAG_BLUE_S3:
            if (DEVICE_ID == 2)
                fill_solid(matrixLeds, MATRIX_LEDS, FLAG_BLUE_COLOR);
            break;


        case FLAG_CHECKERED:

            for (uint8_t i = 0; i < MATRIX_LEDS; i++) {
                uint8_t col = i % 8;
                bool isWhite = toggle ? (col % 2 == 1) : (col % 2 == 0);
                matrixLeds[i] = isWhite ? FLAG_CHECKER_WHITE : FLAG_CHECKER_BLACK;
            }
                            

            break;

        case FLAG_WET:

            for(uint8_t i=0;i<MATRIX_LEDS;i++)
                matrixLeds[i]=(i%2)?FLAG_WET_RED:FLAG_WET_YELLOW;

            break;


        // ----------------------------------------------------
        // FIA sector yellow
        // ----------------------------------------------------

        case FLAG_YELLOW_S1:
            if(DEVICE_ID==0)
                fill_solid(matrixLeds,MATRIX_LEDS,FLAG_YELLOW_COLOR);
            break;

        case FLAG_YELLOW_S2:
            if(DEVICE_ID==1)
                fill_solid(matrixLeds,MATRIX_LEDS,FLAG_YELLOW_COLOR);
            break;

        case FLAG_YELLOW_S3:
            if(DEVICE_ID==2)
                fill_solid(matrixLeds,MATRIX_LEDS,FLAG_YELLOW_COLOR);
            break;

        case FLAG_YELLOW_FS:
            if(DEVICE_ID==0 || DEVICE_ID==1)
                fill_solid(matrixLeds,MATRIX_LEDS,FLAG_YELLOW_COLOR);
            break;

        case FLAG_YELLOW_ST:
            if(DEVICE_ID==1 || DEVICE_ID==2)
                fill_solid(matrixLeds,MATRIX_LEDS,FLAG_YELLOW_COLOR);
            break;

        case FLAG_YELLOW_TF:
            if(DEVICE_ID==2 || DEVICE_ID==0)
                fill_solid(matrixLeds,MATRIX_LEDS,FLAG_YELLOW_COLOR);
            break;


        // ----------------------------------------------------
        // Safety Car
        // ----------------------------------------------------

        case FLAG_SC:
            {
            const char currentLetter = SC_LETTERS[SCletter];

            if(toggle) {
                // Frame A: sfondo giallo, lettera nera
                fill_solid(matrixLeds, MATRIX_LEDS, FLAG_YELLOW_COLOR);
                drawLetter(currentLetter, COLOR_BLACK);
            }
            else {
                // Frame B: sfondo nero, lettera gialla
                fill_solid(matrixLeds, MATRIX_LEDS, COLOR_BLACK);
                drawLetter(currentLetter, FLAG_YELLOW_COLOR);
            }

            advanceLetterEvery4Frames(
                SCletter,
                (uint8_t)(sizeof(SC_LETTERS) / sizeof(SC_LETTERS[0])),
                SCframeCount
            );
            break;
            }

        case FLAG_VSC:
            {
            const char currentLetter = VSC_LETTERS[VSCletter];

            if(toggle) {
                // Frame A: sfondo giallo, lettera nera
                Serial.println("VSC: Frame A");
                fill_solid(matrixLeds, MATRIX_LEDS, FLAG_YELLOW_COLOR);
                drawLetter(currentLetter, COLOR_BLACK);
            }
            else {
                // Frame B: sfondo nero, lettera gialla
                Serial.println("VSC: Frame B");
                fill_solid(matrixLeds, MATRIX_LEDS, COLOR_BLACK);
                drawLetter(currentLetter, FLAG_YELLOW_COLOR);
            }

            advanceLetterEvery4Frames(
                VSCletter,
                (uint8_t)(sizeof(VSC_LETTERS) / sizeof(VSC_LETTERS[0])),
                VSCframeCount
            );
            break;
            }

        default:
            break;
    }

    FastLED.show();
}

#endif