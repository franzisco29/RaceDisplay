#ifndef COMMANDS_H
#define COMMANDS_H

// ------------------------------------------------------------
//  CONNESSIONE
// ------------------------------------------------------------
#define CMD_CONNECT        "CN"
#define CMD_DISCONNECT     "DC"
#define CMD_STATUS         "ST"
#define CMD_CONFIG         "CF"


// ------------------------------------------------------------
//  BANDIERE BASE
// ------------------------------------------------------------
#define CMD_GREEN          "FG"   // Green globale (tutti i device)
#define CMD_RED            "FR"
#define CMD_BLUE           "FB"
#define CMD_WET            "FW"
#define CMD_DRY            "FD"
#define CMD_CHECKERED      "FC"


// ------------------------------------------------------------
//  GREEN SETTORIALI + SPECIALI
// ------------------------------------------------------------
// G1/G2/G3 → verde solo nel settore richiesto (solo matrici)
// GF/GS/GT → esattamente come YF/YS/YT ma in verde
//   GF → settori 1 + 2
//   GS → settori 2 + 3
//   GT → settori 3 + 1

#define CMD_GREEN_S1       "G1"
#define CMD_GREEN_S2       "G2"
#define CMD_GREEN_S3       "G3"
#define CMD_GREEN_FS       "GF"   // S1 + S2
#define CMD_GREEN_ST       "GS"   // S2 + S3
#define CMD_GREEN_TF       "GT"   // S3 + S1


// ------------------------------------------------------------
//  YELLOW SETTORIALI
// ------------------------------------------------------------
#define CMD_YELLOW_S1      "Y1"
#define CMD_YELLOW_S2      "Y2"
#define CMD_YELLOW_S3      "Y3"
#define CMD_YELLOW_FS      "YF"   // S1 + S2
#define CMD_YELLOW_ST      "YS"   // S2 + S3
#define CMD_YELLOW_TF      "YT"   // S3 + S1


// ------------------------------------------------------------
//  BLUE SETTORIALI + PIT
// ------------------------------------------------------------
#define CMD_BLUE_S1        "B1"
#define CMD_BLUE_S2        "B2"
#define CMD_BLUE_S3        "B3"
#define CMD_BLUE_PIT       "BP"


// ------------------------------------------------------------
//  SAFETY
// ------------------------------------------------------------
#define CMD_SAFETY_CAR     "SC"
#define CMD_VIRTUAL_SC     "VS"


// ------------------------------------------------------------
//  PIT
// ------------------------------------------------------------
#define CMD_PIT_OPEN       "PO"
#define CMD_PIT_CLOSE      "PC"
#define CMD_PIT_VALID      "PV"
#define CMD_PIT_OFF        "PF"


// ------------------------------------------------------------
//  SEMAFORO - START SEQUENCE
// ------------------------------------------------------------
#define CMD_LIGHTS_OUT     "LO"   // Spegne le luci
#define CMD_START_PROC     "SP"   // Solo per semaforo: avvia la sequenza di partenza (accensione S1..S5 ogni 1 secondo, senza random delay iniziale)
#define CMD_START_AUTO     "SA"   // Solo per Semaforo: avvia la sequenza di partenza con animazione automatica (non serve inviare S1..S5) e va in random delay tra 0 e 3 secondi per simulare la variabilità reale del semaforo

// ------------------------------------------------------------
//  PRE-GARA
// ------------------------------------------------------------
#define CMD_FORMATION_LAP  "FL"
#define CMD_PRE_RACE       "PR"


// Countdown pre-gara (mapping legacy)
#define CMD_PRE_10         "P0"
#define CMD_PRE_5          "P5"
#define CMD_PRE_2          "P2"
#define CMD_PRE_1          "P1"


// ------------------------------------------------------------
//  LUCI DI PARTENZA (accensione singola S1–S5)
// ------------------------------------------------------------
#define CMD_START_LIGHT_1  "S1"
#define CMD_START_LIGHT_2  "S2"
#define CMD_START_LIGHT_3  "S3"
#define CMD_START_LIGHT_4  "S4"
#define CMD_START_LIGHT_5  "S5"


// ------------------------------------------------------------
//  SISTEMA
// ------------------------------------------------------------
#define CMD_CLEAR_ALL      "CL"
#define CMD_CLEAR_YELLOW   "CY"
#define CMD_CLEAR_BLUE     "CB"
#define CMD_CLEAR_BLUE_S1   "1B"
#define CMD_CLEAR_BLUE_S2   "2B"
#define CMD_CLEAR_BLUE_S3   "3B"




// ------------------------------------------------------------
//  LEGACY COMPATIBILITY (protocollo storico 1-char)
// ------------------------------------------------------------
#define CONN_CMD           'C'
#define DSCN_CMD           'c'

#define LIGHTS_OUT_CMD     'S'
#define START_PROC_CMD     's'

#define GREEN_FLAG_CMD     'G'
#define RED_FLAG_CMD       'R'
#define BLUE_FLAG_CMD      'B'

#define YELLOW_F_CMD       '1'
#define YELLOW_S_CMD       '2'
#define YELLOW_T_CMD       '3'
#define YELLOW_FS_CMD      '4'
#define YELLOW_ST_CMD      '5'
#define YELLOW_TF_CMD      '6'

#define WET_CMD            'W'

#define SAFETY_CAR_CMD     'Y'
#define FULL_YELLOW_CMD    'F'

#define PIT_CLOSER_CMD     'P'
#define PIT_OPEN_CMD       'O'
#define PIT_VALID_CMD      'V'

#define END_SESSION_CMD    'E'

#define CLC_CMD            'd'
#define CLC_YELLOW_CMD     'T'

#define PRE_RACE_CMD       'p'
#define PRE10_CMD          '7'
#define PRE5_CMD           '8'
#define PRE2_CMD           '9'
#define PRE1_CMD           '0'
#define FORMATION_LAP_CMD  'L'

#define BRIEFING_CMD       'b'

#define MENU_CMD           'M'
#define STATUS_CMD_LEGACY  'A'

#endif
