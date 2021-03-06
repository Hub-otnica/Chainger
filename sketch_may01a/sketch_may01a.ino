#include <TimerOne.h>

//sintetizator zvoka za Syntete razvojno okolje. Sintetizator ima 5- wave formov. med katerimi izbiramo s potenciometrom A2

//vrednosti za sinus ne bomo računali ampak prebrali iz tabelce.
const char PROGMEM sinetable[128] = {
  0, 0, 0, 0, 1, 1, 1, 2, 2, 3, 4, 5, 5, 6, 7, 9, 10, 11, 12, 14, 15, 17, 18, 20, 21, 23, 25, 27, 29, 31, 33, 35, 37, 40, 42, 44, 47, 49, 52, 54, 57, 59, 62, 65, 67, 70, 73, 76, 79, 82, 85, 88, 90, 93, 97, 100, 103, 106, 109, 112, 115, 118, 121, 124,
  128, 131, 134, 137, 140, 143, 146, 149, 152, 155, 158, 162, 165, 167, 170, 173, 176, 179, 182, 185, 188, 190, 193, 196, 198, 201, 203, 206, 208, 211, 213, 215, 218, 220, 222, 224, 226, 228, 230, 232, 234, 235, 237, 238, 240, 241, 243, 244, 245, 246, 248, 249, 250, 250, 251, 252, 253, 253, 254, 254, 254, 255, 255, 255,
};

//deklariramo spremenljivke:
unsigned char wavetable[256];
uint8_t amplitude = 255;
unsigned int frequency, _phase;
int pot2;
int pot3;
int pot4;
uint8_t pulz;
uint8_t izbira;


void setup() {

  //avdio: uporabljam Timer1 z dedicated knjižnico. Zvok dobimo na pinu 10
  initAudio();
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  Timer1.initialize(30);         // initialize timer1, and set a 1/2 second period
  Timer1.pwm(9, 512);                // setup pwm on pin 9, 50% duty cycle

  Timer1.attachInterrupt(updateAudioEngine);  // attaches callback() as a timer overflow interrupt
}



void loop() {
  // najprej preberemo vrednosti potenciometrov(ali CV vhodov)
  frequency = analogRead(A0);
  pot2 = analogRead(A1);
  pot3 = analogRead(A2);
  pot4 = analogRead(A3);
  pulz = map(pot4, 0, 1026, 0, 255);
  amplitude = map(pot2, 0, 1026, 0, 255);
  selectWave();
}

void updateAudioEngine() {
  _phase += frequency;
  uint16_t output = ((wavetable[_phase >> 8] * amplitude) >> 8);
  Timer1.pwm(9, output);
}
void sineWave() {
  for (int i = 0; i < 128; i++) {
    wavetable[i] = pgm_read_byte_near(sinetable + i);
  }
  wavetable[128] = 255;
  for (int i = 129; i < 256; i++) {
    wavetable[i] = wavetable[256 - i];
  }
}

void sawToothWave() {
  for (int i = 0; i < 256; i++)
  {
    wavetable[i] = i;
  }
}
void squareWave() {
  for (int i = 0; i < 128; i++)
  { wavetable[i] = 255;
  }
  for (int i = 128; i < 255; i++) {
    wavetable[i] = 1;
  }

}
void pulseWave() {
  for (int i = 0; i < pulz; i++)
    wavetable[i] = 255;
  for (int i = pulz; i < 255; i++)
    wavetable[i] = 1;
}
void triangleWave() {
  for (int i = 0; i < 128; i++)
  {
    wavetable[i] = i * 2;
  }
  int value = 255;
  for (int i = 128; i < 256; i++)
  {
    wavetable[i] = value;
    value--;
  }

}
void noiseWave(){
  for(int i=0;i<256;i++)
  {wavetable[i]=random(1,255);}
}
void initAudio() {
  //squareWave();
  //sawToothWave();
  frequency = 440;
}
void selectWave() {
  izbira = map(pot3, 0, 1026, 0, 6);
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
