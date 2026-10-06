#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include "config.h"

// Imposta 99 per attivare la rotazione automatica ogni minuto,
// oppure metti il singolo numero (es. 6) per vedere solo quella demo fissa.
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
void loop() {

  // PROCEDURA 99: Slideshow automatico ogni 1 minuto
  if (procedure == 99) {
    // Elenco delle procedure da far ruotare in sequenza
    const uint8_t elencoDemo[] = {4,5, 6, 7, 8, 9, 10, 11, 15, 20};
    const uint8_t totDemo      = sizeof(elencoDemo) / sizeof(elencoDemo[0]);

    static uint8_t  indiceAttuale   = 0;
    static uint32_t tempoUltimoCambio = 0;

    // 60000UL = 60 secondi (1 minuto). Il suffisso 'UL' evita overflow su Arduino Uno a 16-bit
    if (millis() - tempoUltimoCambio >= 60000UL) {
      tempoUltimoCambio = millis();

      // Pulisce lo schermo per non lasciare tracce della demo precedente
      tft.fillScreen(0x0000);

      // Passa alla demo successiva (torna a 0 quando arriva alla fine)
      indiceAttuale = (indiceAttuale + 1) % totDemo;

      // Debug opzionale su porta seriale
      Serial.print(F("Attivata procedura rotazione: "));
      Serial.println(elencoDemo[indiceAttuale]);
    }

    // Esegue la demo attualmente attiva
    eseguiProcedura(elencoDemo[indiceAttuale]);
  } 
  else {
    // Esecuzione fissa della singola procedura selezionata
    eseguiProcedura(procedure);
  }

}