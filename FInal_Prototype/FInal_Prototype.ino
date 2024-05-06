#include <LiquidCrystal.h>
#include <Keypad.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <ESP32Servo.h>
#include "homepage.h"

const char* ssid = "VM8828443";
const char* password = "Pf2cskc7memk";

Servo myServo;   

WebServer server(80);
const int led = 13;

#define BUZ 27
#define ROW_NUM     4 // four rows
#define COLUMN_NUM  4 // four columns


char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '4','7', 'A'},
  {'2', '5', '8', '0'},
  {'3', '6', '9', 'B'},
  {'F', 'E', 'D', 'C'}
};

byte pin_rows[ROW_NUM]      = {15,2,0,21}; 
byte pin_column[COLUMN_NUM] = {22, 23, 14, 12};  
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

const int rs = 4, en = 16, d4 = 17, d5 = 5, d6 = 18, d7 = 19;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int len_key = 4; 
char master_key[len_key] = {'1','2','3','4'};
char attempt_key[len_key];
int z=0;

void handleRoot() {
  String message = homePagePart1 ;


  server.send(200, "text/html", message);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void handleKeyPress(){//used to turn on and off the servo motor
  String receivedData="You Pressed: "+server.arg("button");
  Serial.println(receivedData);
  if(server.arg("button")=="ON")
  {
    myServo.write(180);
  }
  else if(server.arg("button")=="OFF")
  {
    myServo.write(10);
  }
  server.send(200);
}

void setup() {
  //Server code
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  myServo.attach(13);
  myServo.write(10);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/sentKeyPressToWebServer",handleKeyPress);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  // LCD and Keypad
  pinMode(BUZ,OUTPUT);
  myServo.attach(13);
  myServo.write(10);
  Serial.begin(115200);
  lcd.begin(16,1);
  lcd.print("Insert Password");
  Serial.println("Insert Password");
}

void loop() {
  server.handleClient();//server code
  delay(2);//allow the cpu to switch to other tasks

  char key = keypad.getKey();
  
  if (key){
    
    switch(key){
      case 'A':
        z=0;
        lcd.clear();
        lcd.print("Reset Insert Pin");
        break;
      /*case '#':
        delay(100); // added debounce
        checkKEY();
        break;*/
      default:
      Serial.println(key);
      lcd.clear();
      lcd.print(key);
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
    lcd.print("Correct Key");
    Serial.println("Correct Key");
    digitalWrite(BUZ,LOW);
    delay(1000);
    z=0;
    lcd.clear();
    lcd.print("Insert Password");
    Serial.println("Insert Password");
   }
   else
   {
    lcd.clear();
    lcd.print("Incorrect Key");
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
