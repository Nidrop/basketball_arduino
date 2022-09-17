//только отображение полученных результатов
void state_check(){
  //включен
  if((data_send.system_state & STATE_ON) == STATE_ON)
  {
    //таймер периода
    if((data_send.system_state & STATE_TIMER_MODE_PERIOD) == STATE_TIMER_MODE_PERIOD)
    {
      //digitalWrite(LED_STRING_RED_PIN, HIGH);
      //digitalWrite(LED_STRING_YELLOW_PIN, HIGH);
      //Serial.println("p");
      show_Clock_LEDmatrix(data_send.period_or_timeout_timer, mRGB(0,255,0));
      matrix.show();
    } 
    //таймер таймаута
    else {
      //Serial.println("t");
      show_Clock_LEDmatrix(data_send.period_or_timeout_timer, mRGB(255,255,0));
      matrix.show();
    }

    //таймер атаки включен
    if((data_send.system_state & STATE_ATTACK_ON) == STATE_ATTACK_ON)
    {
      show_Clock_sm16188 (data_send.attack_timer);
      updateScreen_sm16188();
    }
    //таймер атаки выключен
    else
    {
      clearScreen_sm16188(true); 
      updateScreen_sm16188();
    }

    //одна секунда после окончания таймера периода
    if((data_send.system_state & STATE_ONE_SEC_AFTER_PERIOD) == STATE_ONE_SEC_AFTER_PERIOD)
    {
      digitalWrite(LED_STRING_RED_PIN, HIGH);
    }
    else
    {
      digitalWrite(LED_STRING_RED_PIN, LOW);
  
    }
    
    //одна секунда после окончания таймера атаки
    if((data_send.system_state & STATE_ONE_SEC_AFTER_ATTACK) == STATE_ONE_SEC_AFTER_ATTACK)
    {
      digitalWrite(LED_STRING_YELLOW_PIN, HIGH);
    }
    else
    {
      digitalWrite(LED_STRING_YELLOW_PIN, LOW);
    }
  }
  //выключен
  else {
    drawBitmap_32x32_sm16188(0,0);
    updateScreen_sm16188();
    
    fillString(russian, mRGB(255,255,255));
    //debug
//    matrix.clear();
//    matrix.show();
  }
}
