#ifndef FLAG_TYPES_H
#define FLAG_TYPES_H

//
// ============================================================
//  FlagTypes.h
//  Enumerazioni bandiere, stati speciali e comandi RaceDisplay
//  Strutturato per repository Git (chiaro, modulare, stabile)
// ============================================================
//
//  Questo file definisce:
//   - Tutte le bandiere FIA/FIM usate dal sistema
//   - Stati speciali (SC, VSC, Checkered, ecc.)
//   - Comandi semaforo (pre‑race, start, formation lap)
//   - Comandi PIT (open/close/valid)
//   - Comandi di sistema
//
//  È un file centrale: nessuna logica, solo enumerazioni.
//  Tutti i moduli della libreria lo includono.
//
// ============================================================
//


// ------------------------------------------------------------
//  Bandiere principali
// ------------------------------------------------------------

enum FlagType {

    FLAG_NONE = 0,          // Nessuna bandiera

    // --------------------------------------------------------
    //  GREEN FLAGS
    // --------------------------------------------------------
    FLAG_GREEN,             // Green flag globale (FG)
    FLAG_GREEN_S1,          // Green settore 1 (G1)
    FLAG_GREEN_S2,          // Green settore 2 (G2)
    FLAG_GREEN_S3,          // Green settore 3 (G3)
    FLAG_GREEN_FS,          // Green settori 1 + 2 (GF)
    FLAG_GREEN_ST,          // Green settori 2 + 3 (GS)
    FLAG_GREEN_TF,          // Green settori 3 + 1 (GT)


    // --------------------------------------------------------
    //  RED / BLUE / YELLOW
    // --------------------------------------------------------
    FLAG_RED,               // Red flag

    FLAG_BLUE,              // Blue flag globale
    FLAG_BLUE_S1,           // Blue settore 1
    FLAG_BLUE_S2,           // Blue settore 2
    FLAG_BLUE_S3,           // Blue settore 3
    FLAG_BLUE_PIT,          // Blue PIT

    FLAG_YELLOW_S1,         // Yellow settore 1
    FLAG_YELLOW_S2,         // Yellow settore 2
    FLAG_YELLOW_S3,         // Yellow settore 3

    FLAG_YELLOW_FS,         // Settori 1 + 2
    FLAG_YELLOW_ST,         // Settori 2 + 3
    FLAG_YELLOW_TF,         // Settori 3 + 1

    // --------------------------------------------------------
    //  SAFETY
    // --------------------------------------------------------
    FLAG_SC,                // Safety Car
    FLAG_VSC,               // Virtual Safety Car

    // --------------------------------------------------------
    //  FINISH / CONDITIONS
    // --------------------------------------------------------
    FLAG_CHECKERED,         // Bandiera a scacchi
    FLAG_WET                // Wet Race
};



// ------------------------------------------------------------
//  Comandi PIT (ID 3–4)
// ------------------------------------------------------------

enum PitState {

    PIT_OFF = 0,            // Nessun segnale
    PIT_OPEN,               // Anello 1 verde
    PIT_CLOSE,              // Anello 2 rosso
    PIT_VALID               // Entrambi viola
};



// ------------------------------------------------------------
//  Comandi speciali Semaforo (ID 5)
// ------------------------------------------------------------

enum SemaforoState {

    SEM_NONE = 0,

    SEM_PRE_RACE,           // Tutto acceso
    SEM_PRE_10,             // Pre‑race 10m
    SEM_PRE_5,              // Pre‑race 5m
    SEM_PRE_2,              // Pre‑race 2m
    SEM_PRE_1,              // Pre‑race 1m

    SEM_START_SEQUENCE,     // Accensione progressiva 1→5 + random delay → LO
    SEM_START_SEQUENCE_MODE, // Accensione manuale 1→5 
    SEM_LIGHTS_OUT,         // Tutto spento (start)

    SEM1,                    // Semaforo 1 (anello 1)
    SEM2,                    // Semaforo 2 (anello 2)
    SEM3,                    // Semaforo 3 (anello 3)
    SEM4,                    // Semaforo 4 (anello 4)
    SEM5,                    // Semaforo 5 (anello 5)

    SEM_FORMATION_LAP       // Anelli 2 e 4 accesi
};



// ------------------------------------------------------------
//  Comandi di sistema
// ------------------------------------------------------------

enum SystemCommand {

    CMD_NONE = 0,

    CMD_CLEAR_ALL,          // Spegne tutto
    CMD_RESET_DEVICE,       // Reset software
    CMD_TEST_MODE,          // Test LED
    CMD_SHOW_ID             // Mostra ID dispositivo
};

#endif
