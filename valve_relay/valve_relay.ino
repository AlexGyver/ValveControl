int del=5;   //переменная для хранения времени задержки (миллисекунды)
byte flag;   //флажок

void setup() {
	pinMode(2,OUTPUT);  //сюда подключено реле
	pinMode(4,OUTPUT);  //сюда подключено питание для кнопки
	pinMode(3,INPUT);  //считываем состояние кнопки
	digitalWrite(4,HIGH);  //питание для кнопки
}

void loop() {
	if (digitalRead(3)==1 && flag==0) {   //если кнопка нажата и флаг опущен
		digitalWrite(2,HIGH);  //открыть клапан
		delay(del);   //подождать чутка
		digitalWrite(2,LOW);  //закрыть клапан
		flag=1;  //поднять паруса! Ой, флаг
		delay(100);   //защита от дребезга
	}

	if (digitalRead(3)==0 && flag==1) {  //если кнопка отпущена и поднят флаг (былсовершён выстрел)
		flag=0;  //опустить флаг
	}
	delay(2);  //задержка для стабильности работы
}
