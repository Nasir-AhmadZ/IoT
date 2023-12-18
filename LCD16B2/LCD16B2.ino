#include <LiquidCrystal.h>
#include <Keypad.h>
#include <ESP32Servo.h>

Servo myServo;   

#define BUZ 40
#define ROW_NUM     4 // four rows
#define COLUMN_NUM  4 // four columns


char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '4','7', 'A'},
  {'2', '5', '8', '0'},
  {'3', '6', '9', 'B'},
  {'F', 'E', 'D', 'C'}
};

byte pin_rows[ROW_NUM]      = {36, 35, 0, 45}; 
byte pin_column[COLUMN_NUM] = {48, 47, 21, 20};  

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

LiquidCrystal lcd(4,5,6,7,15,16);

const int len_key = 4;
char master_key[len_key] = {'1','2','3','4'};
char attempt_key[len_key];
int z=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(BUZ,OUTPUT);
  myServo.attach(3);
  myServo.write(10);
  Serial.begin(115200);
  lcd.begin(16,1);
  lcd.print("  Insert Password");
  Serial.println(" Insert Password");
}

void loop() {
  char key = keypad.getKey();
  if (key){
    switch(key){
      case 'A':
        z=0;
        break;
      /*case '#':
        delay(100); // added debounce
        checkKEY();
        break;*/
      default:
      Serial.println(key);
         attempt_key[z]=key;
         z++;
         if(z == len_key){
          checkKEY();
         }
      }
  }
}

void checkKEY()
{
   int correct=0;
   int i;
   for (i=0; i<len_key; i++) {
    if (attempt_key[i]==master_key[i]) {
      correct++;
      }
    }
   if (correct==len_key && z==len_key){
    lcd.clear();
    lcd.print("   Correct Key");
    Serial.println("Correct Key");
    digitalWrite(BUZ,LOW);
    delay(1000);
    z=0;
    lcd.clear();
    lcd.print(" Insert Password");
    Serial.println("Insert Password");
   }
   else
   {
    lcd.clear();
    lcd.print(" Incorrect Key");
    Serial.println("Incorrect Key");
    myServo.write(180); 
    digitalWrite(BUZ,HIGH);
    delay(3000);
    z=0;
    myServo.write(10);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Insert Password");
    Serial.println("Insert Password");
   }
   for (int zz=0; zz<len_key; zz++) {
    attempt_key[zz]=0;
   }
}
