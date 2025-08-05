#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
#include <GyverOLED.h>

GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;

// Выводы для подключения светодиодов
#define LED_PIN_OK 2     // Зелёный светодиод (правильная карта), подключён к D2
#define LED_PIN_ERR 3    // Красный светодиод (неверная карта), подключён к D3

// Массив известных UID карт
const byte knownUids[][4] = {
  {0xA1, 0xB3, 0xF6, 0x3}  // Карта №5
};

// Количество поддерживаемых карт
const size_t numCards = sizeof(knownUids)/sizeof(knownUids[0]);

PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);

void setup() {
  Serial.begin(9600);
  Serial.println("Hello!");
  oled.init();              // инициализация
  Wire.setClock(800000L);

  // Конфигурируем выводы светодиодов
  pinMode(LED_PIN_OK, OUTPUT);  // D2 (светодиод для правильной карты)
  pinMode(LED_PIN_ERR, OUTPUT); // D3 (светодиод для неправильной карты)

  // Оба светодиода начально погашены
  digitalWrite(LED_PIN_OK, LOW);
  digitalWrite(LED_PIN_ERR, LOW);

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // Останавливаемся здесь
  }

  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();

  Serial.println("Waiting for an ISO14443A card");
}

void loop() {
  
  wait();
 
  boolean success;
  uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};
  uint8_t uidLength;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

  if (success) {
    Serial.println("Found a card!");
    Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
    Serial.print("UID Value: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(" 0x"); Serial.print(uid[i], HEX);
    }
    Serial.println("");

    // Определяем карту по её UID
    switchCardAction(uid, uidLength);

    delay(1000);          // Пауза перед следующим циклом чтения
  } else {
    Serial.println("Timed out waiting for a card");
    wait();
  }
}

// Функция определяет карту и запускает соответствующее ей действие
void switchCardAction(uint8_t *uid, uint8_t uidLength) {
  bool foundMatch = false;

  for(int i = 0; i < numCards; i++) {
    if(compareUids(uid, knownUids[i], min(sizeof(knownUids[i]), uidLength))) {
      foundMatch = true;
      oled.update();
      
      // Выполняем различные действия в зависимости от номера карты
      switch(i) {
        case 0: actionForCard1(); break;
        
        default: break;
      }
      break;
    }
  }

  if(!foundMatch) {
    unknownCardDetected();
    
  }
}

// Сравнение двух UID
bool compareUids(const uint8_t* uid, const uint8_t* knownUid, uint8_t length) {
  for(uint8_t i=0;i<length;i++){
    if(uid[i]!=knownUid[i]){
      return false;
    }
  }
  return true;
}

void deny() {
  oled.clear();
  oled.setScale(3);
  oled.setCursorXY(0, 0);
  oled.print("DENY");
  oled.update();
}

void OK() {
  oled.clear();
  oled.setScale(3);
  oled.setCursorXY(0, 0);
  oled.print("OK");
  oled.update();
}

void wait() {
  oled.clear();
  oled.setScale(3);
  oled.setCursorXY(0, 0);
  oled.print("Wait");
  oled.update();
}

void draw_1() {

  oled.clear();
  oled.setScale(3);
  oled.setCursorXY(0, 0);
  oled.print("Card 1");
  oled.update();

}


 




void actionForCard1() {
  Serial.println("Access granted to Card 0");
  digitalWrite(LED_PIN_OK, HIGH);  // Включаем зеленый светодиод
  OK();
  delay(2000);                   // Держим экран 2 секунды
  digitalWrite(LED_PIN_OK, LOW);  // Выключаем зеленый светодиод
}





// Обработка неизвестной карты
void unknownCardDetected() {
  Serial.println("Unknown card detected!");
  digitalWrite(LED_PIN_ERR, HIGH);
  deny(); // Включаем красный светодиод
  delay(2000);                    // Ждем 2 секунды
  digitalWrite(LED_PIN_ERR, LOW);  // Выключаем красный светодиод
}

// Экранные сообщения




void draw_yatavo() {
  int time = 2000;

  oled.clear();
  oled.setScale(3);
  oled.setCursorXY(0, 0);
  oled.print("Made by");
  oled.setCursorXY(0, 30);
  oled.print("YATAVO");
  oled.update();
}