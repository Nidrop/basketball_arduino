void buttons_tick(uint16_t t){
   if(millis() - timer_buttons_old >= t){
    buttonAttack14.tick();
    buttonAttack24.tick();
    buttonAttackPause.tick();
    buttonAttackHide.tick();
    buttonTone1.tick();
    //buttonTone2.tick();
    buttonPeriodPause.tick();
    buttonTimeout.tick();
    
    buttons_handler();

    timer_buttons_old = millis();
   }
}

//void attack_tick(uint16_t t){
//  if(millis() - timer_attack_old >= t){
//
//    state_check(t);
//    
//    bus.sendData(READER_ADDRESS, data_send);  // на адрес 2
//    
//    timer_attack_old = millis();
//    //Serial.println(prev_timer_buttons);
//  } 
//}
