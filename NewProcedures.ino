void Stars_Array() {
  const byte ATTIVALAMPEGGIO = 1;
#define NSTELLE 48

  struct dstars {
    byte id;
    int xpos;
    int ypos;
    byte grandezza;
    byte velocita;
    char contavelocita;
    byte passi;
    byte colore[3];
    char segno;
  };
  typedef struct dstars SDStars;
  static SDStars Stars[NSTELLE];

  int ww = tft.width();
  int hh = tft.height();
  byte timedelay = 25;
  byte stepcolore = 5;
  static unsigned long timevis = 0;
  boolean trovato = false;

  if (timevis + timedelay < millis()) {
    uint16_t blackColor = tft.color565(0, 0, 0);

    for (int i = 0; i < NSTELLE; i++) {
      if (Stars[i].id == 0) {
        if (trovato == false) {
          Stars[i].id = i + 1;
          // Lasciamo un margine sicuro basato sulla grandezza massima (3 pixel)
          Stars[i].xpos = RandNum(3, ww - 4);
          Stars[i].ypos = 0;
          if (ATTIVALAMPEGGIO == 1) {
            Stars[i].velocita = RandNum(1, 5);
          } else {
            Stars[i].velocita = RandNum(1, 1);
          }
          Stars[i].contavelocita = Stars[i].velocita;
          Stars[i].passi = RandNum(1, 5);
          Stars[i].grandezza = RandNum(0, 3);
          Stars[i].colore[0] = RandNum(32, 255);
          Stars[i].colore[1] = RandNum(32, 255);
          Stars[i].colore[2] = RandNum(32, 255);
          trovato = true;
        }
      } else {
        if (Stars[i].contavelocita <= 1) {
          // CANCELLAZIONE SICURA (evita coordinate negative)
          if (Stars[i].grandezza > 0) {
            int vY = Stars[i].ypos - Stars[i].grandezza;
            int vH = (Stars[i].grandezza * 2) + 1;
            if (vY < 0) {
              vH += vY;
              vY = 0;
            }
            if (vH > 0 && vY < hh) {
              tft.drawFastVLine(Stars[i].xpos, vY, vH, blackColor);
            }

            int hX = Stars[i].xpos - Stars[i].grandezza;
            int hW = (Stars[i].grandezza * 2) + 1;
            if (hX < 0) {
              hW += hX;
              hX = 0;
            }
            if (hW > 0 && hX < ww && Stars[i].ypos >= 0 && Stars[i].ypos < hh) {
              tft.drawFastHLine(hX, Stars[i].ypos, hW, blackColor);
            }
          } else {
            if (Stars[i].xpos >= 0 && Stars[i].xpos < ww && Stars[i].ypos >= 0 && Stars[i].ypos < hh) {
              tft.drawPixel(Stars[i].xpos, Stars[i].ypos, blackColor);
            }
          }

          Stars[i].contavelocita = Stars[i].velocita;
          Stars[i].ypos += Stars[i].passi;
          if (Stars[i].ypos > hh - 1) {
            Stars[i].id = 0;
          }
        } else {
          Stars[i].contavelocita--;
        }
      }

      if (Stars[i].id > 0) {
        if (Stars[i].contavelocita == Stars[i].velocita) {
          if ((Stars[i].colore[0] == 255) || (Stars[i].colore[1] == 255) || (Stars[i].colore[2] == 255)) {
            Stars[i].segno = -stepcolore;
          }
          if ((Stars[i].colore[0] == 32) || (Stars[i].colore[1] == 32) || (Stars[i].colore[2] == 32)) {
            Stars[i].segno = stepcolore;
          }
          for (int g = 0; g < 3; g++) {
            Stars[i].colore[g] = constrain((long)(Stars[i].colore[g] + stepcolore), 32, 255);
          }

          uint16_t starColor = tft.color565(Stars[i].colore[0], Stars[i].colore[1], Stars[i].colore[2]);

          // DISEGNO SICURO
          if (Stars[i].grandezza > 0) {
            int vY = Stars[i].ypos - Stars[i].grandezza;
            int vH = (Stars[i].grandezza * 2) + 1;
            if (vY < 0) {
              vH += vY;
              vY = 0;
            }
            if (vH > 0 && vY < hh) {
              tft.drawFastVLine(Stars[i].xpos, vY, vH, starColor);
            }

            int hX = Stars[i].xpos - Stars[i].grandezza;
            int hW = (Stars[i].grandezza * 2) + 1;
            if (hX < 0) {
              hW += hX;
              hX = 0;
            }
            if (hW > 0 && hX < ww && Stars[i].ypos >= 0 && Stars[i].ypos < hh) {
              tft.drawFastHLine(hX, Stars[i].ypos, hW, starColor);
            }
          }
          if (Stars[i].xpos >= 0 && Stars[i].xpos < ww && Stars[i].ypos >= 0 && Stars[i].ypos < hh) {
            tft.drawPixel(Stars[i].xpos, Stars[i].ypos, tft.color565(255, 255, 255));
          }
        }
      }
    }
    timevis = millis();
  }
}

void SinDemo_1() {
  constexpr uint8_t  STEP       = 2;   // Risoluzione orizzontale (2 px per segmento: fluidissimo e leggero)
  constexpr uint16_t MAX_POINTS = 165; // Supporta display fino a 320 px di larghezza (320 / 2 + 5)
  constexpr uint16_t DELAY_MS   = 20;  // Frame rate (~50 FPS)

  // Buffer per memorizzare la linea del frame precedente (solo 330 byte di RAM!)
  static int16_t prevY[MAX_POINTS];

  // Parametri fisici dell'onda
  static float phase1 = 0.0f; // Fase onda primaria
  static float phase2 = 0.0f; // Fase armonica secondaria

  static float amp1 = 20.0f;       // Ampiezza corrente onda principale
  static float targetAmp1 = 30.0f; // Bersaglio casuale da raggiungere
  static float amp2 = 8.0f;        // Ampiezza corrente increspatura
  static float targetAmp2 = 12.0f; // Bersaglio casuale increspatura

  static uint32_t lastTime = 0;
  static int16_t  ww = 0, hh = 0, centerY = 0;
  static uint8_t  colorCycle = 0;
  static bool     inizializzato = false;

  // Inizializzazione al primo avvio
  if (!inizializzato) {
    ww = tft.width();
    hh = tft.height();
    centerY = hh / 2; // Centro dello schermo

    for (uint16_t i = 0; i < MAX_POINTS; i++) {
      prevY[i] = centerY;
    }
    inizializzato = true;
  }

  // Controllo del framerate
  if (millis() - lastTime < DELAY_MS) return;
  lastTime = millis();

  // 1. DINAMICA CASUALE MORBIDA: L'onda si gonfia e si sgonfia gradualmente
  amp1 += (targetAmp1 - amp1) * 0.04f; // Interpolazione verso l'obiettivo primario
  amp2 += (targetAmp2 - amp2) * 0.06f; // Interpolazione dell'increspatura

  // Se è vicina all'obiettivo, estrae una nuova ampiezza casuale
  if (abs(amp1 - targetAmp1) < 1.0f) {
    targetAmp1 = RandNum(8, (hh / 2) - 15); // Da quasi piatta a quasi bordo schermo
  }
  if (abs(amp2 - targetAmp2) < 1.0f) {
    targetAmp2 = RandNum(3, 16);            // Ampiezza dell'increspatura secondaria
  }

  // Velocità di scorrimento delle onde
  phase1 += 0.06f; // Movimento dell'onda principale
  phase2 -= 0.11f; // Movimento in senso contrario dell'increspatura (crea turbolenza liquida)

  // 2. CICLO COLORI NEON DINAMICO (sfuma tra Ciano, Magenta, Giallo, Blu)
  colorCycle += 2;
  uint16_t waveColor = tft.color565(
    127 + (int16_t)(127 * sin(colorCycle * 0.025f)),
    127 + (int16_t)(127 * sin(colorCycle * 0.025f + 2.09f)),
    127 + (int16_t)(127 * sin(colorCycle * 0.025f + 4.18f))
  );

  #if defined(SPI_HAS_TRANSACTION) || defined(_ADAFRUIT_GFX_H_) || defined(_TFT_eSPI_H_)
    tft.startWrite();
  #endif

  int16_t prevNewY = 0;
  uint16_t ptIdx   = 0;

  // 3. DISEGNO E CANCELLAZIONE LINEA PER LINEA
  for (int16_t x = 0; x < ww; x += STEP, ptIdx++) {
    // Calcolo della forma d'onda composita: onda principale + armonica veloce
    float rad1 = (x * 0.028f) + phase1;
    float rad2 = (x * 0.075f) + phase2;
    int16_t currentY = centerY + (int16_t)(sin(rad1) * amp1 + sin(rad2) * amp2);

    if (x > 0) {
      int16_t prevX = x - STEP;

      // CANCELLA il vecchio segmento (0x0000 = nero)
      tft.drawLine(prevX, prevY[ptIdx - 1], x, prevY[ptIdx], 0x0000);

      // DISEGNA il nuovo segmento con il colore neon attivo
      tft.drawLine(prevX, prevNewY, x, currentY, waveColor);

      // Aggiorna lo storico del punto precedente per il prossimo ciclo
      prevY[ptIdx - 1] = prevNewY;
    }

    prevNewY = currentY;
  }

  // Salva l'ultimo punto del frame
  if (ptIdx > 0) {
    prevY[ptIdx - 1] = prevNewY;
  }

  #if defined(SPI_HAS_TRANSACTION) || defined(_ADAFRUIT_GFX_H_) || defined(_TFT_eSPI_H_)
    tft.endWrite();
  #endif
}


// Helper ultra-veloce per colori cangianti (ruota cromatica RGB pura a 8 bit)
inline uint16_t colorWheel(uint8_t pos) {
  if (pos < 85) {
    return tft.color565(255 - pos * 3, pos * 3, 0);         // Rosso -> Verde
  } else if (pos < 170) {
    pos -= 85;
    return tft.color565(0, 255 - pos * 3, pos * 3);         // Verde -> Blu
  } else {
    pos -= 170;
    return tft.color565(pos * 3, 0, 255 - pos * 3);         // Blu -> Rosso
  }
}

void SinDemo_2() {
  constexpr uint8_t PASSO     = 5;  // Avanzamento a 1 px: movimento fluido

  static int16_t  y_old[320];
  static uint8_t  ccont = 0;
  static uint8_t  coloreFase = 0;
  static float    ampFase = 0.0f; // Respirazione dell'onda
  static int16_t  ww = 0, hh = 0, centerY = 0;
  static bool     inizializzato = false;

  if (!inizializzato) {
    ww = tft.width();
    hh = tft.height();
    centerY = hh / 2; // Perfettamente al centro

    tft.fillScreen(0x0000);
    for (int16_t i = 0; i < 320; i++) y_old[i] = centerY;
    inizializzato = true;
  }

  // 1. RESPIRAZIONE CONTINUA DELL'AMPIEZZA
  ampFase += 0.035f; 
  if (ampFase >= 6.2831853f) ampFase -= 6.2831853f;

  const int16_t maxAmp = centerY - 5; // Quasi al limite dello schermo
  // Varia con continuità: diventa completamente piatta (0) e poi si riallarga al massimo
  const int16_t hiy = (int16_t)(maxAmp * sin(ampFase));

  // 2. CALCOLO COLORE CANGIANTE INTERNO (nessuna funzione esterna richiesta)
  coloreFase += 2;
  uint16_t waveColor;
  if (coloreFase < 85) {
    waveColor = tft.color565(255 - coloreFase * 3, coloreFase * 3, 0); // Rosso -> Verde
  } else if (coloreFase < 170) {
    uint8_t pos = coloreFase - 85;
    waveColor = tft.color565(0, 255 - pos * 3, pos * 3);               // Verde -> Blu
  } else {
    uint8_t pos = coloreFase - 170;
    waveColor = tft.color565(pos * 3, 0, 255 - pos * 3);               // Blu -> Rosso
  }

  // 3. Tabella locale del periodo dell'onda (~79 calcoli per frame)
  const uint8_t lenx = (ww - 1) >> 2;
  int16_t sinTable[85];
  const float stepRad = 6.2831853f / lenx;
  for (uint8_t i = 0; i < lenx; i++) {
    sinTable[i] = (int16_t)(sin(i * stepRad) * hiy);
  }

  uint8_t tableIdx = (ccont + ((lenx * 3) >> 2)) % lenx;

  #if defined(SPI_HAS_TRANSACTION) || defined(_ADAFRUIT_GFX_H_) || defined(_TFT_eSPI_H_)
    tft.startWrite();
  #endif

  // 4. Disegno e cancellazione differenziale
  for (int16_t xx = 0; xx < ww; xx++) {
    int16_t y_new = centerY + sinTable[tableIdx];

    if (++tableIdx >= lenx) tableIdx = 0;

    if (y_old[xx] != y_new) {
      tft.drawPixel(xx, y_old[xx], 0x0000); // Cancella vecchio
      tft.drawPixel(xx, y_new, waveColor);  // Disegna nuovo
      y_old[xx] = y_new;
    } else {
      tft.drawPixel(xx, y_new, waveColor);  // Aggiorna colore sui punti fermi
    }
  }

  #if defined(SPI_HAS_TRANSACTION) || defined(_ADAFRUIT_GFX_H_) || defined(_TFT_eSPI_H_)
    tft.endWrite();
  #endif

  ccont += PASSO;
}

void LinkedPoints() {
  constexpr uint8_t NPUNTI = 6;

  // Parametri di rimbalzo: pixel minimi e massimi di spostamento
  constexpr int8_t StepMin = 2;
  constexpr int8_t StepMax = 10;

  // Struttura punto: contiene solo le coordinate attuali sullo schermo
  struct SDPunti {
    int16_t x;
    int16_t y;
  };

  static SDPunti punti[NPUNTI];

  // Array che contiene la velocità/pixel di rimbalzo per ciascun punto:
  // rimbalzo[punto][0 = asse X, 1 = asse Y]
  // Il segno (+ o -) determina la direzione, il valore assoluto determina la velocità
  static int8_t rimbalzo[NPUNTI][2];

  static uint8_t colore[3][2];  // [R,G,B][0:corrente, 1:target]
  static uint8_t idx = 0;
  static int16_t ww = 0;
  static int16_t hh = 0;
  static bool inizializzato = false;

  // Inizializzazione eseguita solo al primo avvio
  if (!inizializzato) {
    ww = tft.width();
    hh = tft.height();

    for (uint8_t i = 0; i < NPUNTI; i++) {
      punti[i].x = RandNum(0, ww - 1);
      punti[i].y = RandNum(0, hh - 1);

      // Calcola il rimbalzo iniziale casuale (con direzione casuale +/-)
      rimbalzo[i][0] = RandNum(StepMin, StepMax) * (RandNum(0, 1) ? 1 : -1);
      rimbalzo[i][1] = RandNum(StepMin, StepMax) * (RandNum(0, 1) ? 1 : -1);
    }

    for (uint8_t col = 0; col < 3; col++) {
      colore[col][0] = RandNum(128, 255);
      colore[col][1] = 128;
    }
    inizializzato = true;
  }

// Opzionale: velocizza il bus SPI su display TFT
#if defined(SPI_HAS_TRANSACTION) || defined(_ADAFRUIT_GFX_H_) || defined(_TFT_eSPI_H_)
  tft.startWrite();
#endif

  // 1. CANCELLA le vecchie linee del punto corrente (nero = 0x0000)
  for (uint8_t i = 1; i < NPUNTI; i++) {
    uint8_t idxnow = idx + i;
    if (idxnow >= NPUNTI) idxnow -= NPUNTI;
    tft.drawLine(punti[idx].x, punti[idx].y, punti[idxnow].x, punti[idxnow].y, 0x0000);
  }

  // 2. AGGIORNA POSIZIONE E CALCOLA NUOVO RIMBALZO AI BORDI

  // --- Asse X ---
  punti[idx].x += rimbalzo[idx][0];
  if (punti[idx].x <= 0) {
    punti[idx].x = 0;
    // Rimbalzo a sinistra: calcola nuovo step casuale positivo (va verso destra)
    rimbalzo[idx][0] = RandNum(StepMin, StepMax);
  } else if (punti[idx].x >= ww - 1) {
    punti[idx].x = ww - 1;
    // Rimbalzo a destra: calcola nuovo step casuale negativo (va verso sinistra)
    rimbalzo[idx][0] = -RandNum(StepMin, StepMax);
  }

  // --- Asse Y ---
  punti[idx].y += rimbalzo[idx][1];
  if (punti[idx].y <= 0) {
    punti[idx].y = 0;
    // Rimbalzo in alto: calcola nuovo step casuale positivo (va verso il basso)
    rimbalzo[idx][1] = RandNum(StepMin, StepMax);
  } else if (punti[idx].y >= hh - 1) {
    punti[idx].y = hh - 1;
    // Rimbalzo in basso: calcola nuovo step casuale negativo (va verso l'alto)
    rimbalzo[idx][1] = -RandNum(StepMin, StepMax);
  }

  // 3. CALCOLA IL COLORE UNA VOLTA SOLA
  uint16_t lineColor = tft.color565(colore[0][0], colore[1][0], colore[2][0]);

  // 4. DISEGNA le nuove linee collegate al punto aggiornato
  for (uint8_t i = 1; i < NPUNTI; i++) {
    uint8_t idxnow = idx + i;
    if (idxnow >= NPUNTI) idxnow -= NPUNTI;
    tft.drawLine(punti[idx].x, punti[idx].y, punti[idxnow].x, punti[idxnow].y, lineColor);
  }

#if defined(SPI_HAS_TRANSACTION) || defined(_ADAFRUIT_GFX_H_) || defined(_TFT_eSPI_H_)
  tft.endWrite();
#endif

  // 5. SFUMATURA DEL COLORE ogni 2 cicli
  if ((idx & 1) == 0) {
    for (uint8_t col = 0; col < 3; col++) {
      if (colore[col][0] == colore[col][1]) {
        colore[col][1] = RandNum(128, 255);
      }
      if (colore[col][0] > colore[col][1]) colore[col][0]--;
      else if (colore[col][0] < colore[col][1]) colore[col][0]++;
    }
  }

  // Avanza al prossimo punto
  if (++idx >= NPUNTI) idx = 0;
}


void Qix() {
  //Versione ottimizzata da PC
#define NQIX 2
#define SPIRE 12

  int ww = tft.width();
  int hh = tft.height();
  int minpasso = 2;
  int maxpasso = 15;

  static int x[NQIX][SPIRE][2];
  static int y[NQIX][SPIRE][2];
  static int destinazione[NQIX][2][2];
  static int passo[NQIX][SPIRE][2];
  static byte colore[NQIX][3][2];  // [q][R,G,B][0: corrente, 1: target]
  static int spireidx = 0;
  static bool primoGiro = false;

  // Inizializzazione al primo avvio
  if (!primoGiro) {
    for (int q = 0; q < NQIX; q++) {
      for (int c = 0; c < 3; c++) {
        colore[q][c][0] = RandNum(40, 255);
        colore[q][c][1] = RandNum(40, 255);
      }

      int startX0 = RandNum(20, ww - 21);
      int startY0 = RandNum(20, hh - 21);
      int startX1 = RandNum(20, ww - 21);
      int startY1 = RandNum(20, hh - 21);

      for (int s = 0; s < SPIRE; s++) {
        x[q][s][0] = startX0;
        x[q][s][1] = startX1;
        y[q][s][0] = startY0;
        y[q][s][1] = startY1;
        passo[q][s][0] = RandNum(minpasso, maxpasso);
        passo[q][s][1] = RandNum(minpasso, maxpasso);
      }

      for (int i = 0; i < 2; i++) {
        destinazione[q][0][i] = RandNum(0, ww - 1);
        destinazione[q][1][i] = RandNum(0, hh - 1);
      }
    }
    primoGiro = true;
  }

  // L'indice della testa precedente da cui calcolare il nuovo passo
  int idxprev = (spireidx + SPIRE - 1) % SPIRE;

  for (int q = 0; q < NQIX; q++) {
    // 1. CANCELLA LA CODA: Lo slot spireidx contiene attualmente la spira più vecchia
    tft.drawLine(x[q][spireidx][0], y[q][spireidx][0], x[q][spireidx][1], y[q][spireidx][1], 0x0000);

    // 2. CALCOLA LA NUOVA TESTA: Parte dalla posizione precedente (idxprev) e avanza verso la destinazione
    for (int i = 0; i < 2; i++) {
      int newX = x[q][idxprev][i];
      int newY = y[q][idxprev][i];

      // Aggiornamento X
      if (abs(newX - destinazione[q][0][i]) <= passo[q][spireidx][0]) {
        destinazione[q][0][i] = RandNum(0, ww - 1);
        passo[q][spireidx][0] = RandNum(minpasso, maxpasso);
      }
      if (newX > destinazione[q][0][i]) newX -= passo[q][spireidx][0];
      else if (newX < destinazione[q][0][i]) newX += passo[q][spireidx][0];

      // Aggiornamento Y
      if (abs(newY - destinazione[q][1][i]) <= passo[q][spireidx][1]) {
        destinazione[q][1][i] = RandNum(0, hh - 1);
        passo[q][spireidx][1] = RandNum(minpasso, maxpasso);
      }
      if (newY > destinazione[q][1][i]) newY -= passo[q][spireidx][1];
      else if (newY < destinazione[q][1][i]) newY += passo[q][spireidx][1];

      // Salviamo le nuove coordinate nello slot spireidx (diventa la nuova testa)
      x[q][spireidx][i] = constrain(newX, 0, ww - 1);
      y[q][spireidx][i] = constrain(newY, 0, hh - 1);
    }

    // 3. GESTIONE COLORE
    for (int c = 0; c < 3; c++) {
      if (colore[q][c][0] == colore[q][c][1]) {
        colore[q][c][1] = RandNum(40, 255);
      }
      if (colore[q][c][0] > colore[q][c][1]) colore[q][c][0]--;
      else if (colore[q][c][0] < colore[q][c][1]) colore[q][c][0]++;
    }
    uint16_t currentColor = tft.color565(colore[q][0][0], colore[q][1][0], colore[q][2][0]);

    // 4. DISEGNA LA NUOVA TESTA
    tft.drawLine(x[q][spireidx][0], y[q][spireidx][0], x[q][spireidx][1], y[q][spireidx][1], currentColor);
  }

  // Avanza il puntatore del buffer circolare per il prossimo frame
  spireidx = (spireidx + 1) % SPIRE;
}

void QixAI() {
  //versione fatta da AI
#define NQIX 2
#define QIX_TRAIL 12

  int ww = tft.width();
  int hh = tft.height();

  static int x[NQIX][QIX_TRAIL][2];
  static int y[NQIX][QIX_TRAIL][2];
  static int targetX[NQIX][2];
  static int targetY[NQIX][2];
  static int stepX[NQIX][2];
  static int stepY[NQIX][2];
  static uint16_t colore[NQIX];
  static int trailIdx = 0;
  static bool primoGiro = false;

  // Palette con colori ad altissima luminosità e visibilità
  static const uint16_t paletteQixAI[6] = {
    0xF800,  // Rosso vivido
    0x07E0,  // Verde brillante
    0x07FF,  // Ciano brillante
    0xF81F,  // Magenta brillante
    0xFFE0,  // Giallo brillante
    0xFDA0   // Arancione brillante
  };

  if (!primoGiro) {
    for (int q = 0; q < NQIX; q++) {
      int cx = RandNum(ww / 6, (ww * 5) / 6);
      int cy = RandNum(hh / 6, (hh * 5) / 6);
      colore[q] = paletteQixAI[RandNum(0, 5)];

      for (int i = 0; i < 2; i++) {
        targetX[q][i] = RandNum(0, ww - 1);
        targetY[q][i] = RandNum(0, hh - 1);
        stepX[q][i] = RandNum(3, 10);
        stepY[q][i] = RandNum(3, 10);
      }

      for (int s = 0; s < QIX_TRAIL; s++) {
        x[q][s][0] = cx + RandNum(-12, 12);
        y[q][s][0] = cy + RandNum(-12, 12);
        x[q][s][1] = cx + RandNum(-12, 12);
        y[q][s][1] = cy + RandNum(-12, 12);
      }
    }
    primoGiro = true;
  }

  // Indice della testa precedente da cui calcolare il nuovo movimento
  int prevIdx = (trailIdx + QIX_TRAIL - 1) % QIX_TRAIL;

  for (int q = 0; q < NQIX; q++) {
    // 1. CANCELLA LA CODA: Lo slot trailIdx contiene la linea più vecchia della scia
    tft.drawLine(x[q][trailIdx][0], y[q][trailIdx][0], x[q][trailIdx][1], y[q][trailIdx][1], 0x0000);

    // 2. CALCOLA LA NUOVA TESTA: Parte dalla posizione precedente (prevIdx)
    for (int i = 0; i < 2; i++) {
      int newX = x[q][prevIdx][i];
      int newY = y[q][prevIdx][i];

      // Controllo e aggiornamento Target X
      if (abs(newX - targetX[q][i]) <= stepX[q][i]) {
        targetX[q][i] = RandNum(0, ww - 1);
        stepX[q][i] = RandNum(3, 10);
      }
      if (newX > targetX[q][i]) newX -= stepX[q][i];
      else if (newX < targetX[q][i]) newX += stepX[q][i];

      // Controllo e aggiornamento Target Y
      if (abs(newY - targetY[q][i]) <= stepY[q][i]) {
        targetY[q][i] = RandNum(0, hh - 1);
        stepY[q][i] = RandNum(3, 10);
      }
      if (newY > targetY[q][i]) newY -= stepY[q][i];
      else if (newY < targetY[q][i]) newY += stepY[q][i];

      // Salva le nuove coordinate nello slot corrente (diventa la nuova testa)
      x[q][trailIdx][i] = constrain(newX, 0, ww - 1);
      y[q][trailIdx][i] = constrain(newY, 0, hh - 1);
    }

    // 3. DISEGNA LA NUOVA TESTA con colore brillante
    tft.drawLine(x[q][trailIdx][0], y[q][trailIdx][0], x[q][trailIdx][1], y[q][trailIdx][1], colore[q]);

    // Cambio di direzione casuale o cambio colore occasionale
    if (RandNum(0, 100) < 5) {
      targetX[q][0] = RandNum(0, ww - 1);
      targetY[q][0] = RandNum(0, hh - 1);
      targetX[q][1] = RandNum(0, ww - 1);
      targetY[q][1] = RandNum(0, hh - 1);
      // Cambia colore ogni tanto per dinamismo
      colore[q] = paletteQixAI[RandNum(0, 5)];
    }
  }

  // Avanza l'indice circolare
  trailIdx = (trailIdx + 1) % QIX_TRAIL;
}


void MoireBars() {
  //prima versione su Arduino
#define BARS 12
#define NMOIRE 2

  int ww = tft.width();
  int hh = tft.height();

  static unsigned long timevis = 0;
  static int x[NMOIRE][BARS][2];
  static int y[NMOIRE][BARS][2];
  static boolean primoGiro = false;
  byte timedelay = 25;

  static uint16_t moireColors[NMOIRE];

  if (!primoGiro) {
    moireColors[0] = tft.color565(255, 64, 64);
    moireColors[1] = tft.color565(64, 255, 64);
    moireColors[2] = tft.color565(64, 128, 255);

    for (int q = 0; q < NMOIRE; q++) {
      int startX0 = RandNum(0, ww / 2);
      int startY0 = RandNum(0, hh / 2);
      int startX1 = RandNum(ww / 2, ww);
      int startY1 = RandNum(hh / 2, hh);

      for (int i = 0; i < BARS; i++) {
        x[q][i][0] = startX0;
        y[q][i][0] = startY0;
        x[q][i][1] = startX1;
        y[q][i][1] = startY1;
      }
    }
    primoGiro = true;
  }

  if (timevis + timedelay < millis()) {
    for (int m = 0; m < NMOIRE; m++) {
      tft.drawLine(x[m][0][0], y[m][0][0], x[m][0][1], y[m][0][1], 0x0000);

      for (int g = 1; g < BARS; g++) {
        x[m][g - 1][0] = x[m][g][0];
        x[m][g - 1][1] = x[m][g][1];
        y[m][g - 1][0] = y[m][g][0];
        y[m][g - 1][1] = y[m][g][1];
      }

      int s = RandNum(1, 4);
      int c = RandNum(10, 30);

      if (s == 1) {
        x[m][BARS - 1][0] = constrain(x[m][BARS - 1][0] + c, 0, ww - 1);
        y[m][BARS - 1][1] = constrain(y[m][BARS - 1][1] - c, 0, hh - 1);
      } else if (s == 2) {
        x[m][BARS - 1][0] = constrain(x[m][BARS - 1][0] - c, 0, ww - 1);
        x[m][BARS - 1][1] = constrain(x[m][BARS - 1][1] + c, 0, ww - 1);
      } else if (s == 3) {
        y[m][BARS - 1][0] = constrain(y[m][BARS - 1][0] + c, 0, hh - 1);
        y[m][BARS - 1][1] = constrain(y[m][BARS - 1][1] - c, 0, hh - 1);
      } else if (s == 4) {
        y[m][BARS - 1][0] = constrain(y[m][BARS - 1][0] - c, 0, hh - 1);
        x[m][BARS - 1][1] = constrain(x[m][BARS - 1][1] - c, 0, ww - 1);
      }

      tft.drawLine(x[m][BARS - 1][0], y[m][BARS - 1][0], x[m][BARS - 1][1], y[m][BARS - 1][1], moireColors[m]);
    }

    timevis = millis();
  }
}