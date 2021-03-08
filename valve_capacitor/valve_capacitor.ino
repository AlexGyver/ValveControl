/*
Created 2016
by AlexGyver
AlexGyver Home Labs Inc.
*/
#include <avr/sleep.h>   //библиотека режимов сна

byte flag1,flag2;  //флажок, что кнопка была нажата
int del=15;  //задержка клапана в миллисекундах
int valve=2;  //клапан на 5 цифровом
volatile unsigned long lastshot;  //время последнего выстрела для таймера сна

void setup()    
{       
	pinMode(valve, OUTPUT);  //клапан (транзистор или реле)
	attachInterrupt(1,shot,RISING);     //аппаратное прерывание при нажатии на кнопку (выстрел/проснуться)
} 

void shot() {
	lastshot=millis();    //запомнить время последнего выстрела (для таймера сна)
	if (flag2==0) {
		flag1=1;           //выстрел
	}
}

void loop() { 
	if (flag1==1 && flag2==0) {
		digitalWrite(valve,HIGH);  //открыть клапан
		delay(del);       //подождать время, равное del
		digitalWrite(valve,LOW);  //закрыть клапан
		flag1=0;
		flag2=1;
		delay(50);    //защита от дребезга контактов
	}
	if (digitalRead(3)==0 && flag2==1) {
		flag2=0;       //кнопка отпущена
	}

	delay(1);   //задержка для стабильности работы прошивки

	if (millis()-lastshot>3000) {    //если после последнего выстрела прошло больше 3 секунд
		set_sleep_mode(SLEEP_MODE_PWR_DOWN); // выбор режима энергопотребления
		sleep_mode();                        // уходим в спячку
	}
} 


