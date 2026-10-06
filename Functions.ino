void InitScreen() {
  // Rilevamento automatico dell'ID del controller TFT e avvio
  uint16_t identifier = tft.readID();
  if (identifier == 0x0101) identifier = 0x9341;  // Fallback comune
  tft.begin(identifier);
  tft.setRotation(1);      // Imposta l'orientamento dello schermo
  tft.fillScreen(0x0000);  // Schermo nero iniziale
}

void LayoutScreen() {
  // Linea orizzontale verde usando GFX
  tft.drawFastHLine(0, 68, tft.width(), tft.color565(0, 255, 0));
}



//================ Random ==============
int goodRandomseed() {
  unsigned long tempBits = 0;  // create a long of random bits to use as seed
  byte noOfBits = 31;          //preassegnato

  for (int i = 1; i <= 32; i++) {
#if (defined(__SAM3X8E__))
    //valido solo per il DUE con un generatore casuale interno
    pmc_enable_periph_clk(ID_TRNG);
    trng_enable(TRNG);
    tempBits = trng_read_output_data(TRNG);
#endif
#if (defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__))
    tempBits = (tempBits | analogRead(3) | (analogRead(0) & 1)) << 1;
    // tempBits = tempBits + ( analogRead( 2 ) | ( analogRead( 1 )  & 1 ) ) << 1;
#else
    tempBits = (tempBits | (analogRead(0) & analogRead(1) & 1)) << 1;
#endif
  }
  randomSeed(tempBits);
}

int RandNum(int rmin, int rmax) {
  int result;
  result = random(rmin, rmax + 1);
  //serprint(String(result));
  return result;
}


// -------------------------------------------------------------
// FUNZIONE SMISTAMENTO: contiene la logica di ciascuna demo
// -------------------------------------------------------------
void eseguiProcedura(int id) {
  switch (id) {
    case 4:
      SinDemo_1();
      SinDemo_2();

      break;

    case 5:
      Qix();
      LinkedPoints();
      break;

    case 6:
      SinDemo_1();
      LinkedPoints();
      break;

    case 7:
      LinkedPoints();
      Stars_Array();
      // ScrollText();
      break;

    case 8:
      MoireBars();
      break;

    case 9:
      Stars_Array();
      Qix();
      break;

    case 10:
      Qix();
      break;

    case 11:
      QixAI();
      break;

    case 15:
      QixAI();
      // MoireBars();
      Qix();
      break;

    case 20:
      Oldloop();
      break;
  }
}
