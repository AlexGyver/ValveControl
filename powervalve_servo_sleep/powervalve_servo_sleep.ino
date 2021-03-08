/*
Created 2016
by AlexGyver
AlexGyver Home Labs Inc.
*/
#include <avr/sleep.h>   //библиотека режимов сна
#include <Servo.h> //билиотека серво

Servo servo;     //объявляем переменную servo типа Servo

byte startpos=145; //заднее положение сервы (затвор открыт)
byte stoppos=51;  //переднее положение сервы (затвор закрыт)
volatile unsigned long lastshot;  //время последнего выстрела для таймера сна

boolean trig_state; //переменная для хранения текущего состояняи кнопки
boolean flag; //флажок для запоминания положения кнопки
byte trigger; //положение спусковой кнопки
byte mode; //положение смены типа стрельбы  авто(1) / одиночные (0)
int del=10;  //задержка клапана, миллисекунды

byte mode_pin = 5; //кнопка переключения режимов на D5 (необязательная, без неё код тоже будет работать)
byte servo_pin = 4; //серво на D4
byte trigger_pin = 3; //спусковая кнопка на D3
byte valve_pin = 2; //реле или мосфет на D2

void setup() {
  Serial.begin(9600);
  pinMode(mode_pin, INPUT_PULLUP); //пин кнопки режимов огня подтянут внутренним резистором, резистор на 10 кОм НЕ НУЖЕН
  pinMode(trigger_pin, INPUT_PULLUP); //пин кнопки подтянут внутренним резистором, резистор на 10 кОм НЕ НУЖЕН
  pinMode(valve_pin, OUTPUT); //сформировать пин реле как выход
  servo.attach(servo_pin); //подключить серво
  attachInterrupt(1,trigger_press,FALLING);  //аппаратное прерывание при нажатии на кнопку (выстрел/проснуться)
  servo.write(stoppos);   //закрыть затвор при включении системы
}

void trigger_press() { // обработчик прерывания  
  //просыпаемся при нажатии кнопки!
}

void loop() {
  
  trig_state = digitalRead(trigger_pin); //считать положение спусковой кнопки
  
  if (digitalRead(mode_pin) == 1) { //считать положение кнопки переключения режимов
    mode = 0;
  } else {mode = 1;}

  if(trig_state==0 && mode==0 && flag==0) { //если спусковая кнопка нажата, выбран режим стрельбы ОДИНОЧНЫМИ и флаг=0
    shot();
    flag=1;         //поставить флажок, что кнопка нажата
  } 

  if(trig_state==1 && mode==0 && flag==1) { //если кнопка отжата, выбран режим стрельбы ОДИНОЧНЫМИ и флаг=1            
    flag=0;    //ставим флажок, что кнопка больше не нажата
  }
  
  if(trig_state==0 && mode==1 && flag==0) { //если спусковая кнопка нажата, выбран АВТОМАТИЧЕСКИЙ режим стрельбы и флаг=0
    shot();
  }  

// этот кусок даёт команду спать
  if (millis()-lastshot > 3000) {    //если после последнего выстрела прошло больше 3 секунд
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // выбор режима энергопотребления
    sleep_mode();                        // уходим в спячку
  }
// этот кусок даёт команду спать

delay(1);   //задержка для стабильности работы прошивки
}

void shot() { //функция выстрела
  digitalWrite(valve_pin,HIGH);  //открыть клапан
  delay(del);       //подождать время, равное del 
  digitalWrite(valve_pin,LOW);  //закрыть клапан
  delay(10);  //подождать немного, чтобы воздух вышел из ствола
  servo.write(startpos);  //открыть затвор
  delay(272);       //подождать серву (шарик попадает на магнит)
  servo.write(stoppos);   //закрыть затвор
  delay(272);       //столько же подождать серву (шарик попадает в ствол)
  lastshot=millis();    //запомнить время последнего выстрела (для таймера сна)
}