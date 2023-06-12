#include <Keypad.h>
//#include <String.h>
#include<LiquidCrystal_I2C.h>
#include<SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
int timepressed[17] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
byte messageLocation = 0;
char text_sender[32] = "";
LiquidCrystal_I2C lcd(0x27,20,4);
unsigned int i=0;
String message_final;
String message_ready;
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 100;
bool adress_get = false;
bool pipe_get = false;
bool sender = false;
bool reciver = false;
char adress[6];
int adress_final;
char pipe[2];
int pipe_final;
int k=0;
String text_out = "";
bool sender_mode = false;
bool reciver_mode = false;
byte recived_message_icon[] = {
  B00100,
  B01110,
  B01110,
  B01110,
  B11111,
  B00000,
  B00100,
  B00000
};
RF24 radio(7, 8); 
//int times=0; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// connect the pins from right to left to pin 2, 3, 4, 5,6,7,8,9
byte rowPins[ROWS] = {5,6,2,3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A0,A1,A2,A3}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const byte addresses[][6] = {"00001", "00002"}; //This device ID is 00002 -> The second device => Reads on the 00002 address

void setup(){
  Serial.begin(9600);
  startMillis = millis();
  radio.begin();
  lcd.begin();
  lcd.backlight();
  start_text();
  lcd.createChar(0,recived_message_icon);
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MIN);
  text_out.reserve(32);
  /*if(message_ready.length()>5&&adress_get==false)
  {
    //nrf adress =(message_ready);
    lcd.clear();
    adress_get = true;
    lcd.setCursor(0,0);
    message_ready.remove(message_ready.length()-1);
    message_ready.toCharArray(adress,sizeof(adress));
    adress_final = atoi(adress);
    lcd.print("Adress: " + message_ready);
    //Serial.println(message_ready);
    Serial.println("Adress");
    Serial.println(adress_final);
    delay(2000);
    for(int a=0;a<16;a++) timepressed[a] = 1;
    delay(1000);
    lcd.clear();
    message_ready = "";
    //i=0;
  }
  if(message_ready.length()>1&&adress_get==true&&pipe_get==false)
  {
    //nrf pipe(message_ready);
    lcd.clear();
    pipe_get = true;
    lcd.setCursor(0,0);
    message_ready.remove(message_ready.length()-1);
    message_ready.toCharArray(pipe,sizeof(pipe));
    pipe_final = atoi(pipe);
    lcd.print("Pipe: " + message_ready);
    Serial.println(message_ready);
    Serial.println("Pipe");
    Serial.println(pipe_final);
    delay(2000);
    for(int a=0;a<16;a++) timepressed[a] = 1;
    delay(1000);
    lcd.clear();
    message_ready = "";
    //i=0;
  }
  message_final.reserve(128);
  currentMillis = millis();
  /////keyboard
  Keyboard_funct(); 
  Serial.println(message_ready);
  LcdDisplayMessage();
  checkStringLastChar();*/
}
  

void loop(){
  
  //message_final.reserve(32); //Max nRF24l01 buffer size
  message_final.reserve(128);
  message_ready.reserve(32);
  currentMillis = millis();
  /////keyboard
  Keyboard_funct(); 
  Serial.println(message_ready);
  LcdDisplayMessage();
  checkStringLastChar();
  Reciver_funct();
}
void checkStringLastChar()
{
  message_ready.reserve(32);
  //if(message_final.charAt(message_final.length()-1) != message_ready.charAt(message_ready.length()-1)&&times<10){ //&&times<10
    message_ready.concat(message_final);
    //Serial.println(times);
    
  //}
  //message_ready.remove(message_ready.length()-1);
  message_final="\0";
}
void LcdDisplayMessage()
{
  if(message_ready.length()<80){ 
    switch(message_ready.length())
    {
      case 0:
      lcd.setCursor(0,0);
      break;
      case 20:
      lcd.setCursor(0,1);
      break;
      case 40:
      lcd.setCursor(0,2);
      break;
      case 60:
      lcd.setCursor(0,3);
      break;
    }
    lcd.print(message_final);
  }
  //times=0; 
}
/*
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
*/
void Keyboard_funct()
{
  char key = keypad.getKey();
  switch(key)
  {
    //if(i<256)
    //{
    case '1': //49
    //times++;
    i++; //urmatoarea pozitie din vector mesaj
    if(currentMillis - startMillis >= period){
    startMillis=currentMillis;
    char key = keypad.getKey();
    k=0;
    switch(timepressed[1]){
      case 1:
      message_final.remove(message_final.length()-1);
      message_final+= '1';
      timepressed[1]++;
      break;
      case 2:
      message_final.remove(message_final.length()-1);
      message_final+= '.';
      timepressed[1]++;
      //i--; //nu trec la urmatoarea pozitie ci o editez pozitia din prezent
      break;
      case 3:
      message_final.remove(message_final.length()-1);
      message_final+= ',';
      timepressed[1]++;
      //i--;
      break;
      case 4:
      message_final.remove(message_final.length()-1);
      message_final+= '?';
      timepressed[1]++;
      //i--;
      break;
      case 5:
      message_final.remove(message_final.length()-1);
      message_final+= '1';
      //i--;
      timepressed[1] = 1; //reset timepressed
      break;
    }
    //message_final.concat(message[i]);
    }
    break;
    ////////////////////////////////
    case '2': //50
    //times++;
    i++;
    //int aux=i;
    if(currentMillis - startMillis >= period){
    startMillis=currentMillis;
    char key = keypad.getKey();
    k=0;
    //char key = keypad.getKey();
    switch(timepressed[2]){
      case 1:
      message_final.remove(message_final.length()-1);
      message_final+= '2';
      timepressed[2]++;
      break;
      case 2:
      message_final.remove(message_final.length()-1);
      message_final+= 'A';
      timepressed[2]++;
      //i--; //nu trec la urmatoarea pozitie ci o editez pozitia din prezent
      break;
      case 3:
      message_final.remove(message_final.length()-1);
      message_final+= 'B';
      timepressed[2]++;
      //i--;
      break;
      case 4:
      message_final.remove(message_final.length()-1);
      message_final+= 'C';
      timepressed[2]++;
      //i--;
      break;
      case 5:
      message_final.remove(message_final.length()-1);
      message_final+= '2';
      //i--;
      timepressed[2] = 1; //reset timepressed
      break;
    }
    ////message_final.concat(message[i]);
    }
    break;
    ////////////////////////////////
    case '3': //51
    //times++;
    i++;
    if(currentMillis - startMillis >= period){
    startMillis=currentMillis;
    char key = keypad.getKey();
    k=0;
    switch(timepressed[3]){
      case 1:
      message_final.remove(message_final.length()-1);
      message_final+= '3';
      timepressed[3]++;
      break;
      case 2:
      message_final.remove(message_final.length()-1);
      message_final+= 'D';
      timepressed[3]++;
      //i--; //nu trec la urmatoarea pozitie ci o editez pozitia din prezent
      break;
      case 3:
      message_final.remove(message_final.length()-1);
      message_final+= 'E';
      timepressed[3]++;
      //i--;
      break;
      case 4:
      message_final.remove(message_final.length()-1);
      message_final+= 'F';
      timepressed[3]++;
      //i--;
      break;
      case 5:
      message_final.remove(message_final.length()-1);
      message_final+= '3';
      //i--;
      timepressed[3] = 1; //reset timepressed
      break;
    }
    ////message_final.concat(message[i]);
    }
    break;
    ////////////////////////////////
    case '4': //52
    //times++;
    i++;
    if(currentMillis - startMillis >= period){
    startMillis=currentMillis;
    char key = keypad.getKey();
    k=0;
    switch(timepressed[4]){
      case 1:
      message_final.remove(message_final.length()-1);
      message_final+= '4';
      timepressed[4]++;
      break;
      case 2:
      message_final.remove(message_final.length()-1);
      message_final+= 'G';
      timepressed[4]++;
      //i--; //nu trec la urmatoarea pozitie ci o editez pozitia din prezent
      break;
      case 3:
      message_final.remove(message_final.length()-1);
      message_final+= 'H';
      timepressed[4]++;
      //i--;
      break;
      case 4:
      message_final.remove(message_final.length()-1);
      message_final+= 'I';
      timepressed[4]++;
      //i--;
      break;
      case 5:
      message_final.remove(message_final.length()-1);
      message_final+= '4';
      //i--;
      timepressed[4] = 1; //reset timepressed
      break;
    }
    //message_final.concat(message[i]);
    }
    break;
    ////////////////////////////////
    case '5': //53
    //times++;
    i++;
    if(currentMillis - startMillis >= period){
    startMillis=currentMillis;
    char key = keypad.getKey();
    k=0;
    switch(timepressed[5]){
      case 1:
      message_final.remove(message_final.length()-1);
      message_final+= '5';
      timepressed[5]++;
      break;
      case 2:
      message_final.remove(message_final.length()-1);
      message_final+= 'J';
      timepressed[5]++;
      //i--; //nu trec la urmatoarea pozitie ci o editez pozitia din prezent
      break;
      case 3:
      message_final.remove(message_final.length()-1);
      message_final+= 'K';
      timepressed[5]++;
      //i--;
      break;
      case 4:
      message_final.remove(message_final.length()-1);
      message_final+= 'L';
      timepressed[5]++;
      //i--;
      break;
      case 5:
      message_final.remove(message_final.length()-1);
      message_final+= '5';
      //i--;
      timepressed[5] = 1; //reset timepressed
      break;
    }
    //message_final.concat(message[i]);
    }
    break;
    ////////////////////////////////
    case '6': //54
    //times++;
    i++;
    if(currentMillis - startMillis >= period){
    startMillis=currentMillis;
    char key = keypad.getKey();
    k=0;
    //Serial.println(message_final); //Serial.println(i);
    Serial.println();
    for(int j=0;j<12;j++) Serial.println(timepressed[j]);
    //message_final+= '6';
    switch(timepressed[6]){
      case 1:
      message_final.remove(message_final.length()-1);
      message_final+= '6';
      timepressed[6]++;
      break;
      case 2:
      message_final.remove(message_final.length()-1);
      message_final+= 'M';
      timepressed[6]++;
      //i--; //nu trec la urmatoarea pozitie ci o editez pozitia din prezent
      break;
      case 3:
      message_final.remove(message_final.length()-1);
      message_final+= 'N';
      timepressed[6]++;
      //i--;
      break;
      case 4:
      message_final.remove(message_final.length()-1);
      message_final+= 'O';
      timepressed[6]++;
      //i--;
      break;
      case 5:
      message_final.remove(message_final.length()-1);
      message_final+= '6';
      //i--;
      timepressed[6] = 1; //reset timepressed
      break;
    }
    //message_final.concat(message[i]);
    }
    break;
    ////////////////////////////////
    case '7': //55
    //times++;
    i++;
    if(currentMillis - startMillis >= period){
    startMillis=currentMillis;
    char key = keypad.getKey();
    k=0;
    switch(timepressed[7]){
      case 1:
      message_final.remove(message_final.length()-1);
      message_final+= '7';
      timepressed[7]++;
      break;
      case 2:
      message_final.remove(message_final.length()-1);
      message_final+= 'P';
      timepressed[7]++;
      //i--; //nu trec la urmatoarea pozitie ci o editez pozitia din prezent
      break;
      case 3:
      message_final.remove(message_final.length()-1);
      message_final+= 'Q';
      timepressed[7]++;
      //i--;
      break;
      case 4:
      message_final.remove(message_final.length()-1);
      message_final+='R';
      timepressed[7]++;
      //i--;
      break;
      case 5:
      message_final.remove(message_final.length()-1);
      message_final+='S';
      timepressed[7]++;
      //i--;
      break;
      case 6:
      message_final.remove(message_final.length()-1);
      message_final+= '7';
      //i--;
      timepressed[7] = 1; //reset timepressed
      break;
    }
    //message_final.concat(message[i]);
    }
    break;
    ////////////////////////////////
    case '8': //56
    //times++;
    i++;
    if(currentMillis - startMillis >= period){
    startMillis=currentMillis;
    char key = keypad.getKey();
    k=0;
    switch(timepressed[8]){
      case 1:
      message_final.remove(message_final.length()-1);
      message_final+= '8';
      timepressed[8]++;
      break;
      case 2:
      message_final.remove(message_final.length()-1);
      message_final+= 'T';
      timepressed[8]++;
      //i--; //nu trec la urmatoarea pozitie ci o editez pozitia din prezent
      break;
      case 3:
      message_final.remove(message_final.length()-1);
      message_final+= 'U';
      timepressed[8]++;
      //i--;
      break;
      case 4:
      message_final.remove(message_final.length()-1);
      message_final+= 'V';
      timepressed[8]++;
      //i--;
      break;
      case 5:
      message_final.remove(message_final.length()-1);
      message_final+= '8';
      //i--;
      timepressed[8] = 1; //reset timepressed
      break;
    }
    //message_final.concat(message[i]);
    }
    break;
    ////////////////////////////////
    case '9': //57
    //times++;
    i++;
    if(currentMillis - startMillis >= period){
    startMillis=currentMillis;
    //char key = keypad.getKey();
    k=0;
    
    switch(timepressed[9]){
      case 1:
      message_final.remove(message_final.length()-1);
      message_final+= '9';
      timepressed[9]++;
      break;
      case 2:
      message_final.remove(message_final.length()-1);
      message_final+= 'W';
      timepressed[9]++;
      //i--; //nu trec la urmatoarea pozitie ci o editez pozitia din prezent
      break;
      case 3:
      message_final.remove(message_final.length()-1);
      message_final+= 'X';
      timepressed[9]++;
      //i--;
      break;
      case 4:
      message_final.remove(message_final.length()-1);
      message_final+= 'Y';
      timepressed[9]++;
      //i--;
      break;
      case 5:
      message_final.remove(message_final.length()-1);
      message_final+= 'Z';
      timepressed[9]++;
      //i--;
      break;
      case 6:
      message_final.remove(message_final.length()-1);
      message_final+= '9';
      //i--;
      timepressed[9] = 1; //reset timepressed
      break;
    }
    //message_final.concat(message[i]);
    }
    break;
    ////////////////////////////////
    case '0': //48
    //times++;
    i++;
   if(currentMillis - startMillis >= period){
    startMillis=currentMillis;
    //char key = keypad.getKey();
    k=0;
    
    switch(timepressed[0]){
      case 1:
      message_final.remove(message_final.length()-1);
      message_final+= '0';
      timepressed[0]++;
      break;
      case 2:
      message_final.remove(message_final.length()-1);
      message_final+= ' ';
      timepressed[0]++;
      //i--;
      break;
      case 3:
      message_final.remove(message_final.length()-1);
      message_final+= '0';
      //i--;
      timepressed[0] = 1; //reset timepressed
      break;
    }
    //message_final.concat(message[i]);
    }
    break;
    case '#': //48
    i--;
   if(currentMillis - startMillis >= period){
    startMillis=currentMillis;
    //char key = keypad.getKey();
      message_ready.remove(message_ready.length()-1);
      if(key>=1 && key<=9){ for(int a=0;a<key;a++) timepressed[a] = 1;
      for(int a=key+1;a<16;a++) timepressed[a] = 1;}
      message_final="\0";
      lcd.clear();
      lcd.print(message_ready);
      //for(int a=0;a<16;a++) timepressed[a] = 1;
      //times=0;
    }
    break;



    case 'A': //Sender
    if(sender_mode==false){
    //times++;
    //char key = keypad.getKey();
    k=0;
    lcd.print("Sender mode");
    delay(2000);
    lcd.clear();
    //message_final.concat(message[i]);
    sender_mode=true;
    reciver_mode=false;
    }
    break;





    case 'B': //Reciver
    if(reciver_mode==false){
    //times++;
    //char key = keypad.getKey();
    k=0;
    lcd.setCursor(0,0);
    lcd.print("Reciver mode");
    delay(2000);
    lcd.clear();
    //message_final.concat(message[i]);
    sender_mode=false;
    reciver_mode=true;
    }
    break;




    case 'C': //Send message
    if(sender_mode==true){
    if(message_ready=="")
    {
      Serial.println(message_ready);
      lcd.clear();
      lcd.print("Message empty");
      delay(2000);
      lcd.clear();
    }
    else{
    Sender_funct();
    delay(500);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Message was sent");
    delay(1500);
    lcd.clear();
    lcd.print(message_ready);
    }
    }
    break;






    case 'D': //Display recived message
    if(reciver_mode==true){

      
    if(text_out=="")//NULL
    {
      Serial.println(message_ready);
      lcd.clear();
      lcd.print("No new messages");
      delay(2000);
      lcd.clear();
    }
    else{
    lcd.setCursor(18,3);
    lcd.print(" ");
    lcd.setCursor(0,0);
    lcd.print(text_out);
    delay(1000);
    //wait for keypress *
    text_out="";
    }
    }
    break;



    
    }
}
void start_text()
{
  lcd.setCursor(0,0);
  lcd.print("Enter adress and");
  lcd.setCursor(0,1);
  lcd.print("then the pipe");
  lcd.setCursor(0,2);
  lcd.print("adress!");
  delay(5000);
  lcd.clear();
}
void Sender_funct()
{
  radio.stopListening();
  message_ready.toCharArray(text_sender,sizeof(text_sender));
  radio.write(&text_sender,sizeof(text_sender));
}
void Reciver_funct()
{
  radio.startListening();
  if(radio.available())
  {
    char text[32]="";
    radio.read(&text,sizeof(text));
    text_out = String(text);
  }
  if(text_out!=""){
    lcd.setCursor(18,3);
    lcd.write(0);
    //lcd.setCursor(message_ready.length()%20,message_ready.length()/20);
}
}
