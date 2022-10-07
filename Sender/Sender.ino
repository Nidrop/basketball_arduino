//button pins
#define BUTTON_ATTACK_14_PIN    2
#define BUTTON_ATTACK_24_PIN    3
#define BUTTON_ATTACK_PAUSE_PIN 4
#define BUTTON_ATTACK_HIDE_PIN  5
#define BUTTON_TONE1_PIN        6
#define BUTTON_TONE2_PIN        7
#define BUTTON_PERIOD_PAUSE_PIN 7
#define BUTTON_TIMEOUT_PIN      8

//tone pin
#define TONE_PIN 11

//gbus
#define LINK_READER_PIN 12 //D12
#define LINK_SENDER_TO_READER_SPEED 4800
#define SENDER_ADDRESS 1
#define READER_ADDRESS 2

//states
#define STATE_ON                    0b00000001
#define STATE_TIMER_MODE_PERIOD     0b00000010  //period=1, timeout=0
#define STATE_TIMER_PAUSE           0b00000100
#define STATE_ATTACK_ON             0b00001000
#define STATE_ATTACK_PAUSE          0b00010000
#define STATE_ONE_SEC_AFTER_PERIOD  0b00100000
#define STATE_ONE_SEC_AFTER_ATTACK  0b01000000
#define STATE_ONE_SEC_AFTER_TIMEOUT 0b10000000

// отправка данных по однопроводному юарту
#include "softUART.h"
// делаем только отправителем (экономит память)
softUART<LINK_READER_PIN, GBUS_TX> UART(LINK_SENDER_TO_READER_SPEED); // пин 12, скорость 1200

// подключаем GBUS
#include "GBUS.h"
GBUS bus(&UART, SENDER_ADDRESS, 13); // обработчик UART, адрес 1, буфер 13 байт

//#include <EncButton2.h>
//EncButton2<EB_BTN> button1(INPUT_PULLUP, 4);
//EncButton2<EB_BTN> button2(INPUT_PULLUP, 6);
#include "GyverButton.h"
GButton buttonAttack14(BUTTON_ATTACK_14_PIN, HIGH_PULL, NORM_CLOSE); // атака 14 секунд 
GButton buttonAttack24(BUTTON_ATTACK_24_PIN, HIGH_PULL, NORM_CLOSE); // атака 24 секунды
GButton buttonAttackPause(BUTTON_ATTACK_PAUSE_PIN, HIGH_PULL, NORM_CLOSE); // атака пауза
GButton buttonAttackHide(BUTTON_ATTACK_HIDE_PIN, HIGH_PULL, NORM_CLOSE); // атака скрыть
GButton buttonTone1(BUTTON_TONE1_PIN, HIGH_PULL, NORM_CLOSE); // гудок
GButton buttonTone2(BUTTON_TONE2_PIN, HIGH_PULL, NORM_CLOSE); // гудок  
GButton buttonPeriodPause(BUTTON_PERIOD_PAUSE_PIN, HIGH_PULL, NORM_CLOSE); // период пауза   
GButton buttonTimeout(BUTTON_TIMEOUT_PIN, HIGH_PULL, NORM_CLOSE); // таймаут

struct data_send_struct {
  byte system_state;
  uint32_t period_or_timeout_timer;
  uint32_t attack_timer;
} data_send;

union{
  byte v1[9];
  data_send_struct v2;
}buf;

uint32_t timer_buttons_old;
uint32_t timer_attack_old;
uint32_t timer_send_old;
uint32_t firstSecondAfterAttackTimer;

void setup() {
  Serial.begin(9600);

  buttonAttack14.setDebounce(5); 
  buttonAttack14.setTimeout(500);
  buttonAttack14.setClickTimeout(75);
  buttonAttack24.setDebounce(5); 
  buttonAttack24.setTimeout(500);
  buttonAttack24.setClickTimeout(75);
  buttonAttackPause.setDebounce(5); 
  buttonAttackPause.setTimeout(500);
  buttonAttackPause.setClickTimeout(75);
  buttonAttackHide.setDebounce(5); 
  buttonAttackHide.setTimeout(500);
  buttonAttackHide.setClickTimeout(75);
  buttonTone1.setDebounce(5); 
  buttonTone1.setTimeout(500);
  buttonTone1.setClickTimeout(75);
  buttonTone2.setDebounce(5); 
  buttonTone2.setTimeout(500);
  buttonTone2.setClickTimeout(75);
  buttonPeriodPause.setDebounce(5); 
  buttonPeriodPause.setTimeout(500);
  buttonPeriodPause.setClickTimeout(75);
  buttonTimeout.setDebounce(5); 
  buttonTimeout.setTimeout(500);
  buttonTimeout.setClickTimeout(75);
  
  timer_buttons_old=millis();
  timer_attack_old=millis();
  timer_send_old=millis();
  firstSecondAfterAttackTimer = 0;

  data_send.system_state = 0;
  data_send.system_state = STATE_ATTACK_ON |
                           STATE_ATTACK_PAUSE;
  data_send.period_or_timeout_timer = 0;
  data_send.attack_timer = 0;
}

void loop() {
  
  bus.tick(); // не реже 1'000'000/LINK_SENDER_TO_READER_SPEED/4
  
  buttons_tick(50);
  //attack_tick(100);
  
  if (Serial.available() > 0) {
    Serial.readBytes(buf.v1, 5);
    data_send.system_state = (data_send.system_state & (STATE_ATTACK_ON | STATE_ATTACK_PAUSE | STATE_ONE_SEC_AFTER_ATTACK)) | buf.v2.system_state;
    data_send.period_or_timeout_timer = buf.v2.period_or_timeout_timer;
    state_check(100); //должна быть та же частота, с которой идет отправка
    bus.sendData(READER_ADDRESS, data_send);  // на адрес 2
  }
}
