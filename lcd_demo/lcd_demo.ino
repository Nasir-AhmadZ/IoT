#include <LiquidCrystal.h>

const int rs=2 ,en=0 ,d4=4 ,d5=16 , d6=17 , d7=5 ;
  LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  lcd.print("hello world");
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0,1);
  lcd.print(millis()/1000);
}
