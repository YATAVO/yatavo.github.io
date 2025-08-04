#include <GyverOLED.h>
#include <Servo.h> // подключаем библиотеку для работы с сервоприводом
#include <DHT.h>


GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
Servo servo1;

int trigPin = 9; // назначаем имя для Pin8
int echoPin = 8; // назначаем имя для Pin9

#define DHTPIN 3       // Подключен к цифровому выводу 2
#define DHTTYPE DHT11   // Тип датчика
DHT dht(DHTPIN, DHTTYPE);



void setup() {
  Serial.begin(9600);
  oled.init();              // инициализация
  Wire.setClock(800000L);

  servo1.attach(2); // привязываем сервопривод к аналоговому выходу 11

  pinMode(trigPin, OUTPUT); // назначаем trigPin, как выход
  pinMode(echoPin, INPUT); // назначаем echoPin, как вход
  dht.begin(); // Инициализация датчика DHT11

}

void loop() {

   int duration, cm; // назначаем переменную "cm" и "duration" для показаний датчика
  digitalWrite(trigPin, LOW); // изначально датчик не посылает сигнал
  delayMicroseconds(2); // ставим задержку в 2 ммикросекунд

  digitalWrite(trigPin, HIGH); // посылаем сигнал
  delayMicroseconds(10); // ставим задержку в 10 микросекунд
  digitalWrite(trigPin, LOW); // выключаем сигнал

  duration = pulseIn(echoPin, HIGH); // включаем прием сигнала

  cm = duration / 58; // вычисляем расстояние в сантиметрах

  Serial.print(cm); // выводим расстояние в сантиметрах
  Serial.println(" cm");

  if (cm <= 10) {
    open();

 
    
    

  } else {

    close();


  }

  draw_dht();






}


void open() {

 servo1.write(85);
     oled.clear();
    oled.setScale(3);
    oled.home();
    oled.print("Open");
  oled.update();
  delay(1000);




}

void close() {

servo1.write(10);




}

void draw_dht() {
  float temperature = dht.readTemperature(); // Читаем температуру
  float humidity = dht.readHumidity();       // Читаем влажность

  oled.clear();

  oled.setScale(1);
  oled.setCursorXY(0, 0);
  oled.print("Температура:");
  oled.setScale(2);
  oled.setCursorXY(0, 10);
  oled.print(String(temperature) + " *C");



  oled.setScale(1);
  oled.setCursorXY(0, 30);
  oled.print("Влажность:");
  oled.setScale(2);
  oled.setCursorXY(0, 40);
  oled.print(String(humidity) + " %");
  oled.update();

  delay(1000);



}

void draw_yatavo() {


  int time = 2000;

  oled.clear();
  oled.setScale(3);
  oled.setCursorXY(0, 0);
  oled.print("Made by");
  oled.setCursorXY(0, 30);
  oled.print("YATAVO");
  oled.update();



  delay(time);

  oled.clear();

  oled.setScale(1);
  oled.setCursorXY(0, 0);
  oled.print("Website:");
  oled.setCursorXY(0, 10);
  oled.print("YATAVO.github.io");

  oled.setCursorXY(0, 30);
  oled.print("GitHub:");
  oled.setCursorXY(0, 40);
  oled.print("github.com/YATAVO");
  oled.update();

  delay(time);


}


