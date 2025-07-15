#include <Wire.h>            
#include <LiquidCrystal_I2C.h> 

LiquidCrystal_I2C lcd(0x27, 20, 4); 

int led1 = 6;  
int led2 = 7;         
int ledAkym4 = 2;       
int ledAkym3 = 3;       
int ledAkym2 = 4;       
int ledAkym1 = 5;       
int hladLed = 8;  
int alarmLed = 10;
int zapasLed = 11;      
const int hladTumb = 9;  
int hladTumbState = 0; 
int asistLed = 13; 
const int asistTumb = 12;  
bool asistTumbState = false;  
float temperature = 600.0; 
int asist = 0; 
enum CoolingState { IDLE, COOLING };
CoolingState currentState = IDLE;
unsigned long lastChangeTime = 0; 
unsigned long coolingInterval = 100; 

void setup() {
  Wire.begin();              
  lcd.init();                
  lcd.backlight();           

  pinMode(led1, OUTPUT);  
  pinMode(led2, OUTPUT);   
  pinMode(ledAkym4, OUTPUT); 
  pinMode(ledAkym3, OUTPUT); 
  pinMode(ledAkym2, OUTPUT); 
  pinMode(ledAkym1, OUTPUT); 
  pinMode(hladLed, OUTPUT);  
  pinMode(hladTumb, INPUT); 
  pinMode(asistTumb, INPUT);
  pinMode(alarmLed, OUTPUT);  
  pinMode(zapasLed, OUTPUT);  
  pinMode(asistLed, OUTPUT);

  digitalWrite(led1, HIGH);  
  delay(100);
  digitalWrite(led2, HIGH);
  delay(300);                
  digitalWrite(ledAkym1, HIGH);
  delay(100);                
  digitalWrite(ledAkym2, HIGH);
  delay(100);                
  digitalWrite(ledAkym3, HIGH);
  delay(100);                
  digitalWrite(ledAkym4, HIGH);
}

void loop() {
  unsigned long currentMillis = millis();

 
  hladTumbState = digitalRead(hladTumb);
  asistTumbState = digitalRead(asistTumb);


  switch(currentState) {
    case IDLE:
    
      if (hladTumbState == HIGH) { 
        digitalWrite(hladLed, HIGH); 
        temperature -= 5;         
      } else {                      
        digitalWrite(hladLed, LOW); 
        temperature += 5;           
      }


      if (temperature >= 3700 && asistTumbState == true) {
        currentState = COOLING;
        lastChangeTime = currentMillis;
      }
      break;

    case COOLING:
  
      if ((currentMillis - lastChangeTime) >= coolingInterval) {
        temperature -= 5;          
        lastChangeTime = currentMillis;
        digitalWrite(hladLed, HIGH);

        
        if (temperature <= 3600) {
          currentState = IDLE;
          digitalWrite(hladLed, LOW);
        }
      }
      break;
  }


  if (asistTumbState == true) {   
    digitalWrite(asistLed, HIGH); 
  } else {
    digitalWrite(asistLed, LOW); 
  }




  if (temperature <= 600) {
    digitalWrite(led2, LOW);
    digitalWrite(zapasLed, HIGH);

  } else {
    digitalWrite(led2, HIGH);
    digitalWrite(zapasLed, LOW);
  }

  if (temperature <= 200) {
    
    digitalWrite(led1, LOW);
   
  } else {
    
    digitalWrite(led1, HIGH);
    
  }


  if (temperature >= 3700 ) {
    digitalWrite(alarmLed, HIGH);
  } else {
    digitalWrite(alarmLed, LOW);
  }

    if (temperature >= 3800 ) {
    digitalWrite(alarmLed, HIGH);
    delay(100);
    digitalWrite(led1, LOW);
    delay(100);
    digitalWrite(led2, LOW);
    delay(100);
    digitalWrite(ledAkym1, LOW); 
    delay(100);
    digitalWrite(ledAkym2, LOW); 
    delay(100);
    digitalWrite(ledAkym3, LOW); 
    delay(100);
    digitalWrite(ledAkym4, LOW);
    delay(99999999);
  } 
   
    if (temperature <= -273 ) {
    digitalWrite(alarmLed, LOW);
    delay(100);
    digitalWrite(led1, LOW);
    delay(100);
    digitalWrite(led2, LOW);
    delay(100);
    digitalWrite(ledAkym1, LOW); 
    delay(100);
    digitalWrite(ledAkym2, LOW); 
    delay(100);
    digitalWrite(ledAkym3, LOW); 
    delay(100);
    digitalWrite(ledAkym4, LOW);
    delay(100);
    digitalWrite(zapasLed, LOW);
    delay(100);
    digitalWrite(hladLed, LOW);
    delay(99999999);
  } 


  float power = min(max((temperature - 600) / 3, 0), 1000); 
  float percent = power / 1000 * 100;                        


  if (power > 0 && power <= 250) {
    digitalWrite(ledAkym1, HIGH); 
    digitalWrite(ledAkym2, LOW); 
    digitalWrite(ledAkym3, LOW); 
    digitalWrite(ledAkym4, LOW); 
  } else if (power > 250 && power <= 500) {
    digitalWrite(ledAkym1, HIGH); 
    digitalWrite(ledAkym2, HIGH); 
    digitalWrite(ledAkym3, LOW); 
    digitalWrite(ledAkym4, LOW); 
  } else if (power > 500 && power <= 750) {
    digitalWrite(ledAkym1, HIGH); 
    digitalWrite(ledAkym2, HIGH); 
    digitalWrite(ledAkym3, HIGH); 
    digitalWrite(ledAkym4, LOW); 
  } else if (power > 750) {
    digitalWrite(ledAkym1, HIGH); 
    digitalWrite(ledAkym2, HIGH); 
    digitalWrite(ledAkym3, HIGH); 
    digitalWrite(ledAkym4, HIGH); 
  } else {
    digitalWrite(ledAkym1, LOW); 
    digitalWrite(ledAkym2, LOW); 
    digitalWrite(ledAkym3, LOW); 
    digitalWrite(ledAkym4, LOW); 
  }

  char tempStr[10];
  dtostrf(temperature, 4, 1, tempStr); 

  char powStr[10];
  dtostrf(power, 4, 0, powStr);        

  char percStr[10];
  dtostrf(percent, 4, 0, percStr);     


  lcd.clear();             
  lcd.setCursor(0, 0);     
  lcd.print("Temp: ");     
  lcd.print(tempStr);      
  lcd.print(" *C");        

  lcd.setCursor(0, 1);     
  lcd.print("Power: ");    
  lcd.print(powStr);       
  lcd.print(" MW");        

  lcd.setCursor(0, 2);     
  lcd.print("Percent: ");  
  lcd.print(percStr);      
  lcd.print("%");  

  lcd.setCursor(0, 3);          
  lcd.print("asist: ");
  lcd.print(asistTumbState);

     

  delay(100);                 
}
