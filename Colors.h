#ifndef COLORS_H
#define COLORS_H

//
// ============================================================
//  colors.h
//  Definizione colori LED per RaceDisplay
//  Strutturato per repository Git (chiaro, modulare, stabile)
// ============================================================
//
//  Questo file definisce:
//   - Colori standard per bandiere FIA/FIM
//   - Colori per PIT (open/close/valid)
//   - Colori per semaforo
//   - Colori utility (nero, bianco, viola, ecc.)
//
//  Tutti i driver (matrici e anelli) usano questi valori.
//  Nessuna logica, solo costanti.
//
// ============================================================
//


// ------------------------------------------------------------
//  Inclusione libreria LED (FastLED o equivalente)
// ------------------------------------------------------------

#include <FastLED.h>



// ------------------------------------------------------------
//  Colori base
// ------------------------------------------------------------

static const CRGB COLOR_BLACK   = CRGB(0,   0,   0);
static const CRGB COLOR_WHITE   = CRGB(255, 255, 255);
static const CRGB COLOR_RED     = CRGB(255, 0,   0);
static const CRGB COLOR_GREEN   = CRGB(0,   255, 0);
static const CRGB COLOR_BLUE    = CRGB(0,   0,   255);
static const CRGB COLOR_YELLOW  = CRGB(255, 255, 0);
static const CRGB COLOR_ORANGE  = CRGB(255, 128, 0);
static const CRGB COLOR_PURPLE  = CRGB(180, 0,   255);   // Usato per PIT VALID
static const CRGB COLOR_CYAN    = CRGB(0,   255, 255);
static const CRGB COLOR_MAGENTA = CRGB(255, 0,   255);



// ------------------------------------------------------------
//  Colori bandiere
// ------------------------------------------------------------

static const CRGB FLAG_GREEN_COLOR     = COLOR_GREEN;
static const CRGB FLAG_RED_COLOR       = COLOR_RED;
static const CRGB FLAG_BLUE_COLOR      = COLOR_BLUE;
static const CRGB FLAG_YELLOW_COLOR    = COLOR_YELLOW;
static const CRGB FLAG_CHECKER_WHITE   = COLOR_WHITE;
static const CRGB FLAG_CHECKER_BLACK   = COLOR_BLACK;
static const CRGB FLAG_WET_YELLOW      = COLOR_YELLOW;
static const CRGB FLAG_WET_RED         = COLOR_RED;



// ------------------------------------------------------------
//  Colori PIT (ID 3–4)
// ------------------------------------------------------------
//
//  Anello 1 = Open (verde)
//  Anello 2 = Close (rosso)
//  Entrambi = Valid (viola)
//

static const CRGB PIT_OPEN_COLOR   = COLOR_GREEN;
static const CRGB PIT_CLOSE_COLOR  = COLOR_RED;
static const CRGB PIT_VALID_COLOR  = COLOR_PURPLE;



// ------------------------------------------------------------
//  Colori Semaforo (ID 5)
// ------------------------------------------------------------
//
//  Gli anelli usano gli stessi colori delle bandiere,
//  ma qui possiamo definire eventuali varianti future.
//

static const CRGB SEM_GREEN_COLOR  = COLOR_GREEN;
static const CRGB SEM_RED_COLOR    = COLOR_RED;
static const CRGB SEM_BLUE_COLOR   = COLOR_BLUE;
static const CRGB SEM_YELLOW_COLOR = COLOR_YELLOW;
static const CRGB SEM_WET_YELLOW   = COLOR_YELLOW;
static const CRGB SEM_WET_RED      = COLOR_RED;



// ------------------------------------------------------------
//  Colori utility
// ------------------------------------------------------------

static const CRGB COLOR_DIM_WHITE = CRGB(80, 80, 80);   // utile per numeri
static const CRGB COLOR_DIM_RED   = CRGB(80, 0, 0);
static const CRGB COLOR_DIM_GREEN = CRGB(0, 80, 0);



#endif
