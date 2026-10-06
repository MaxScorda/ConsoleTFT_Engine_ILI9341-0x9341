void Oldloop() {
  delay(1000);
  
  // Esecuzione sequenziale delle demo senza il menu ssd1306
  bitmapDemo();
  spriteDemo();
  textDemo();
  canvasDemo();
  drawLinesDemo();
}

static void bitmapDemo() {
  int w=tft.width();
  int h=tft.height();
  tft.fillScreen(0x0000);
  tft.drawBitmap((w/2)-10, (h/2)-10, heartImage8, 8, 8, tft.color565(255, 255, 255));
  tft.drawBitmap((w/2)+10, (h/2)+10, heartImage, 8,8, tft.color565(255, 64, 64));
  delay(3000);
}

static void spriteDemo() {
  tft.fillScreen(0x0000);

  int sprite_x = 0;
  int sprite_y = 0;
  
  for (int i = 0; i < 250; i++) {
    delay(15);
    
    // Cancella la vecchia posizione disegnandoci sopra un rettangolo nero
    tft.fillRect(sprite_x, sprite_y, 8, 8, 0x0000);
    
    // Calcola le nuove coordinate
    sprite_x++;
    if (sprite_x >= tft.width()) {
      sprite_x = 0;
    }
    sprite_y++;
    if (sprite_y >= tft.height()) {
      sprite_y = 0;
    }
    
    // Disegna il nuovo sprite in rosso
    tft.drawBitmap(sprite_x, sprite_y, heartImage, 8, 8, tft.color565(255, 32, 32));
  }

//16*16
  sprite_x = 0;
  sprite_y = 0;
    for (int i = 0; i < 250; i++) {
    delay(15);
    
    // Cancella la vecchia posizione disegnandoci sopra un rettangolo nero
    tft.drawBitmap(sprite_x, sprite_y, heartImage16, 16, 16,0x0000);
    
    // Calcola le nuove coordinate
    sprite_x+=4;
    if (sprite_x >= tft.width()) {
      sprite_x = 0;
    }
    sprite_y+=4;
    if (sprite_y >= tft.height()) {
      sprite_y = 0;
    }
    
    // Disegna il nuovo sprite in rosso
    tft.drawBitmap(sprite_x, sprite_y,heartImage16, 16, 16, tft.color565(255, 32, 32));
  }
}

static void textDemo() {
  tft.fillScreen(0x0000);
  tft.setTextSize(2);
  
  tft.setTextColor(tft.color565(255, 255, 0));
  tft.setCursor(0, 16);
  tft.print("Normal text");
  
  tft.setTextColor(tft.color565(0, 255, 0));
  tft.setCursor(0, 48);
  tft.print("bold text?");
  
  tft.setTextColor(tft.color565(0, 255, 255));
  tft.setCursor(0, 64);
  tft.print("Italic text?");
  
  // Testo nero su sfondo bianco (Modalità invertita)
  tft.setTextColor(0x0000, tft.color565(255, 255, 255));
  tft.setCursor(0, 96);
  tft.print("Inverted bold?");
  
  delay(3000);
}

static void canvasDemo() {
  tft.fillScreen(0x0000);
  
  // Centriamo il finto "canvas" nello schermo del TFT
  int cx = (tft.width() - 64) / 2;
  int cy = 1;
  
  tft.fillRect(cx + 10, cy + 3, 80, 5, tft.color565(0, 255, 0));
  delay(500);
  
  tft.fillRect(cx + 50, cy + 1, 60, 15, tft.color565(0, 255, 0));
  delay(1500);
  
  // Testo nero su sfondo verde
  tft.setTextColor(0x0000, tft.color565(0, 255, 0)); 
  tft.setTextSize(2);
  tft.setCursor(cx + 30, cy + 1);
  tft.print(" DEMO ");
  
  delay(3000);
}

static void drawLinesDemo() {
  tft.fillScreen(0x0000);
  for (uint16_t y = 0; y < tft.height(); y += 8) {
    tft.drawLine(0, 0, tft.width() - 1, y, tft.color565(255, 0, 0));
  }
  for (int16_t x = tft.width() - 1; x > 7; x -= 8) {
    tft.drawLine(0, 0, x, tft.height() - 1, tft.color565(0, 255, 0));
  }
  delay(3000);
}
