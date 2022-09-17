void stale_tick (uint16_t t) {
  if(millis() - stale_timer >= t){

    data_send.system_state = 0;
    state_check();
    
    stale_timer = millis();
  } 
}
