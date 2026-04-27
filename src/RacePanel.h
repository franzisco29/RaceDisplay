#ifndef RACE_PANEL_H
#define RACE_PANEL_H

#include <Arduino.h>
#include "Commands.h"
#include "FlagSettings.h"

// ============================================================
//  RacePanel
//  Gestione dispositivi DeviceSender (button box)
// ============================================================

#if DEVICE_TYPE == DEVICE_TYPE_SENDER

class RacePanel {

public:

    // --------------------------------------------------------
    //  Configurazione hardware pulsanti
    // --------------------------------------------------------
    static constexpr uint8_t PINS[] = { 2, 3, 4, 5, 6, 7, 9, 10, 20, 21 };
    static constexpr uint8_t N = sizeof(PINS);

    // Mappa comandi → usa commands.h
    static constexpr const char* CMDS[] = {
        CMD_SAFETY_CAR,   // SC  (PIN 2)
        CMD_YELLOW_S1,    // Y1  (PIN 3)
        CMD_GREEN,        // FG  (PIN 4)
        CMD_RED,          // FR  (PIN 5)
        CMD_CLEAR_ALL,    // CL  (PIN 6)
        CMD_YELLOW_S3,    // Y3  (PIN 7)
        CMD_START_PROC,   // SP  (PIN 9, toggle con LO)
        CMD_WET,          // FW  (PIN 10)
        CMD_VIRTUAL_SC,   // VS  (PIN 20)
        CMD_YELLOW_S2     // Y2  (PIN 21)
    };

    // Toggle SP/LO
    static constexpr uint8_t TOGGLE_IDX = 6;
    static constexpr const char* TOGGLE_CMDS[2] = { CMD_START_PROC, CMD_LIGHTS_OUT };

    // Debounce
    static constexpr uint16_t DEBOUNCE_MS = 30;

private:
    bool     state[N];
    uint32_t last[N];
    uint8_t  toggleState = 0;

    // 🔥 Ultimo comando premuto
    const char* lastCommand = nullptr;

public:

    // --------------------------------------------------------
    //  Setup
    // --------------------------------------------------------
    void begin() {
        for (uint8_t i = 0; i < N; i++) {
            pinMode(PINS[i], INPUT_PULLUP);
            state[i] = HIGH;
            last[i]  = 0;
        }
    }

    // --------------------------------------------------------
    //  Loop non bloccante
    // --------------------------------------------------------
    void update() {
        uint32_t now = millis();

        for (uint8_t i = 0; i < N; i++) {

            bool r = digitalRead(PINS[i]);

            if (r != state[i] && now - last[i] > DEBOUNCE_MS) {
                last[i]  = now;
                state[i] = r;

                if (r == LOW) {
                    if (i == TOGGLE_IDX) {
                        lastCommand = TOGGLE_CMDS[toggleState];
                        toggleState ^= 1;
                    } else {
                        lastCommand = CMDS[i];
                    }
                }
            }
        }
    }

    // --------------------------------------------------------
    //  Recupera l’ultimo comando premuto
    // --------------------------------------------------------
    const char* getLastCommand() const {
        return lastCommand;
    }

    // --------------------------------------------------------
    //  Cancella il comando dopo l’invio
    // --------------------------------------------------------
    void clearLastCommand() {
        lastCommand = nullptr;
    }
};

#endif // DEVICE_TYPE_SENDER

#endif // RACE_PANEL_H