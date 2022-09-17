#define D1 8
#define D2 9

byte *ScreenRAM = (byte*)malloc(128);

void writePixel_sm16188(unsigned int bX, unsigned int bY, byte bPixel)
  {
    static uint16_t X, Y;
    if (bY > 15) { X = bX + 32; Y = bY - 16; }
    else { X = bX; Y = bY; }

    uint16_t Pointer;
    Pointer = (X * 2) + int(Y / 8);
    Y = Y - (int(Y / 8) * 8);
    if (bPixel == true)
      bitSet(ScreenRAM[Pointer], Y); // zero bit is pixel off
    else
      bitClear(ScreenRAM[Pointer], Y); // one bit is pixel on
  }          

  void clearScreen_sm16188(byte bNormal)
  {
    if (bNormal) // clear all pixels
      memset(ScreenRAM, 0, 128);
    else // set all pixels
      memset(ScreenRAM, 255, 128);
  }

void updateScreen_sm16188()
  {
    noInterrupts();
    for (int i = 127; i >= 0; i -= 2)
    {
      transfer2(ScreenRAM[i], D2);
    }
      transferBrightness2(1, D2);
    for (int i = 126; i >= 0; i -= 2)
    {
      transfer2(ScreenRAM[i], D1);
    }
      transferBrightness2(1, D1);
      interrupts();
  }

void transfer2(byte val, uint8_t pin)
{
    uint32_t delay_Micro;
    switch (pin)
    {
      case D1:
        for (int i = 7; i >= 0; i--)
        {
          if (bitRead(val, i))
          {
            delay_Micro = micros();
            if (!bit_is_set(PORTD, 2)) // D1
			      {
			        bitSet(PORTD, 2); // D1
			      }
            while (micros() - delay_Micro < 1) {}
            if (!bit_is_clear(PORTD, 2)) // D1
			      {
			       	bitClear(PORTD, 2); // D1
			      }
          }
          else
          {
            delay_Micro = micros();
            if (!bit_is_set(PORTD, 2)) // D1
				    {
					    bitSet(PORTD, 2); // D1
            }
            if (!bit_is_clear(PORTD, 2)) // D1
	          {
	            bitClear(PORTD, 2); // D1
            }
            while (micros() - delay_Micro < 1) {}
           }
         }
        break;
      case D2:
        for (int i = 7; i >= 0; i--)
        {
          if (bitRead(val, i))
          {
            delay_Micro = micros();
            if (!bit_is_set(PORTD, 3)) // D2
				    {
					    bitSet(PORTD, 3); // D2
				    }
            while (micros() - delay_Micro < 1) {}
            if (!bit_is_clear(PORTD, 3)) // D2
				    {
				   	  bitClear(PORTD, 3); // D2
			      }
          }
          else
          {
            delay_Micro = micros();
            if (!bit_is_set(PORTD, 3)) // D2
				    {
					    bitSet(PORTD, 3); // D2
				    }
            if (!bit_is_clear(PORTD, 3)) // D2
				    {
				     	bitClear(PORTD, 3); // D2
			      }
            while (micros() - delay_Micro < 1) {}
           }
        }
        break;
    }
}

void transferBrightness2(byte val, uint8_t pin)
    {
    uint32_t delay_Micro;
        switch (pin)
        {
        case D1:
          for (int i = 3; i >= 0; i--)
          {
            if (bitRead(val, i))
            {
              delay_Micro = micros();
              if (!bit_is_set(PORTD, 2)) // D1
				        {
					        bitSet(PORTD, 2); // D1
				        }
                while (micros() - delay_Micro < 1) {}
              if (!bit_is_clear(PORTD, 2)) // D1
				       {
				         	bitClear(PORTD, 2); // D1
			         }
            }
            else
            {
              delay_Micro = micros();
              if (!bit_is_set(PORTD, 2)) // D1
				        {
					       bitSet(PORTD, 2); // D1
                }
              if (!bit_is_clear(PORTD, 2)) // D1
	              {
	               bitClear(PORTD, 2); // D1
                }
                while (micros() - delay_Micro < 1) {}
            }
          }
            break;
        case D2:
            for (int i = 3; i >= 0; i--)
            {
              if (bitRead(val, i))
              {
                delay_Micro = micros();
                if (!bit_is_set(PORTD, 3)) // D2
				          {
					          bitSet(PORTD, 3); // D2
				          }
                    while (micros() - delay_Micro < 1) {}
                if (!bit_is_clear(PORTD, 3)) // D2
				          {
				         	 bitClear(PORTD, 3); // D2
			            }
              }
              else
              {
                delay_Micro = micros();
                if (!bit_is_set(PORTD, 3)) // D2
				          {
					          bitSet(PORTD, 3); // D2
				          }
                if (!bit_is_clear(PORTD, 3)) // D2
				          {
				          	bitClear(PORTD, 3); // D2
			            }
                    while (micros() - delay_Micro < 1) {}
              }
            }
           break;
        }
    }
