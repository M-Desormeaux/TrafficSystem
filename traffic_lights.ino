// === Michael Desormeaux  ====
// Final Project Component, Option#1 

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP32.h>
int ONOFF ;
const int LED0 = 16;                  //GPIO16  to trigger the emergency button

  // === Wifi Information
  char *ssid = "---";
  char *wifiPassword = "---";

  // Cayenne Info
  char username[] = "7571c550-35c2-11eb-883c-638d8ce4c23d";  
  char password[] = "048d97293070b6e96c147278cd53d898ee74964e";   
  char clientID[] = "d89320b0-3a69-11eb-a2e4-b32ea624e442";   
  // === End Cayenne token and SSID/PW Setting ===
  // =============================================

// === LCD Libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// === LCD Addressing ===
LiquidCrystal_I2C lcd(0x27,16,2);

// === GPIO pin settings for different components
const int buzzer = 32;
const int red_LED1  = 14;
const int yellow_LED1  = 12;
const int green_LED1 = 13;
const int red_LED2  = 25;
const int yellow_LED2  = 26;
const int green_LED2 = 27;


int crosswalk_value;
const int crosswalk_button = 19;

void setup() {
  Serial.begin(115200);
  pinMode(crosswalk_button, INPUT_PULLUP);
  pinMode(LED0, OUTPUT);
  digitalWrite(LED0, LOW);

  Serial.print("WIFI status = ");
  Serial.println(WiFi.getMode());
  
  WiFi.disconnect(true);
  delay(1000);
  WiFi.mode(WIFI_STA);
  delay(1000);
  Serial.print("WIFI status = ");
  Serial.println(WiFi.getMode());
  WiFi.begin(ssid, password);
  
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("=== CEIS114 ===");
  
  pinMode(buzzer, OUTPUT); 
  
  pinMode(red_LED1, OUTPUT);
  pinMode(yellow_LED1, OUTPUT);
  pinMode(green_LED1, OUTPUT);
  
  pinMode(red_LED2, OUTPUT);
  pinMode(yellow_LED2, OUTPUT);
  pinMode(green_LED2, OUTPUT);
}

void loop() {
  Cayenne.loop();
  
  crosswalk_value=digitalRead(crosswalk_button);
  if(crosswalk_value == 0 or ONOFF == 1) {
    digitalWrite(yellow_LED1, LOW);
    digitalWrite(green_LED1, LOW);
    digitalWrite(yellow_LED2, LOW);
    digitalWrite(green_LED2, LOW);
    
    for (int i=10; i>= 0; i--) {
      Serial.print(" Count =  ");
      Serial.print(i);
      Serial.println("  == Walk ==  ");
      lcd.setCursor(0,1);
      lcd.print("                ");
      lcd.setCursor(0,1);
      lcd.print(" == Walk ==   ");
      lcd.print(i);
      //=== can be RED only no flashing ===
      digitalWrite(red_LED1, HIGH);
      digitalWrite(red_LED2, HIGH);
      digitalWrite(buzzer, HIGH);
      delay(500);
      digitalWrite(red_LED1, LOW);
      digitalWrite(red_LED2, LOW);
      digitalWrite(buzzer, LOW);
      delay(500);
    }
    
    lcd.setCursor(0,1);
    lcd.print("                ");
  } // === end of button pressed loop ===
  
  else {
    lcd.setCursor(0,1);
    lcd.print(" = Do Not Walk =");
    
    Serial.println("  == Do Not Walk ==  ");

    digitalWrite(red_LED1, HIGH);
    digitalWrite(yellow_LED1 , LOW);
    digitalWrite(green_LED1, LOW);
    digitalWrite(red_LED2, LOW);
    digitalWrite(yellow_LED2 , LOW);
    digitalWrite(green_LED2, HIGH);
    delay(2000);
    
    digitalWrite(yellow_LED2 , HIGH);
    digitalWrite(green_LED2, LOW);
    delay(2000);
    
    digitalWrite(red_LED2, HIGH);
    digitalWrite(yellow_LED2 , LOW);
    delay(1000);
    
    digitalWrite(red_LED1, LOW);
    digitalWrite(green_LED1, HIGH);
    delay(2000);
    
    digitalWrite(yellow_LED1 , HIGH);
    digitalWrite(green_LED1, LOW);
    delay(2000);
    
    digitalWrite(red_LED1, HIGH);
    digitalWrite(yellow_LED1 , LOW);
    delay(1000);
    
  } // === LED Loop Closing ===
}
  
CAYENNE_IN(1) {
  ONOFF = getValue.asInt();
  digitalWrite(LED0, ONOFF);
  //digitalWrite(LED0,getValue.asInt());
}
