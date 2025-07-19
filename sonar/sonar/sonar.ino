int trigPin = 3; // назначаем имя для Pin8
int echoPin = 2; // назначаем имя для Pin9
int led1 = 5;
int led2 = 6;
 
void setup() { 
  Serial.begin (9600); // подключаем монитор порта
  pinMode(trigPin, OUTPUT); // назначаем trigPin, как выход
  pinMode(echoPin, INPUT); // назначаем echoPin, как вход
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
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


  if (cm <= 10 ){
    digitalWrite(led1, HIGH);
  } else {
    digitalWrite(led1, LOW);
  }

    if (cm >= 10 ){
    digitalWrite(led2, HIGH);
  } else {
  
    digitalWrite(led2, LOW);
  }

  delay(1000); // ставим паузу в 1 секунду
}