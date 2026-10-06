#ifndef config_H
#define config_H

#include <avr/pgmspace.h>

#define FARLEFT  1

const char Info_00[] PROGMEM = "Demo ConsoleTFT 0.9 ";
const char Info_01[] PROGMEM = "(c) 2019-2020 ";
const char Info_02[] PROGMEM = "Max Scordamaglia ";
const char Info_03[] PROGMEM = "maxscorda@gmail.com ";
const char Info_04[] PROGMEM = " ... ";


const char* const menu_Info[] PROGMEM = {Info_00, Info_01, Info_02, Info_03, Info_04
                                        };

 /*
   Heart image below is defined directly in flash memory.
   This reduces SRAM consumption.
   The image is defined from bottom to top (bits), from left to
   right (bytes).
*/
                                       
const PROGMEM uint8_t heartImage[8] =
{
  0B00001110,
  0B00011111,
  0B00111111,
  0B01111110,
  0B01111110,
  0B00111101,
  0B00011001,
  0B00001110
};

const PROGMEM uint8_t heartImage8[ 8 * 8 ] =
{
  0x00, 0xE0, 0xE0, 0x00, 0x00, 0xE5, 0xE5, 0x00,
  0xE0, 0xC0, 0xE0, 0xE0, 0xE0, 0xEC, 0xEC, 0xE5,
  0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE5, 0xEC, 0xE5,
  0x80, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE5, 0xE0,
  0x00, 0x80, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0x00,
  0x00, 0x00, 0x80, 0xE0, 0xE0, 0xE0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x80, 0xE0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const PROGMEM uint8_t heartImage16[32] = {
  0B00000000, 0B00000000, // Riga 0
  0B00011110, 0B01111000, // Riga 1
  0B00111111, 0B11111100, // Riga 2
  0B01111111, 0B11111110, // Riga 3
  0B11111111, 0B11111111, // Riga 4
  0B11111111, 0B11111111, // Riga 5
  0B11111111, 0B11111111, // Riga 6
  0B01111111, 0B11111110, // Riga 7
  0B00111111, 0B11111100, // Riga 8
  0B00011111, 0B11111000, // Riga 9
  0B00001111, 0B11110000, // Riga 10
  0B00000111, 0B11100000, // Riga 11
  0B00000011, 0B11000000, // Riga 12
  0B00000001, 0B10000000, // Riga 13
  0B00000000, 0B00000000, // Riga 14
  0B00000000, 0B00000000  // Riga 15
};


#endif
