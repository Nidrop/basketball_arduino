#define M_PIN 7           // Пин подключения LED матрицы
#define M_WIDTH 33        // Ширина матрицы
#define M_HEIGHT 9    
#define NUM_LEDS (M_WIDTH * M_HEIGHT) 
#define COLOR_DEBTH 1

#define TEXT_HEIGHT 0     // высота, на которой бежит текст (от низа матрицы)
#define LET_WIDTH 9       // ширина буквы шрифта
#define NUMBER_WIDTH 7    // ширина цифры шрифта
#define LET_HEIGHT 9      // высота буквы шрифта
#define SPACE 1           // пробел

#define SM16188_PIN_D1 2
#define SM16188_PIN_D2 3

#define LED_STRING_RED_PIN 9
#define LED_STRING_YELLOW_PIN 10

//gbus
#define LINK_SENDER_PIN 12 //D12
#define LINK_SENDER_TO_READER_SPEED 3800
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
#define STATE_ONE_SEC_AFTER_5_SEC_LEFT_PERIOD  0b10000000

#include <microLED.h>  
#include "My_Font.h"
#include <softUART.h>
#include <GBUS.h>

microLED<NUM_LEDS, M_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER> 
 matrix(M_WIDTH, M_HEIGHT, ZIGZAG, LEFT_TOP, DIR_RIGHT);


char russian[] = "Судейская система <БАСКЕТБОЛ>___BELSPORT.PRO";


softUART<LINK_SENDER_PIN, GBUS_RX> UART(LINK_SENDER_TO_READER_SPEED); // пин 12, скорость 1000

GBUS bus(&UART, READER_ADDRESS, 13); // обработчик UART, адрес 2, буфер 40 байт

struct data_send_struct {
  byte system_state;
  uint32_t period_or_timeout_timer;
  uint32_t attack_timer;
} data_send;

union{
  byte v1[9];
  data_send_struct v2;
}buf;

uint32_t stale_timer;

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(LED_STRING_RED_PIN, OUTPUT);     
  pinMode(LED_STRING_YELLOW_PIN, OUTPUT);
  digitalWrite(LED_STRING_RED_PIN, LOW);
  digitalWrite(LED_STRING_YELLOW_PIN, LOW);

  pinMode(SM16188_PIN_D1, OUTPUT);
  pinMode(SM16188_PIN_D2, OUTPUT);
  digitalWrite(SM16188_PIN_D1, LOW);
  digitalWrite(SM16188_PIN_D2, LOW);

  drawBitmap_32x32_sm16188(0,0);
  updateScreen_sm16188();
  
  matrix.setBrightness(100);
  matrix.clear();
  matrix.show();

  stale_timer = 0;  //таймер потери связи
  
  data_send.system_state = 0;
  data_send.period_or_timeout_timer = 0;
  data_send.attack_timer = 0;
}

void loop()
{
  bus.tick();

//  if(Serial.available() > 0){
//    Serial.readBytes(buf.v1, 9);
//    data_send.system_state = buf.v2.system_state;
//    state_check();
//  }
  
  if (bus.gotData()) {
    bus.readData(data_send);
    
    //Serial.println(data_send.period_or_timeout_timer);

    state_check();
    stale_timer = millis();  //"обнуляем", т.к. связь не потеряна
  }

  stale_tick(1000); //1 секунда на проверку потери связи
}
 
