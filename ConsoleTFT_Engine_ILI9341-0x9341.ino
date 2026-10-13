#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include "config.h"

// -------------------------------------------------------
// CONVENZIONE NUMERAZIONE PROCEDURE:
//   90+  = modalità macro/auto  (slideshow, random, ecc.)
//   ≤ 20 = singola procedura/effetto
// -------------------------------------------------------
// 99 = coppia casuale di effetti ogni 60s
// 98 = slideshow sequenziale ogni 60s
//  n = esegui solo la procedura numero n (es. 6, 17...)
// -------------------------------------------------------
int procedure = 99;

MCUFRIEND_kbv tft;

#include <TEFX.h>
TEFX Row1("", 20);

void setup() {
  InitScreen();

  Serial.begin(19200);
  goodRandomseed();
}


// -------------------------------------------------------------
// LOOP PRINCIPALE
// -------------------------------------------------------------

// Dichiarazioni forward degli effetti atomici
void Stars_Array();
void SinDemo_1();
void SinDemo_2();
void LinkedPoints();
void Qix();
void QixAI();
void MoireBars();
void MoireScreen();
void Oldloop();      // OldProcedures.ino (usa delay() interni) — solo avvio fisso procedure=20

// Funzioni info overlay / schermata autonoma
void InfoTicker();
void InfoScreen();

// Array di puntatori agli effetti atomici disponibili per la modalità auto
typedef void (*FnEffetto)();
const FnEffetto effettiAuto[] = {
  Stars_Array,
  SinDemo_1,
  SinDemo_2,
  LinkedPoints,
  Qix,
  QixAI,
  MoireBars,
  MoireScreen,
};
const uint8_t TOT_EFFETTI = sizeof(effettiAuto) / sizeof(effettiAuto[0]);

void loop() {

  // PROCEDURA 99: Coppia casuale di effetti, cambia ogni 60 secondi
  if (procedure == 99) {
    static uint8_t  effA              = 0;   // Indice primo effetto attivo
    static uint8_t  effB              = 1;   // Indice secondo effetto attivo (diverso da effA)
    static uint32_t tempoUltimoCambio = 0;
    static bool     primaVolta        = true;

    // Al primo avvio o ogni 60 secondi: sceglie una nuova coppia casuale distinta
    if (primaVolta || (millis() - tempoUltimoCambio >= 60000UL)) {
      tempoUltimoCambio = millis();
      primaVolta = false;

      // Pulisce lo schermo per non lasciare tracce degli effetti precedenti
      tft.fillScreen(0x0000);

      // Sceglie effA casuale
      effA = (uint8_t)RandNum(0, TOT_EFFETTI - 1);

      // Sceglie effB diverso da effA
      do {
        effB = (uint8_t)RandNum(0, TOT_EFFETTI - 1);
      } while (effB == effA);

      // Debug opzionale su porta seriale
      Serial.print(F("Nuova coppia effetti: "));
      Serial.print(effA);
      Serial.print(F(" + "));
      Serial.println(effB);
    }

    // Esegue entrambi gli effetti della coppia corrente ad ogni iterazione del loop
    effettiAuto[effA]();
    effettiAuto[effB]();

    // Ticker con le stringhe Info in sovrimpressione sul fondo dello schermo
    InfoTicker();

  } else if (procedure == 98) {
    // PROCEDURA 98: Slideshow sequenziale ordinato, cambia ogni 60 secondi
    const uint8_t elencoDemo[] = {4, 5, 6, 7, 8, 9, 10, 11, 15, 16, 17};
    const uint8_t totDemo      = sizeof(elencoDemo) / sizeof(elencoDemo[0]);

    static uint8_t  indiceAttuale     = 0;
    static uint32_t tempoUltimoCambio = 0;

    if (millis() - tempoUltimoCambio >= 60000UL) {
      tempoUltimoCambio = millis();

      tft.fillScreen(0x0000);

      // Avanza in sequenza, torna a 0 alla fine
      indiceAttuale = (indiceAttuale + 1) % totDemo;

      Serial.print(F("Procedura sequenziale: "));
      Serial.println(elencoDemo[indiceAttuale]);
    }

    eseguiProcedura(elencoDemo[indiceAttuale]);

    // Ticker con le stringhe Info in sovrimpressione sul fondo dello schermo
    InfoTicker();

  } else if (procedure == 17) {
    // PROCEDURA 17: Schermata autonoma con le stringhe Info + bordo animato
    InfoScreen();

  } else {
    // Esecuzione fissa della singola procedura selezionata
    eseguiProcedura(procedure);
  }

}