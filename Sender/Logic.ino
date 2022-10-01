bool noToneIsSet = true;

void buttons_handler(){
  if (buttonAttack14.isClick()){
    data_send.attack_timer = 14100; //+100 т.к. сразу после нажатия идет отнятие
    data_send.system_state = (data_send.system_state | STATE_ATTACK_ON) & (~STATE_ATTACK_PAUSE);
    //Serial.println(data_send.attack_timer);
  }
  if (buttonAttack24.isClick()){
    data_send.attack_timer = 24100; //+100 т.к. сразу после нажатия идет отнятие
    data_send.system_state = (data_send.system_state | STATE_ATTACK_ON) & (~STATE_ATTACK_PAUSE);
    //Serial.println(data_send.attack_timer);
  }
  if (buttonAttack14.isHold()){
    data_send.attack_timer = 14000;
    data_send.system_state = data_send.system_state | STATE_ATTACK_ON | STATE_ATTACK_PAUSE;
    //Serial.println(data_send.attack_timer);
  }
  if (buttonAttack24.isHold()){
    data_send.attack_timer = 24000;
    data_send.system_state = data_send.system_state | STATE_ATTACK_ON | STATE_ATTACK_PAUSE;
    //Serial.println(data_send.attack_timer);
  }
  if (buttonAttackPause.isClick()){
    if((data_send.system_state & STATE_ATTACK_ON) == STATE_ATTACK_ON){
      data_send.system_state = data_send.system_state ^ STATE_ATTACK_PAUSE;
    }
  }
  if (buttonAttackHide.isClick()){
    data_send.attack_timer = 0;
    data_send.system_state = data_send.system_state ^ STATE_ATTACK_ON;
    data_send.system_state = data_send.system_state | STATE_ATTACK_PAUSE;
  }
  if (buttonTone1.isClick()){
    tone(TONE_PIN, 250, 500);
  }
  if (buttonTone1.isHold()){
    tone(TONE_PIN, 250, 1000);
  }
//  if (buttonTone2.isClick()){
//    tone(TONE_PIN, 250, 500);
//  }
//  if (buttonTone2.isHold()){
//    tone(TONE_PIN, 250, 1000);
//  }
  if (buttonPeriodPause.isClick()){
    //отправка запроса на паузу компьютеру
    Serial.write(data_send.system_state ^ STATE_TIMER_PAUSE);
  }
  if (buttonTimeout.isClick()){
    //отправка запроса на таймаут компьютеру
    Serial.write(data_send.system_state ^ STATE_TIMER_MODE_PERIOD);
  }
} 

void state_check(uint16_t t){
  //если атака не на паузе и атака больше нуля
  if( (!((data_send.system_state & STATE_ATTACK_PAUSE) == STATE_ATTACK_PAUSE)) && (data_send.attack_timer > 0)){
    data_send.attack_timer -= t;
    
    if(data_send.attack_timer == 0){
      data_send.system_state = data_send.system_state | STATE_ATTACK_PAUSE;
      data_send.system_state = data_send.system_state | STATE_ONE_SEC_AFTER_ATTACK;
      firstSecondAfterAttackTimer = millis();
      tone(TONE_PIN, 250);
    }
  }

  if((data_send.system_state & STATE_ONE_SEC_AFTER_ATTACK) == STATE_ONE_SEC_AFTER_ATTACK){
    if(millis() - firstSecondAfterAttackTimer >= 1000){
      data_send.system_state = data_send.system_state & (~STATE_ONE_SEC_AFTER_ATTACK);
      noTone(TONE_PIN);
    }
  }

  bool oneSecAfterPeriod = (data_send.system_state & STATE_ONE_SEC_AFTER_PERIOD) == STATE_ONE_SEC_AFTER_PERIOD;
  bool oneSecAfter5SecLeftPeriod = (data_send.system_state & STATE_ONE_SEC_AFTER_5_SEC_LEFT_PERIOD) == STATE_ONE_SEC_AFTER_5_SEC_LEFT_PERIOD;
  if(oneSecAfterPeriod || oneSecAfter5SecLeftPeriod)
  {
    if(oneSecAfterPeriod){
      tone(TONE_PIN, 250);
    }
    if(oneSecAfter5SecLeftPeriod){
      tone(TONE_PIN, 300);
    }
    noToneIsSet = false;
  }
  else
  {
    if(!noToneIsSet){
      noTone(TONE_PIN);
      noToneIsSet = true;
    }
  }
}
