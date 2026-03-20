# 🏁 RaceDisplay

**Sistema modulare per bandiere, semafori e pannelli LED per motorsport**

![Platform Arduino](https://img.shields.io/badge/platform-Arduino-blue.svg)
![ESP32](https://img.shields.io/badge/ESP32-supported-green.svg)
![License MIT](https://img.shields.io/badge/license-MIT-lightgrey.svg)
![Version](https://img.shields.io/badge/version-1.2.7-blue)

RaceDisplay è una libreria C++ modulare per microcontrollori (ESP32 / ESP8266 / AVR) progettata per creare dispositivi LED professionali per il motorsport.

Gestisce matrici 8x8, anelli PIT, semafori completi e button box.

Il sistema è completamente **non bloccante**, **scalabile** e progettato per garantire animazioni fluide e protocolli di comando chiari e veloci.

---

## 🚀 Funzionalità principali

* **Supporto Multi-Device:**
  * Matrici LED 8x8 (ID 0–2)
  * PIT In / Out (ID 3–4)
  * Semaforo a 5 anelli (ID 5)
  * Button Box (ID 8)

* **Protocollo Comandi Avanzato**
  * Moderno (2 caratteri)
  * Compatibilità legacy (1 carattere)

* **Animazioni Non Bloccanti**
  * Nessun `delay()`
  * Loop sempre reattivo

* **Gestione Bandiere**
  * Green, Yellow, Blue, Red, Wet, Checkered
  * Safety Car (SC) e VSC animati

* **Semaforo Gara**
  * Start automatico (random delay)
  * Modalità manuale
  * Pre-race + Formation lap

---

## 📁 Struttura della Libreria

```
RaceDisplay/
│
├── RaceDisplay.h
├── FlagSettings.h
├── FlagTypes.h
├── Commands.h
│
├── AnimationEngine.h
├── FlagManager.h
│
├── MatrixDriver.h
├── RingDriver.h
│
├── Colors.h
└── RacePanel.h
```

---

## ⚙️ Configurazione

Imposta l'ID dispositivo in `FlagSettings.h`:

```cpp
#define DEVICE_ID 0
```

| ID | Tipo | Descrizione |
|----|------|------------|
| 0 | MATRIX | Settore 1 |
| 1 | MATRIX | Settore 2 |
| 2 | MATRIX | Settore 3 |
| 3 | PIT | Pit In |
| 4 | PIT | Pit Out |
| 5 | SEMAFORO | Start Lights |
| 8 | SENDER | Button Box |

---

## 🛰️ Protocollo Comandi

| Categoria | Comando | Descrizione |
|----------|--------|------------|
| Global | FG | Green |
| Global | FR | Red |
| Global | FB | Blue |
| Global | FC | Checkered |
| Settore | Y1 Y2 Y3 | Yellow |
| Settore | G1 G2 G3 | Green |
| Safety | SC | Safety Car |
| Safety | VS | Virtual SC |
| PIT | PO PC | Open / Closed |
| Start | SA | Start Auto |
| Start | LO | Lights Out |

---

## 📦 Installazione

### Metodo Arduino IDE

1. Scarica la release
2. Vai su:

```
Sketch → Include Library → Add .ZIP Library
```

---

## 🧪 Esempio base

```cpp
#include <RaceDisplay.h>

RaceDisplay display;

void setup() {
    display.begin();
}

void loop() {
    display.update();
    display.handleCommand("Y1");
}
```

---

## 📄 Licenza

Distribuito sotto licenza MIT.
