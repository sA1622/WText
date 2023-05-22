#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>
#include<LiquidCrystal_I2C.h>
RF24 radio(7,8);
LiquidCrystal_I2C lcd(0x27,16,4);
byte adress[6]=""//adresa walkie talkie
int i=0;
void setup() {
  radio.begin();
  lcd.begin()
  lcd.backLight();
  while(adress[5]!=NULL)
  {
    //read adress
    adress[i] = 
  }
  radio.openWritingPipe(adress);
  radio.setPALevel();
  radio.setDataRate();
  radio.stopListening();
}

void loop() {
  
}
