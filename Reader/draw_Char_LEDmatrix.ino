#define TEXT_SPEED 40

void drawLetter_LEDmatrix(uint8_t letter, int16_t offset, uint32_t color) {
 
   int8_t start_pos = 0, finish_pos = LET_WIDTH;
   if (offset < -LET_WIDTH || offset > M_WIDTH) return;   // Рисует если смещение неходится в диапазоне -ширина буквы длинна матрицы
   if (offset < 0) start_pos = -offset;
   if (offset > (M_WIDTH - LET_WIDTH)) finish_pos = M_WIDTH - offset;
   for (byte i = start_pos; i < finish_pos; i++) {
     uint16_t thisByte;
     thisByte = getFont((int)letter, i);
     for (int j = 0; j < LET_HEIGHT; j++) {
        boolean thisBit;
        thisBit = thisByte & (1 << (LET_HEIGHT - 1 - j));
        if (thisBit) matrix.set (offset + i, TEXT_HEIGHT + j, color);
        else matrix.set (offset + i, TEXT_HEIGHT + j, 0x000000);
     } 
   }
}

uint16_t getFont(uint8_t font, uint8_t row) {
  font = font - '0' + 16;   // перевод код символа из таблицы ASCII в номер согласно нумерации массива
  if (font <= 90) {
     return pgm_read_word(&(fontHEX_high9[font][row]));
  } else if (font >= 112 && font <= 159) {    // и пизд*ц ждя русских
    return pgm_read_word(&(fontHEX_high9[font - 17][row]));;
  }  else if (font >= 96 && font <= 111) {
     return pgm_read_word(&(fontHEX_high9[font + 47][row]));
  }
}

void fillString(char text[], uint32_t color) {
  uint32_t tmr = 0;
  
  matrix.clear();
  int stringLeght = 0;
  int i = 0;

  while (text[i] != '\0') {
      stringLeght = stringLeght + LET_WIDTH - 1;
      for (byte s = 0; s < 9; s++) {  
       if (getFont(text[i], s) == 0 && text[i] != 32) stringLeght --; // Установливаем динамическую ширину шрифта
      }
       

     if ((byte)text[i] > 191) i++;    // работаем с русскими буквами!
     i++;
     stringLeght ++;
  }
  int k = M_WIDTH;

  //Serial.println(string_Offset);
  //Serial.println (stringLeght);
  //Serial.println (k);
  while (k > (0 - stringLeght)) {
    
    int char_Offset = 0;
    if (millis() - tmr >= TEXT_SPEED) { 
      tmr = millis();
      k--;
      
    int i = 0, j = 0;
    while (text[i] != '\0') {
       if ((byte)text[i] > 191) i++;    // работаем с русскими буквами!
      drawLetter_LEDmatrix(text[i], (k + j * LET_WIDTH + SPACE - char_Offset), color);
       for (byte s = 0; s < 9; s++) {  
          if (getFont(text[i], s) == 0 && text[i] != 32) char_Offset ++; // Установливаем динамическую ширину шрифта
       }
       char_Offset --;
      i++;
      j++;
    }
     matrix.show();
    }
  }
} 

void show_Time_LEDmatrix (uint32_t milliseconds, uint32_t color)  {
  uint8_t char1, char2, char3, char4;
  char1 = milliseconds / 600000;
  char2 = (milliseconds-((milliseconds/600000)*600000))/60000;
  char3 = (milliseconds-((milliseconds/60000)*60000))/10000;
  char4 = (milliseconds % 10000)/1000;
  matrix.clear();
     drawLetter_LEDmatrix(char1 + 48 ,0, color);
     drawLetter_LEDmatrix(char2 + 48 ,7, color);
     drawLetter_LEDmatrix(58 ,14, mRGB(255, 0, 0));
     drawLetter_LEDmatrix(char3 + 48 ,19, color);
     drawLetter_LEDmatrix(char4 + 48 ,26, color);
}

void show_Time_LEDmatrix_One_min (uint32_t milliseconds, uint32_t color)  {
  uint8_t char1, char2, char3;
  char1 = (milliseconds-((milliseconds/60000)*60000))/10000;
  char2 = (milliseconds % 10000)/1000;
  char3 = (milliseconds % 1000)/100;
  matrix.clear();
     drawLetter_LEDmatrix(char1 + 48 ,10, color);
     drawLetter_LEDmatrix(char2 + 48 ,17, color);
     drawLetter_LEDmatrix(46 ,24, mRGB(255, 0, 0));
     drawLetter_LEDmatrix(char3 + 48 ,27, color);     
}

void show_Clock_LEDmatrix (uint32_t milliseconds, uint32_t color) {
  clearScreen_sm16188(true);
  if (milliseconds >= 60000) show_Time_LEDmatrix (milliseconds, color);
     else show_Time_LEDmatrix_One_min (milliseconds, color);
  
}
