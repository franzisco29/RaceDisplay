#ifndef FLAG_MANAGER_H
#define FLAG_MANAGER_H

#include "FlagSettings.h"
#include "FlagTypes.h"
#include "MatrixDriver.h"
#include "RingDriver.h"
#include "AnimationEngine.h"
#include "Colors.h"
#include "Commands.h"

namespace FlagManager {

    // Stati correnti
    static FlagType currentFlag = FLAG_NONE;
    static PitState currentPit = PIT_OFF;
    static SemaforoState currentSem = SEM_NONE;

    // Gestione speciale GREEN FLAG
    static bool greenActive = false;
    static unsigned long greenStartTime = 0;


    // ------------------------------------------------------------
    //  CLEAR ALL
    // ------------------------------------------------------------
    static void clearAll() {

        AnimationEngine::start(FLAG_NONE);
        AnimationEngine::startSem(SEM_NONE);

        currentFlag = FLAG_NONE;
        currentPit  = PIT_OFF;
        currentSem  = SEM_NONE;
        greenActive = false;

        if (DEVICE_TYPE == DEVICE_TYPE_MATRIX) {
            MatrixClear();
        } 
        else if (DEVICE_TYPE == DEVICE_TYPE_PIT) {
            PitShow(PIT_OFF);
        } 
        else if (DEVICE_TYPE == DEVICE_TYPE_SEMAFORO) {
            SemaforoShowLightsOut();
        }

        FastLED.show();
    }


    // ------------------------------------------------------------
    //  SHOW FLAG (GREEN gestita separatamente)
    // ------------------------------------------------------------
    static void showFlag(FlagType flag) {

        // GREEN FLAG → fissa 3 secondi
        if (flag == FLAG_GREEN) {
            greenActive = true;
            greenStartTime = millis();
            currentFlag = FLAG_GREEN;

            // Mostra la green UNA SOLA VOLTA
            if (DEVICE_TYPE == DEVICE_TYPE_MATRIX) {
                MatrixShowFlag(FLAG_GREEN, false);
            } 
            else if (DEVICE_TYPE == DEVICE_TYPE_SEMAFORO) {
                SemaforoShowFlag(FLAG_GREEN);
            }

            FastLED.show(); // un solo refresh
            return;
        }

        // Tutte le altre bandiere → reset green
        greenActive = false;

        currentFlag = flag;
        AnimationEngine::start(flag);

        if (DEVICE_TYPE == DEVICE_TYPE_MATRIX) {
            MatrixShowFlag(flag);
        } 
        else if (DEVICE_TYPE == DEVICE_TYPE_SEMAFORO) {
            SemaforoShowFlag(flag);
        }

        FastLED.show();
    }


    // ------------------------------------------------------------
    //  PIT
    // ------------------------------------------------------------
    static void showPit(PitState state) {

        if (DEVICE_TYPE != DEVICE_TYPE_PIT)
            return;

        currentPit = state;
        PitShow(state);
    }


    // ------------------------------------------------------------
    //  SEMAFORO
    // ------------------------------------------------------------
    static void showSemaforo(SemaforoState state) {

        if (DEVICE_TYPE != DEVICE_TYPE_SEMAFORO)
            return;

        currentSem = state;
        AnimationEngine::startSem(state);

        switch (state) {
            case SEM_PRE_RACE:
            case SEM_PRE_10:
            case SEM_PRE_5:
            case SEM_PRE_2:
            case SEM_PRE_1:
            case SEM_LIGHTS_OUT:
                SemaforoShowLightsOut();
                break;

            case SEM_FORMATION_LAP:
                SemaforoShowFormationLap();
                break;

            case SEM_START_SEQUENCE:
            default:
                break;
        }
    }


    // ------------------------------------------------------------
    //  HANDLE COMMAND
    // ------------------------------------------------------------
    static void handleCommand(const String& cmd) {

        String c = cmd;
        c.trim();

        if (c.length() == 1) {
            char ch = c[0];

            switch (ch) {
                case GREEN_FLAG_CMD:      showFlag(FLAG_GREEN); return;
                case RED_FLAG_CMD:        showFlag(FLAG_RED); return;
                case YELLOW_F_CMD:        showFlag(FLAG_YELLOW_S1); return;
                case YELLOW_S_CMD:        showFlag(FLAG_YELLOW_S2); return;
                case YELLOW_T_CMD:        showFlag(FLAG_YELLOW_S3); return;
                case YELLOW_FS_CMD:       showFlag(FLAG_YELLOW_FS); return;
                case YELLOW_ST_CMD:       showFlag(FLAG_YELLOW_ST); return;
                case YELLOW_TF_CMD:       showFlag(FLAG_YELLOW_TF); return;
                case WET_CMD:             showFlag(FLAG_WET); return;
                case SAFETY_CAR_CMD:      showFlag(FLAG_SC); return;
                case FULL_YELLOW_CMD:     showFlag(FLAG_VSC); return;
                case PIT_OPEN_CMD:        showPit(PIT_OPEN); return;
                case PIT_CLOSER_CMD:      showPit(PIT_CLOSE); return;
                case PIT_VALID_CMD:       showPit(PIT_VALID); return;
                case END_SESSION_CMD:     showFlag(FLAG_CHECKERED); return;
                case CLC_CMD:             clearAll(); return;
                case FORMATION_LAP_CMD:   showSemaforo(SEM_FORMATION_LAP); return;
                case LIGHTS_OUT_CMD:      showSemaforo(SEM_LIGHTS_OUT); return;
                case START_PROC_CMD:      showSemaforo(SEM_START_SEQUENCE); return;
                case BLUE_FLAG_CMD:       showFlag(FLAG_BLUE); return;
                default: break;
            }
        }

        c.toUpperCase();

        if (c == "CLEAR") {
            clearAll();
            return;
        }

        if (c.startsWith("PIT")) {
            if (c == "PIT OPEN")  showPit(PIT_OPEN);
            if (c == "PIT CLOSE") showPit(PIT_CLOSE);
            if (c == "PIT VALID") showPit(PIT_VALID);
            if (c == "PIT OFF")   showPit(PIT_OFF);
            return;
        }

        if (c.startsWith("START")) {
            showSemaforo(SEM_START_SEQUENCE);
            return;
        }

        if (c == "FORMATION") {
            showSemaforo(SEM_FORMATION_LAP);
            return;
        }

        if (c == "GREEN") showFlag(FLAG_GREEN);
        else if (c == "RED") showFlag(FLAG_RED);
        else if (c == "BLUE") showFlag(FLAG_BLUE);
        else if (c == "CHECK") showFlag(FLAG_CHECKERED);
        else if (c == "WET") showFlag(FLAG_WET);
        else if (c == "SC") showFlag(FLAG_SC);
        else if (c == "VSC") showFlag(FLAG_VSC);
        else if (c == "YELLOW S1") showFlag(FLAG_YELLOW_S1);
        else if (c == "YELLOW S2") showFlag(FLAG_YELLOW_S2);
        else if (c == "YELLOW S3") showFlag(FLAG_YELLOW_S3);
        else if (c == "YELLOW FS") showFlag(FLAG_YELLOW_FS);
        else if (c == "YELLOW ST") showFlag(FLAG_YELLOW_ST);
        else if (c == "YELLOW TF") showFlag(FLAG_YELLOW_TF);
    }


    // ------------------------------------------------------------
    //  UPDATE (GREEN non ridisegna → zero lag)
    // ------------------------------------------------------------
    static void update() {

        // GREEN FLAG attiva → fissa per 3 secondi
        if (greenActive) {

            // NON ridisegnare la green ogni frame!
            // È già stata mostrata in showFlag()

            if (millis() - greenStartTime >= 3000) {
                greenActive = false;
                currentFlag = FLAG_NONE;

                if (DEVICE_TYPE == DEVICE_TYPE_MATRIX) {
                    MatrixClear();
                } 
                else if (DEVICE_TYPE == DEVICE_TYPE_SEMAFORO) {
                    SemaforoShowLightsOut();
                }

                FastLED.show(); // un solo refresh
            }

            return; // evita interferenze
        }

        // Normale gestione animazioni
        AnimationEngine::update();
    }
}


// Compatibility wrappers
static inline void ClearAll() { FlagManager::clearAll(); }
static inline void ShowFlag(FlagType flag) { FlagManager::showFlag(flag); }
static inline void ShowPit(PitState state) { FlagManager::showPit(state); }
static inline void ShowSemaforo(SemaforoState state) { FlagManager::showSemaforo(state); }

#endif
