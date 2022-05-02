#include <JC_Button.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//za zaslon
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#define SCREEN_ADDRESS 0x3C
#include "pitches.h"

//GitHub black magic situacija

Button leftB(2);
Button rightB(4);
Button upB(7);
Button downB(8);


uint16_t pot1 = 0;
uint16_t pot2 = 0;
uint16_t pot3 = 0;
uint16_t pot4 = 0;

const uint8_t digitalMap[8] = {3, 5, 6, 9, 10, 11, 12, 13};
bool playModeOn = false;

uint8_t korak = 2;
uint8_t stopinja = 2;
unsigned long previousMillis;
int tempo = 240;
int preracunanTempo;

int frekvenca;

uint8_t toni[8] = {1, 1, 1, 1, 1, 1, 1, 1};

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  delay(1000);
  display.clearDisplay();
  display.setTextSize(2);   
display.setTextColor(WHITE);
display.setCursor(0,10);  




  leftB.begin();
  rightB.begin();
  upB.begin();
  downB.begin();


  //set pinsmode for LED's
  for (int i = 0; i < 8; i++) {
    pinMode(digitalMap[i], OUTPUT);
  }
  //set pinMode for keypad
  pinMode(2, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);

  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  display.setCursor(0,0);  
  display.println("ton");
  display.println(toni[korak]);
  display.println("tempo: ");
    display.println(tempo);
display.display();


    
    

  readButtons();
  readPots();

tempo=map(pot2, 0,1023,60,240);


  //writeMode

  if (playModeOn == false) {
    lightUpLED(korak, 1);
    lightOffOther(korak);
    if (downB.wasReleased()) {
      writeValue(korak);
    }
    for (int i = 0; i < 8; i++) {
      Serial.print("ton ");
      Serial.print(i);
      Serial.println(toni[i]);
    }
    if (leftB.wasReleased()) {
      korak = korakNazaj(korak);
    }
    if (rightB.wasReleased()) {
      korak = korakNaprej(korak);
    }
    if (upB.wasReleased()) {
      playModeOn = true;
      lightOffOther(16);
    }
    tone(3, dolociFrekvenco(korak));
  }
  else {

    //smo v play modu
    //izracunaj tempo
    preracunanTempo = map(tempo, 60, 240, 1000, 250);
    unsigned long currentMillis = millis();
    

    if (currentMillis - previousMillis >= preracunanTempo)
    {

      if (korak < 8) {
        korak++;
      }
      else {
        korak = 2;
      }
      previousMillis = currentMillis;
      Serial.println("korak");
      Serial.println(korak);
      Serial.println("tempo");
      Serial.println(preracunanTempo);
    }
    //play mode  tukaj prozimo avdio glede na step na katerem smo. step pa ne menjamo s klikanjem ampak avtomatsko.
    lightUpLED(korak, 1);
    lightOffOther(korak);


    if (upB.wasReleased()) {
      playModeOn = false;
      lightOffOther(16);
    }
    tone(3, dolociFrekvenco(korak));

  }

display.clearDisplay();
}

void readPots() {
  pot1 = analogRead(A0);
  pot2 = analogRead(A1);
  pot3 = analogRead(A2);
  pot4 = analogRead(A3);

}
void readButtons() {
  leftB.read();
  rightB.read();
  upB.read();
  downB.read();
}

void lightUpLED(uint8_t number, bool onoff) {
  digitalWrite(digitalMap[number - 1], onoff);
}
void lightShow() {
  for (int i = 0; i < 8; i++) {
    lightUpLED(i, 1);
    delay(100);
    lightUpLED(i, 0);
  }
}

void writeValue(uint8_t kanal) {
  toni[kanal] = map(pot1, 0, 1023, 1, 12);
}


uint8_t korakNazaj(uint8_t _korak) {
  if (_korak > 2)
    return _korak - 1;
  else
    return 8;
}
uint8_t korakNaprej(uint8_t _korak) {
  if (_korak < 8)
    return _korak + 1;
  else
    return 2;
}
void lightOffOther(uint8_t _korak)
{
  for (int i = 0; i < 8; i++) {
    if (i != _korak)
    {
      lightUpLED(i, 0);
    }
  }
}
int dolociFrekvenco(uint8_t _korak) {
  switch (toni[_korak]) {
    case 1:
      return NOTE_C3;
      break;
    case 2:
      return NOTE_CS3;
      break;
    case 3:
      return NOTE_D3;
      break;
    case 4:
      return NOTE_DS3;
      break;
    case 5:
      return NOTE_E3;
      break;
    case 6:
      return NOTE_F3;
      break;
    case 7:
      return NOTE_FS3;
      break;
    case 8:
      return NOTE_G3;
      break;
    case 9:
      return NOTE_GS3;
      break;
    case 10:
      return NOTE_A3;
      break;
    case 11:
      return NOTE_AS3;
      break;
    case 12:
      return NOTE_B3;
      break;
  }
}
