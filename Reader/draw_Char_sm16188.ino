void drawBitmap_32x32_sm16188(uint8_t x, uint8_t y) {
  clearScreen_sm16188(true);
  for (byte i = 0; i < 32; i++) {
    uint16_t thisByte;
    thisByte = bitmap[2 * i];
      for (int j = 0; j < 16; j++) {
        boolean thisBit;
        thisBit = thisByte & (1 << (15 - j));
        if (thisBit) writePixel_sm16188 (i + x, j + 16 + y, 1);
      }
    thisByte = bitmap[2 * i + 1];
     for (int j = 0; j < 16; j++) {
       boolean thisBit;
       thisBit = thisByte & (1 << (15 - j));
       if (thisBit) writePixel_sm16188 (i + x, j + y, 1);
     }
      
  }

}

void drawLetter_high32_sm16188(uint8_t letter, uint8_t x, uint8_t y) {
 
     for (byte i = 0; i < 16; i++) {
      uint16_t thisByte;
       thisByte = pgm_read_word(&(fontHEX_high32[letter - '0' ][2 * i]));
        for (int j = 0; j < 16; j++) {
        boolean thisBit;
        thisBit = thisByte & (1 << (15 - j));
        if (thisBit) writePixel_sm16188 (i + x, j + 16 + y, 1);
       }
      thisByte = pgm_read_word(&(fontHEX_high32[letter - '0' ][2 * i + 1]));
       for (int j = 0; j < 16; j++) {
        boolean thisBit;
        thisBit = thisByte & (1 << (15 - j));
        if (thisBit) writePixel_sm16188 (i + x, j + y, 1);
       }
      
    }
  } 

void drawLetter_high16_sm16188(uint8_t letter, uint8_t x, uint8_t y) {
 
     for (byte i = 0; i < 16; i++) {
      uint16_t thisByte;
      thisByte = pgm_read_word(&(fontHEX_high16[letter - '0'][i]));
      for (int j = 0; j < 16; j++) {
        boolean thisBit;
        thisBit = thisByte & (1 << (15 - j));
        if (thisBit) writePixel_sm16188 (i + x, j + 16 + y, 1);
      }
     }
 }


void show_Time_sm16188 (uint32_t milliseconds) {
 uint8_t char1 = 0, char2 = 0;
    char1 = (milliseconds-((milliseconds/60000)*60000))/10000;
    char2 = (milliseconds % 10000)/1000;
   drawLetter_high32_sm16188(char1 + '0', 0, 0);
   drawLetter_high32_sm16188(char2 + '0', 16, 0);
}

void show_Time_sm16188_Small (uint32_t milliseconds) {
 uint8_t char1 = 0, char2 = 0;
    char1 = (milliseconds % 10000)/1000;
   char2 = (milliseconds % 1000)/100;
   drawLetter_high32_sm16188(char1 + '0', 4, 0);
   drawLetter_high16_sm16188(char2 + '0', 20, 0);

}

void show_Clock_sm16188 (uint32_t milliseconds) {
  clearScreen_sm16188(true);
  if (milliseconds >= 6000) show_Time_sm16188 (milliseconds);
     else show_Time_sm16188_Small (milliseconds);
  
}
