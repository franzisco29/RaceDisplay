#ifndef FLAG_SETTINGS_H
#define FLAG_SETTINGS_H

//
// ============================================================
//  FlagSettings.h
//  Configurazione generale del sistema RaceDisplay
//  Strutturato per repository Git (chiaro, stabile, leggibile)
//
// Autore: Francesco Troianiello
// Data: 2025-03-07
// Descrizione: Impostazioni globali per dispositivi, LED e tempi di lampeggio
// 
// Nota: Diventerà su SD per configurazione dinamica in futuro, ma per ora è un file di header statico per chiarezza e semplicità.
//
// ============================================================
//
//  Questo file definisce:
//   - ID del dispositivo
//   - Tipo di dispositivo (matrice, pit, semaforo)
//   - Mappatura settori
//   - Parametri hardware
//   - Tempi di lampeggio
//   - Costanti generali
//
//  È il file di configurazione principale dell'intero sistema.
//  Tutti gli altri moduli lo includono.
//
// ============================================================


// ------------------------------------------------------------
//  Identificazione dispositivo
// ------------------------------------------------------------
//
//  DEVICE_ID deve essere impostato manualmente su ogni dispositivo.
//  Valori possibili:
//
//    0 = Lap Done (matrice, settore 1)
//    1 = S1 (matrice, settore 2)
//    2 = S2 (matrice, settore 3)
//    3 = Pit In (2 anelli)
//    4 = Pit Out (2 anelli)
//    5 = Semaforo (5 anelli)
//    6 = Attack Mode In (future)
//    7 = Attack Mode Out (future)
//

// ------------------------------------------------------------
//  Override rapidi da sketch (.ino)
//  Definisci questi macro prima di #include <RaceDisplay.h>
//
//  In modalità HC_RUNTIME_CONFIG (DeviceConfig attivo),
//  DEVICE_ID e LED_BRIGHTNESS vengono ignorati a runtime
//  e letti da NVS/Flash. LED_PIN resta compile-time perché
//  FastLED richiede il pin come template parameter.
// ------------------------------------------------------------

#ifndef DEVICE_ID
#define DEVICE_ID       0
#endif

#ifndef LED_PIN
#define LED_PIN         6
#endif

#ifndef LED_BRIGHTNESS
#define LED_BRIGHTNESS  200     // 0-255 (200 = 78%)
#endif

#ifndef MATRIX_ROTATION
#define MATRIX_ROTATION 0
#endif


// ------------------------------------------------------------
//  Tipologia dispositivo
// ------------------------------------------------------------

#define DEVICE_TYPE_MATRIX     0
#define DEVICE_TYPE_PIT        1
#define DEVICE_TYPE_SEMAFORO   2
#define DEVICE_TYPE_SENDER     3

#if DEVICE_ID <= 2
    #define DEVICE_TYPE DEVICE_TYPE_MATRIX
#elif DEVICE_ID == 3 || DEVICE_ID == 4
    #define DEVICE_TYPE DEVICE_TYPE_PIT
#elif DEVICE_ID == 5
    #define DEVICE_TYPE DEVICE_TYPE_SEMAFORO
#elif DEVICE_ID == 8
    #define DEVICE_TYPE DEVICE_TYPE_SENDER
#else
    #define DEVICE_TYPE DEVICE_TYPE_MATRIX
#endif

// ------------------------------------------------------------
//  Macro utili per stampare valori nei pragma
// ------------------------------------------------------------
#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

// ------------------------------------------------------------
//  Diagnostica compile-time
// ------------------------------------------------------------
#ifndef HC_RUNTIME_CONFIG
  #if DEVICE_TYPE == DEVICE_TYPE_MATRIX
      #pragma message ("[RaceDisplay] DEVICE_TYPE = MATRIX (ID " STRINGIFY(DEVICE_ID) ")")
  #elif DEVICE_TYPE == DEVICE_TYPE_PIT
      #pragma message ("[RaceDisplay] DEVICE_TYPE = PIT (ID " STRINGIFY(DEVICE_ID) ")")
  #elif DEVICE_TYPE == DEVICE_TYPE_SEMAFORO
      #pragma message ("[RaceDisplay] DEVICE_TYPE = SEMAFORO (ID " STRINGIFY(DEVICE_ID) ")")
  #elif DEVICE_TYPE == DEVICE_TYPE_SENDER
      #pragma message ("[RaceDisplay] DEVICE_TYPE = SENDER (ID " STRINGIFY(DEVICE_ID) ")")
  #else
      #pragma message ("[RaceDisplay] DEVICE_TYPE = UNKNOWN (fallback MATRIX)")
  #endif
#else
  #pragma message ("[RaceDisplay] Modalita RUNTIME (DeviceConfig) - DEVICE_TYPE determinato a runtime")
#endif

// ------------------------------------------------------------
//  Dispositivi senza LED (DeviceSender) — solo modalità statica
// ------------------------------------------------------------
#ifndef HC_RUNTIME_CONFIG
  #if DEVICE_TYPE == DEVICE_TYPE_SENDER
      #undef LED_PIN
      #undef LED_BRIGHTNESS
      #define LED_PIN        -1
      #define LED_BRIGHTNESS 0
  #endif
#endif

// ------------------------------------------------------------
//  Helper runtime: calcola DEVICE_TYPE da un ID
//  Usato da DeviceRuntime e DeviceConfig
// ------------------------------------------------------------
inline uint8_t getDeviceType(uint8_t id) {
  if (id <= 2)             return DEVICE_TYPE_MATRIX;
  if (id == 3 || id == 4) return DEVICE_TYPE_PIT;
  if (id == 5)             return DEVICE_TYPE_SEMAFORO;
  if (id == 8)             return DEVICE_TYPE_SENDER;
  return DEVICE_TYPE_MATRIX;
}

// ------------------------------------------------------------
//  Mappatura settori per le matrici (ID 0–2)
// ------------------------------------------------------------
#define SECTOR_YELLOW_ID0   1
#define SECTOR_YELLOW_ID1   2
#define SECTOR_YELLOW_ID2   3

// ------------------------------------------------------------
//  Hardware LED
// ------------------------------------------------------------

// Matrici 8×8
#define MATRIX_WIDTH   8
#define MATRIX_HEIGHT  8
#define MATRIX_LEDS   (MATRIX_WIDTH * MATRIX_HEIGHT)

// Rotazioni matrice
#define MATRIX_ROT_0    0
#define MATRIX_ROT_90   1
#define MATRIX_ROT_180  2
#define MATRIX_ROT_270  3

// Semaforo (ID 5)
#ifndef RING_LEDS
#define RING_LEDS 12
#endif

#ifndef SEMAFORO_RINGS
#define SEMAFORO_RINGS 5
#endif

// Pit (ID 3–4)
#ifndef PIT_RINGS
#define PIT_RINGS 2
#endif

// Totali
#define PIT_TOTAL_LEDS      (PIT_RINGS * RING_LEDS)
#define SEMAFORO_TOTAL_LEDS (SEMAFORO_RINGS * RING_LEDS)

// ------------------------------------------------------------
//  Tempi di lampeggio (ms)
// ------------------------------------------------------------
#define BLINK_YELLOW_MS     300
#define BLINK_SC_MS         300
#define BLINK_VSC_MS        300
#define BLINK_RED_MS        500
#define BLINK_CHECK_MS      500
#define BLINK_BLUE_MS       500
#define BLINK_WET_MS        500

// ------------------------------------------------------------
//  Parametri Starting Procedure (solo ID 5)
// ------------------------------------------------------------
#define START_MIN_DELAY_MS  2000
#define START_MAX_DELAY_MS  5000

// ------------------------------------------------------------
//  Parametri Formation Lap (solo ID 5)
// ------------------------------------------------------------
#define FORMATION_LAP_RING1  2
#define FORMATION_LAP_RING2  4

// ------------------------------------------------------------
//  Parametri PIT (ID 3–4)
// ------------------------------------------------------------
#define PIT_RING_OPEN   1
#define PIT_RING_CLOSE  2

// ------------------------------------------------------------
//  Sicurezza: nessun delay() nel sistema
// ------------------------------------------------------------
#define USE_NON_BLOCKING true

#endif