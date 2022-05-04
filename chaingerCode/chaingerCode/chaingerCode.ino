#include <JC_Button.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TimerOne.h>

//za zaslon
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#define SCREEN_ADDRESS 0x3C
#include "waveBmp.h"
#include "pitches.h"


Button leftB(2);
Button rightB(4);
Button upB(7);
Button downB(8);


uint16_t pot1 = 0;
uint16_t pot2 = 0;
uint16_t pot3 = 0;
uint16_t pot4 = 0;

const uint8_t digitalMap[7] = {3, 5, 6, 10, 11, 12, 13};
bool playModeOn = false;

uint8_t korak = 1;

unsigned long previousMillis;
uint8_t tempo = 240;
uint16_t preracunanTempo;



uint8_t toni[7] = {1, 1, 1, 1, 1, 1, 1};

//avdio engine spremenljivke:
unsigned char wavetable[256];
uint8_t amplitude = 255;
unsigned int _phase;
uint8_t puls;


//sinus looku tablica:
const char PROGMEM sinetable[128] = {
  0, 0, 0, 0, 1, 1, 1, 2, 2, 3, 4, 5, 5, 6, 7, 9, 10, 11, 12, 14, 15, 17, 18, 20, 21, 23, 25, 27, 29, 31, 33, 35, 37, 40, 42, 44, 47, 49, 52, 54, 57, 59, 62, 65, 67, 70, 73, 76, 79, 82, 85, 88, 90, 93, 97, 100, 103, 106, 109, 112, 115, 118, 121, 124,
  128, 131, 134, 137, 140, 143, 146, 149, 152, 155, 158, 162, 165, 167, 170, 173, 176, 179, 182, 185, 188, 190, 193, 196, 198, 201, 203, 206, 208, 211, 213, 215, 218, 220, 222, 224, 226, 228, 230, 232, 234, 235, 237, 238, 240, 241, 243, 244, 245, 246, 248, 249, 250, 250, 251, 252, 253, 253, 254, 254, 254, 255, 255, 255,
};
  void pulseWave() {
  for (uint8_t i = 0; i < puls; i++)
    wavetable[i] = 255;
  for (uint8_t i = puls; i < 255; i++)
    wavetable[i] = 1;
}
void triangleWave() {
  for (uint8_t i = 0; i < 128; i++)
  {
    wavetable[i] = i * 2;
  }
  int value = 255;
  for (uint8_t i = 128; i < 256; i++)
  {
    wavetable[i] = value;
    value--;
  }

}
void noiseWave(){
  for(uint8_t i=0;i<256;i++)
  {wavetable[i]=random(1,255);}
}
void sawToothWave() {
  for (uint8_t i = 0; i < 256; i++)
  {
    wavetable[i] = i;
  }
}
void squareWave() {
  for (uint8_t i = 0; i < 128; i++)
  { wavetable[i] = 255;
  }
  for (uint8_t i = 128; i < 255; i++) {
    wavetable[i] = 1;
  }

}




void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  delay(1000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);


  //sound setup:
  initAudio();
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  Timer1.initialize(30);         // initialize timer1, and set a 1/2 second period
  Timer1.pwm(9, 512);                // setup pwm on pin 9, 50% duty cycle

  Timer1.attachInterrupt(updateAudioEngine);  // attaches callback() as a timer overflow interrupt

  leftB.begin();
  rightB.begin();
  upB.begin();
  downB.begin();


  //set pinsmode for LED's
  for (uint8_t i = 0; i < 7; i++) {
    pinMode(digitalMap[i], OUTPUT);
  }
  //set pinMode for keypad
  pinMode(2, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);

  // put your setup code here, to run once:
  
}

void loop() {
  display.setCursor(0, 0);
  display.println("ton");
  display.println(toni[korak]);
  //display.println("tempo: ");
  display.println(tempo);
  display.display();





  readButtons();
  readPots();

  tempo = map(pot2, 0, 1023, 60, 240);
  


  //writeMode

  if (playModeOn == false) {
    lightUpLED(korak, 1);
    lightOffOther(korak);
    if (downB.wasReleased()) {
      writeValue(korak);
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
    //frequency = pot1;
    puls = map(pot4, 0, 1026, 0, 255);
    amplitude = map(pot2, 0, 1026, 0, 255);
    selectWave();
    


    //avdio del:
    //tone(3, dolociFrekvenco(korak));
  }
  else {

    //smo v play modu
    //izracunaj tempo
    preracunanTempo = map(tempo, 60, 240, 1000, 250);
    unsigned long currentMillis = millis();


    if (currentMillis - previousMillis >= preracunanTempo)
    {

      if (korak < 7) {
        korak++;
      }
      else {
        korak = 1;
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
    //tone(3, dolociFrekvenco(korak));
    //avdio del loop:
    //frequency = pot1;
    puls = map(pot4, 0, 1026, 0, 255);
    amplitude = map(pot2, 0, 1026, 0, 255);
    selectWave();

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
  for (uint8_t i = 0; i < 7; i++) {
    lightUpLED(i, 1);
    delay(100);
    lightUpLED(i, 0);
  }
}

void writeValue(uint8_t kanal) {
  toni[kanal] = map(pot1, 0, 1023, 1, 12);
}


uint8_t korakNazaj(uint8_t _korak) {
  if (_korak > 1)
    return _korak - 1;
  else
    return 7;
}
uint8_t korakNaprej(uint8_t _korak) {
  if (_korak < 7)
    return _korak + 1;
  else
    return 1;
}
void lightOffOther(uint8_t _korak)
{
  for (uint8_t i = 0; i < 7; i++) {
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
void sineWave() {
  for (uint8_t i = 0; i < 128; i++) {
    wavetable[i] = pgm_read_byte_near(sinetable + i);
  }
  wavetable[128] = 255;
  for (uint8_t i = 129; i < 256; i++) {
    wavetable[i] = wavetable[256 - i];
  }
}

void updateAudioEngine() {
  //_phase += frequency;
  _phase +=dolociFrekvenco(korak);
  uint16_t output = ((wavetable[_phase >> 8] * amplitude) >> 8);
  Timer1.pwm(9, output);
}
void initAudio() {
  sineWave();
  //sawToothWave();
  //frequency = 440;
}
void selectWave() {
  uint8_t izbira = map(pot3, 0, 1026, 0, 6);
  switch (izbira) {
    case 0: squareWave();
      break;
    case 1: sawToothWave();
      break;
    case 2: pulseWave();
      break;
    case 3: triangleWave();
      break;
    case 4: sineWave();
      break;
    case 5: noiseWave();

  }
}
