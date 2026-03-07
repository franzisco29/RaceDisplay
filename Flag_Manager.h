#ifndef FLAG_MANAGER_H
#define FLAG_MANAGER_H

//
// ============================================================
//  FlagManager.h
//  Gestione logica bandiere e routing dispositivi RaceDisplay
//  Strutturato per repository Git (chiaro, modulare, stabile)
// ============================================================
//
//  Questo file gestisce:
//   - Interpretazione comandi (stringhe)
//   - Routing bandiere verso matrici, PIT, semaforo
//   - Attivazione animazioni (SC, VSC, Yellow, Checkered, ecc.)
//   - Gestione stati speciali (start, formation lap)
//   - Clear e reset
//
//  NON disegna LED: delega a MatrixDriver e RingDriver.
//  NON gestisce animazioni: delega a AnimationEngine.
//
// ============================================================
//

#include "FlagSettings.h"
#include "FlagTypes.h"
#include "MatrixDriver.h"
#include "RingDriver.h"
#include "AnimationEngine.h"
#include "colors.h"



// ------------------------------------------------------------
//  Stato interno
// ------------------------------------------------------------

namespace FlagManager {

    static FlagType currentFlag = FLAG_NONE;
    static PitState currentPit = PIT_OFF;
    static SemaforoState currentSem = SEM_NONE;
}



// ------------------------------------------------------------
//  Clear completo del dispositivo
// ------------------------------------------------------------

static void ClearAll() {

    FlagManager::currentFlag = FLAG_NONE;
    FlagManager::currentPit  = PIT_OFF;
    FlagManager::currentSem  = SEM_NONE;

    if (DEVICE_TYPE == DEVICE_TYPE_MATRIX)
        MatrixClear();

    else if (DEVICE_TYPE == DEVICE_TYPE_PIT)
        PitShow(PIT_OFF);

    else if (DEVICE_TYPE == DEVICE_TYPE_SEMAFORO)
        SemaforoShowLightsOut();

    FastLED.show();
}



// ------------------------------------------------------------
//  Mostra bandiera su dispositivo corrente
// ------------------------------------------------------------

static void ShowFlag(FlagType flag) {

    FlagManager::currentFlag = flag;
    AnimationStart(flag);

    if (DEVICE_TYPE == DEVICE_TYPE_MATRIX)
        MatrixShowFlag(flag);

    else if (DEVICE_TYPE == DEVICE_TYPE_SEMAFORO)
        SemaforoShowFlag(flag);

    FastLED.show();
}



// ------------------------------------------------------------
//  Mostra stato PIT (ID 3–4)
// ------------------------------------------------------------

static void ShowPit(PitState state) {

    if (DEVICE_TYPE != DEVICE_TYPE_PIT)
        return;

    FlagManager::currentPit = state;
    PitShow(state);
}



// ------------------------------------------------------------
//  Mostra stato semaforo (ID 5)
// ------------------------------------------------------------

static void ShowSemaforo(SemaforoState state) {

    if (DEVICE_TYPE != DEVICE_TYPE_SEMAFORO)
        return;

    FlagManager::currentSem = state;
    AnimationStartSem(state);

    switch(state) {

        case SEM_PRE_RACE:
        case SEM_PRE_10:
        case SEM_PRE_5:
        case SEM_PRE_2:
        case SEM_PRE_1:
            SemaforoShowLightsOut();
            break;

        case SEM_START_SEQUENCE:
            // Sequenza gestita da AnimationEngine
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



// ------------------------------------------------------------
//  Interpretazione comandi (stringhe)
// ------------------------------------------------------------
//
//  Esempi:
//    "GREEN"
//    "RED"
//    "YELLOW S1"
//    "SC"
//    "VSC"
//    "CHECK"
//    "PIT OPEN"
//    "PIT CLOSE"
//    "START"
//    "FORMATION"
//    "CLEAR"
//

static void handleCommand(const String& cmd) {

    String c = cmd;
    c.trim();
    c.toUpperCase();


    // --------------------------------------------------------
    //  Clear
    // --------------------------------------------------------
    if (c == "CLEAR") {
        ClearAll();
        return;
    }


    // --------------------------------------------------------
    //  PIT (solo ID 3–4)
    // --------------------------------------------------------
    if (c.startsWith("PIT")) {

        if (c == "PIT OPEN")  ShowPit(PIT_OPEN);
        if (c == "PIT CLOSE") ShowPit(PIT_CLOSE);
        if (c == "PIT VALID") ShowPit(PIT_VALID);
        if (c == "PIT OFF")   ShowPit(PIT_OFF);

        return;
    }


    // --------------------------------------------------------
    //  Semaforo (solo ID 5)
    // --------------------------------------------------------
    if (c.startsWith("START")) {
        ShowSemaforo(SEM_START_SEQUENCE);
        return;
    }

    if (c == "FORMATION") {
        ShowSemaforo(SEM_FORMATION_LAP);
        return;
    }


    // --------------------------------------------------------
    //  Bandiere generali
    // --------------------------------------------------------

    if (c == "GREEN") ShowFlag(FLAG_GREEN);
    else if (c == "RED") ShowFlag(FLAG_RED);
    else if (c == "BLUE") ShowFlag(FLAG_BLUE);
    else if (c == "CHECK") ShowFlag(FLAG_CHECKERED);
    else if (c == "WET") ShowFlag(FLAG_WET);

    else if (c == "SC") ShowFlag(FLAG_SC);
    else if (c == "VSC") ShowFlag(FLAG_VSC);

    else if (c == "YELLOW S1") ShowFlag(FLAG_YELLOW_S1);
    else if (c == "YELLOW S2") ShowFlag(FLAG_YELLOW_S2);
    else if (c == "YELLOW S3") ShowFlag(FLAG_YELLOW_S3);

    else if (c == "YELLOW FS") ShowFlag(FLAG_YELLOW_FS);
    else if (c == "YELLOW ST") ShowFlag(FLAG_YELLOW_ST);
    else if (c == "YELLOW TF") ShowFlag(FLAG_YELLOW_TF);
}



// ------------------------------------------------------------
//  Update non bloccante
// ------------------------------------------------------------

static void update() {
    AnimationUpdate();
}



#endif
