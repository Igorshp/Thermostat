#include <LiquidCrystal.h>
#include <String.h>

//define XBEE




//these pins can not be changed 2/3 are special pins
int encoderPin1 = 2;
int encoderPin2 = 3;
int encoderSwitchPin = A6; //push button switch

volatile int lastEncoded = 0;
volatile float set_temp = 10.0;
float temp_step = 0.2;

float max_temp = 35.0;
float min_temp = 10.0;
float current_temp = 22.3;
int status = 0;
int relay_pin = 12;

unsigned long temp_screen_timeout = 0;
int lcd_view = 0;

long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;

//LCD pins

int rs_pin = 8;
int e_pin = 9;
int d4_pin = 4;
int d5_pin = 5;
int d6_pin = 6;
int d7_pin = 7;
int backlight_pin = 13;
LiquidCrystal lcd(rs_pin, 11, e_pin, d4_pin, d5_pin, d6_pin, d7_pin);

void setup() {

  //setup xbee

  //setup LCD
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, LOW);
  pinMode(backlight_pin, OUTPUT);
  digitalWrite(backlight_pin, HIGH);
  lcd.begin(20,4);


  Serial.begin (9600);

  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);

  pinMode(encoderSwitchPin, INPUT);


  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

  digitalWrite(encoderSwitchPin, HIGH); //turn pullup resistor on


  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
  attachInterrupt(0, updateEncoder, CHANGE); 
  attachInterrupt(1, updateEncoder, CHANGE);
  lcd.clear();
   lcd_temp_view();

}

void lcd_main_view(){
  //default view on lcd screen
  if(lcd_view != 0){
   lcd.clear();
   lcd_view = 0;
  }
    lcd.setCursor(0,0);
   lcd.print("Thermostat Control");
   lcd.setCursor(0,1);
   lcd.print("Current: 22.3");
   lcd.print((char)223);
   lcd.setCursor(0,2);
   lcd.print("Target: ");
   lcd.print(set_temp);
   lcd.setCursor(0,3);
   lcd.print("Status: ");
   if(status == 0){
lcd.print("Waiting");
    }else{
      lcd.print("Active");
    }
   
 }
 
 void lcd_temp_view(){
  //when temperature is adjusted, switch to this view for several seconds.
  if(lcd_view != 1){
   lcd.clear();
   lcd_view = 1;
 }
 lcd.setCursor(3,1);
 lcd.print("Target: ");
 lcd.print(set_temp);
 lcd.print("c");
 lcd.setCursor(0,2); 

 float unit = (max_temp - min_temp)/20;
 char buf[12];
 int bars = lround((set_temp-min_temp)/unit);
 for(int start = 0; start < 20; start++){
  if(start < bars){
   lcd.print((char)255);
   }else{
     lcd.print((char)95);
   } 
 }

}

void loop(){ 
  //Do stuff here
  


  if(millis() < temp_screen_timeout){
     lcd_temp_view();    
  }else{
     lcd_main_view(); 
  }

  if(set_temp < current_temp){
    status = 0;
    digitalWrite(relay_pin, HIGH);
  }else{
    status = 1;
    digitalWrite(relay_pin, LOW);
  }



  if(digitalRead(encoderSwitchPin)){
    //button is not being pushed
  }else{
    //button is being pushed
  }
 
  int val = analogRead(0); //hand detector for screen backlight
  delay(100); //just here to slow down the output, and show it will work  even during a delay
}






void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit
  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011){
    if(set_temp < max_temp){
    set_temp += temp_step/4;
    }
  }
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000){
    if(set_temp > min_temp){
    set_temp -= temp_step/4;
    }
  }
  temp_screen_timeout = millis()+1000;
  lastEncoded = encoded; //store this value for next time
}
