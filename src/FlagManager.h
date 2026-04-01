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

    static FlagType currentFlag = FLAG_NONE;
    static PitState currentPit = PIT_OFF;
    static SemaforoState currentSem = SEM_NONE;


    // ------------------------------------------------------------
    //  CLEAR ALL
    // ------------------------------------------------------------
    static void clearAll() {

        AnimationEngine::start(FLAG_NONE);
        AnimationEngine::startSem(SEM_NONE);

        currentFlag = FLAG_NONE;
        currentPit  = PIT_OFF;
        currentSem  = SEM_NONE;

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
    //  SHOW FLAG (tutta la logica è in AnimationEngine)
    // ------------------------------------------------------------
    static void showFlag(FlagType flag) {

        // --------------------------------------------------------
        //  FILTRO VERDI SETTORIALI (G1/G2/G3/GF/GS/GT)
        // --------------------------------------------------------
        if (flag == FLAG_GREEN_S1 && DEVICE_ID != 0) return;
        if (flag == FLAG_GREEN_S2 && DEVICE_ID != 1) return;
        if (flag == FLAG_GREEN_S3 && DEVICE_ID != 2) return;

        if (flag == FLAG_GREEN_FS && !(DEVICE_ID == 0 || DEVICE_ID == 1)) return;
        if (flag == FLAG_GREEN_ST && !(DEVICE_ID == 1 || DEVICE_ID == 2)) return;
        if (flag == FLAG_GREEN_TF && !(DEVICE_ID == 2 || DEVICE_ID == 0)) return;

        // --------------------------------------------------------
        //  SE IL FLAG È DESTINATO AL DEVICE → MOSTRALO
        // --------------------------------------------------------
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

        if (DEVICE_TYPE == DEVICE_TYPE_MATRIX)
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

        // L’unico responsabile delle animazioni è AnimationEngine
        AnimationEngine::startSem(state);

        

        // Nessuna chiamata diretta a SemaforoShowXXXX()
    }



    // ------------------------------------------------------------
    //  HANDLE COMMAND
    // ------------------------------------------------------------
    static void handleCommand(const String& cmd) {

        String c = cmd;
        c.trim();

        // ------------------------------------------------------------
        //  LEGACY 1-char
        // ------------------------------------------------------------
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

        // ------------------------------------------------------------
        //  PROTOCOLLO MODERNO 2-CARATTERI
        // ------------------------------------------------------------


        // Verde globale (FG)
        if (c == CMD_GREEN)          { showFlag(FLAG_GREEN); return; }

        // Nuovi verdi settoriali singoli
        if (c == CMD_GREEN_S1)       { showFlag(FLAG_GREEN_S1); return; }
        if (c == CMD_GREEN_S2)       { showFlag(FLAG_GREEN_S2); return; }
        if (c == CMD_GREEN_S3)       { showFlag(FLAG_GREEN_S3); return; }

        // Verdi doppi (esatti analoghi di YF/YS/YT)
        if (c == CMD_GREEN_FS)       { showFlag(FLAG_GREEN_FS); return; }  // GF → S1+S2
        if (c == CMD_GREEN_ST)       { showFlag(FLAG_GREEN_ST); return; }  // GS → S2+S3
        if (c == CMD_GREEN_TF)       { showFlag(FLAG_GREEN_TF); return; }  // GT → S3+1


        // Bandiere base
        if (c == CMD_RED)            { showFlag(FLAG_RED); return; }
        if (c == CMD_BLUE)           { showFlag(FLAG_BLUE); return; }
        if (c == CMD_WET)            { showFlag(FLAG_WET); return; }
        if (c == CMD_DRY)            { showFlag(FLAG_DRY); return; }
        if (c == CMD_CHECKERED)      { showFlag(FLAG_CHECKERED); return; }

        // Yellow settoriali
        if (c == CMD_YELLOW_S1)      { showFlag(FLAG_YELLOW_S1); return; }
        if (c == CMD_YELLOW_S2)      { showFlag(FLAG_YELLOW_S2); return; }
        if (c == CMD_YELLOW_S3)      { showFlag(FLAG_YELLOW_S3); return; }
        if (c == CMD_YELLOW_FS)      { showFlag(FLAG_YELLOW_FS); return; }
        if (c == CMD_YELLOW_ST)      { showFlag(FLAG_YELLOW_ST); return; }
        if (c == CMD_YELLOW_TF)      { showFlag(FLAG_YELLOW_TF); return; }

        // Blue settoriali
        if (c == CMD_BLUE_S1)        { showFlag(FLAG_BLUE_S1); return; }
        if (c == CMD_BLUE_S2)        { showFlag(FLAG_BLUE_S2); return; }
        if (c == CMD_BLUE_S3)        { showFlag(FLAG_BLUE_S3); return; }
        if (c == CMD_BLUE_PIT)       { showFlag(FLAG_BLUE_PIT); return; }

        // Safety
        if (c == CMD_SAFETY_CAR)     { showFlag(FLAG_SC); return; }
        if (c == CMD_VIRTUAL_SC)     { showFlag(FLAG_VSC); return; }

        // Pit
        if (c == CMD_PIT_OPEN)       { showPit(PIT_OPEN); return; }
        if (c == CMD_PIT_CLOSE)      { showPit(PIT_CLOSE); return; }
        if (c == CMD_PIT_VALID)      { showPit(PIT_VALID); return; }
        if (c == CMD_PIT_OFF)        { showPit(PIT_OFF); return; }

        // Semaforo
        if (c == CMD_LIGHTS_OUT)     { showSemaforo(SEM_LIGHTS_OUT); return; }
        if (c == CMD_START_AUTO)     { showSemaforo(SEM_START_SEQUENCE); return; }
        if (c == CMD_START_PROC)     { showSemaforo(SEM_START_SEQUENCE_MODE); return; }
        if (c == CMD_FORMATION_LAP)  { showSemaforo(SEM_FORMATION_LAP); return; }

        if (c == CMD_PRE_RACE)       { showSemaforo(SEM_PRE_RACE); return; }

        if (c == CMD_PRE_10)          { showSemaforo(SEM_PRE_10); return; }
        if (c == CMD_PRE_5)           { showSemaforo(SEM_PRE_5); return; }
        if (c == CMD_PRE_2)           { showSemaforo(SEM_PRE_2); return; }
        if (c == CMD_PRE_1)           { showSemaforo(SEM_PRE_1); return; }

        // Start lights (mapping custom)
        if (c == CMD_START_LIGHT_1)  { showSemaforo(SEM1); return; }
        if (c == CMD_START_LIGHT_2)  { showSemaforo(SEM2); return; }
        if (c == CMD_START_LIGHT_3)  { showSemaforo(SEM3); return; }
        if (c == CMD_START_LIGHT_4)  { showSemaforo(SEM4); return; }
        if (c == CMD_START_LIGHT_5)  { showSemaforo(SEM5); return; }

        // Clear
        if (c == CMD_CLEAR_ALL)      { clearAll(); return; }
        if (c == CMD_CLEAR_YELLOW)   { showFlag(FLAG_NONE); return; }
        

        // ------------------------------------------------------------
        //  CLEAR BLUE SETTORIALE (b1/b2/b3)
        // ------------------------------------------------------------
        if (c == CMD_CLEAR_BLUE_S1) {
            if (DEVICE_ID == 0 &&
                (currentFlag == FLAG_BLUE_S1 || currentFlag == FLAG_BLUE))
            {
                showFlag(FLAG_NONE);
            }
            return;
        }

        if (c == CMD_CLEAR_BLUE_S2) {
            if (DEVICE_ID == 1 &&
                (currentFlag == FLAG_BLUE_S2 || currentFlag == FLAG_BLUE))
            {
                showFlag(FLAG_NONE);
            }
            return;
        }

        if (c == CMD_CLEAR_BLUE_S3) {
            if (DEVICE_ID == 2 &&
                (currentFlag == FLAG_BLUE_S3 || currentFlag == FLAG_BLUE))
            {
                showFlag(FLAG_NONE);
            }
            return;
        }



        // ------------------------------------------------------------
        //  COMANDI TESTUALI (fallback)
        // ------------------------------------------------------------
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
    //  UPDATE
    // ------------------------------------------------------------
    static void update() {
        AnimationEngine::update();
    }
}


// Compatibility wrappers
static inline void ClearAll() { FlagManager::clearAll(); }
static inline void ShowFlag(FlagType flag) { FlagManager::showFlag(flag); }
static inline void ShowPit(PitState state) { FlagManager::showPit(state); }
static inline void ShowSemaforo(SemaforoState state) { FlagManager::showSemaforo(state); }

#endif
