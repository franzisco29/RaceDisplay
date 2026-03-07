
# RaceDisplay Library

RaceDisplay è una libreria modulare per la gestione di display LED dedicati al motorsport.  
Supporta matrici 8×8, anelli PIT e un semaforo a 5 anelli, con logica completa per bandiere, procedure di gara e animazioni non bloccanti.

Progettata per essere **scalabile**, **chiara**, **collaborativa** e perfetta per progetti open‑source.

---

## 🚦 Dispositivi supportati

| ID | Tipo | Descrizione |
|----|------|-------------|
| **0** | Matrice | Lap Done – mostra numeri e bandiere (Yellow S1) |
| **1** | Matrice | Settore 1 – mostra bandiere (Yellow S2) |
| **2** | Matrice | Settore 2 – mostra bandiere (Yellow S3) |
| **3** | PIT | Pit In – 2 anelli (Open/Close/Valid) |
| **4** | PIT | Pit Out – 2 anelli (Open/Close/Valid) |
| **5** | Semaforo | 5 anelli – bandiere, start, formation lap |

---

## 🟨 Bandiere supportate

- Green  
- Red  
- Blue  
- Yellow S1 / S2 / S3  
- Yellow FS / ST / TF  
- SC (S↔C)  
- VSC (V→S→C)  
- Checkered  
- Wet Race  

---

## 🟦 Funzioni speciali

### Semaforo (ID 5)
- Starting Procedure (accensione progressiva 1→5)
- Lights Out
- Formation Lap (anelli 2 e 4)
- Pre‑Race (PRE10, PRE5, PRE2, PRE1)

### PIT (ID 3–4)
- Pit Open (verde)
- Pit Close (rosso)
- Pit Valid (viola)
- Pit Off

---

## 🧩 Architettura della libreria

```
RaceDisplay/
│
├── RaceDisplay.h        → Entry point della libreria
├── FlagSettings.h       → Configurazione hardware e parametri
├── FlagTypes.h          → Enumerazioni bandiere e stati
├── colors.h             → Palette colori
├── MatrixDriver.h       → Rendering matrici 8×8
├── RingDriver.h         → Rendering anelli PIT e semaforo
├── AnimationEngine.h    → Motore animazioni non bloccanti
└── FlagManager.h        → Logica bandiere e routing
```

---

## 🧪 Esempio di utilizzo

```cpp
#include "RaceDisplay.h"

RaceDisplay display;

void setup() {
    display.begin();
}

void loop() {
    display.update();

    // Esempio: mostra bandiera verde
    // display.handleCommand("GREEN");
}
```

---

## 🛠 Comandi disponibili

### Bandiere
```
GREEN
RED
BLUE
CHECK
WET
SC
VSC
YELLOW S1
YELLOW S2
YELLOW S3
YELLOW FS
YELLOW ST
YELLOW TF
```

### PIT
```
PIT OPEN
PIT CLOSE
PIT VALID
PIT OFF
```

### Semaforo
```
START
FORMATION
```

### Sistema
```
CLEAR
```

---

## 📄 Licenza
MIT License (o quella che preferisci).

---

## 👤 Autore
Francesco — Race Display System  
Progetto modulare, open‑source, pensato per motorsport education & development.
```

