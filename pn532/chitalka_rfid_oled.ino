  #include <Wire.h>
  #include <PN532_I2C.h>
  #include <PN532.h>
  #include <NfcAdapter.h>
  #include <GyverOLED.h>

GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
  
  PN532_I2C pn532i2c(Wire);
  PN532 nfc(pn532i2c);

  
void setup(void) {
  Serial.begin(9600);
  Serial.println("Hello!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); 
  }
  

  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  link();

  

  nfc.setPassiveActivationRetries(0xFF);
  

  nfc.SAMConfig();
    
  Serial.println("Waiting for an ISO14443A card");
  wait();
}

void loop(void) {
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  
  uint8_t uidLength;                        
  

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

  if (success) {
    Serial.println("Found a card!");
    Serial.print("UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("UID Value: ");


    const int elementsPerLine = 4;


    int lineNumber = 0;

   
    oled.clear();
    oled.setScale(1); 

    
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(" 0x");Serial.print(uid[i], HEX); 

      
      String elementStr = "0x" + String(uid[i], HEX);

      
      if (i % elementsPerLine == 0) {
        
        oled.setCursorXY(0, lineNumber * 16); 
        lineNumber++;
      }

      
      oled.print(elementStr);
      oled.print(" "); 
    }

    Serial.println("\\n");

    
    oled.update();

    delay(1000); 
  }
  else {
    
    Serial.println("Timed out waiting for a card");
    wait();
  }
}


void link() {
  oled.clear();
  oled.setScale(2);
  oled.setCursorXY(0, 0);
  oled.print("LINKED!");
  oled.update();
}



void wait() {
  oled.clear();
  oled.setScale(3);
  oled.setCursorXY(0, 0);
  oled.print("Wait");
  oled.update();
}
