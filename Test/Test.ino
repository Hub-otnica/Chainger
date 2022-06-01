//To je testna koda za Sintete, ki vključuje sintetizator z različnimi oblikami (saw,square,sine,pulse,triangle in noise) ki jih izbiramo s potenciometrom 3.
//Ton sprožimo s pritiskom na gumb D2 (levo), kar ustvari avdio signal na pinu 9 (uprabljam timer1).
//S potenciometrom 2 spreminjamo decay time.
//S potenciometrom 1 spreminjamo frekvenco.
//Potenciometer 4 pa spreminja pulse width pri pulz signalu.
//obliko envelope-a izberemo v funkciji decayEnvelope(), in sicer tako da amplitudi pripišemo bodisi vrednost EnvO,Env1 in Env2. Te so zaenkrat arrayi tipa uin8_t,v bodoče pa bi jih dal v progmem.


#include <TimerOne.h>
#include <JC_Button.h>
#include <math.h> 
#include "envelope.h"
const char PROGMEM sinetable[128] = {
  0, 0, 0, 0, 1, 1, 1, 2, 2, 3, 4, 5, 5, 6, 7, 9, 10, 11, 12, 14, 15, 17, 18, 20, 21, 23, 25, 27, 29, 31, 33, 35, 37, 40, 42, 44, 47, 49, 52, 54, 57, 59, 62, 65, 67, 70, 73, 76, 79, 82, 85, 88, 90, 93, 97, 100, 103, 106, 109, 112, 115, 118, 121, 124,
  128, 131, 134, 137, 140, 143, 146, 149, 152, 155, 158, 162, 165, 167, 170, 173, 176, 179, 182, 185, 188, 190, 193, 196, 198, 201, 203, 206, 208, 211, 213, 215, 218, 220, 222, 224, 226, 228, 230, 232, 234, 235, 237, 238, 240, 241, 243, 244, 245, 246, 248, 249, 250, 250, 251, 252, 253, 253, 254, 254, 254, 255, 255, 255,
};
uint8_t Env0[]=
{
  255,  //0
  254,  //1
  254,  //2
  254,  //3
  253,  //4
  253,  //5
  253,  //6
  252,  //7
  252,  //8
  251,  //9
  251,  //10
  250,  //11
  249,  //12
  248,  //13
  248,  //14
  247,  //15
  246,  //16
  245,  //17
  241,  //18
  237,  //19
  232,  //20
  228,  //21
  223,  //22
  219,  //23
  215,  //24
  210,  //25
  205,  //26
  200,  //27
  195,  //28
  189,  //29
  184,  //30
  179,  //31
  173,  //32
  168,  //33
  163,  //34
  157,  //35
  151,  //36
  145,  //37
  139,  //38
  133,  //39
  127,  //40
  121,  //41
  115,  //42
  110,  //43
  105,  //44
  101,  //45
  96, //46
  91, //47
  86, //48
  82, //49
  77, //50
  72, //51
  69, //52
  65, //53
  62, //54
  58, //55
  54, //56
  51, //57
  47, //58
  44, //59
  41, //60
  38, //61
  36, //62
  33, //63
  31, //64
  28, //65
  26, //66
  23, //67
  20, //68
  19, //69
  18, //70
  17, //71
  15, //72
  14, //73
  13, //74
  12, //75
  10, //76
  9,  //77
  9,  //78
  9,  //79
  8,  //80
  8,  //81
  7,  //82
  7,  //83
  6,  //84
  6,  //85
  6,  //86
  6,  //87
  5,  //88
  5,  //89
  5,  //90
  5,  //91
  5,  //92
  5,  //93
  5,  //94
  5,  //95
  5,  //96
  4,  //97
  4,  //98
  4,  //99
  4,  //100
  4,  //101
  4,  //102
  3,  //103
  3,  //104
  3,  //105
  2,  //106
  2,  //107
  2,  //108
  1,  //109
  1,  //110
  1,  //111
  1,  //112
  1,  //113
  0,  //114
  0,  //115
  0,  //116
  0,  //117
  0,  //118
  0,  //119
  0,  //120
  0,  //121
  0,  //122
  0,  //123
  0,  //124
  0,  //125
  0,  //126
  0,  //127
  0,  //127
  0,  //127
};
uint8_t Env2[]=
{
  255,  //0
  250,  //1
  246,  //2
  242,  //3
  238,  //4
  233,  //5
  229,  //6
  225,  //7
  221,  //8
  217,  //9
  213,  //10
  209,  //11
  206,  //12
  202,  //13
  199,  //14
  195,  //15
  191,  //16
  188,  //17
  183,  //18
  179,  //19
  175,  //20
  170,  //21
  166,  //22
  161,  //23
  157,  //24
  153,  //25
  148,  //26
  144,  //27
  139,  //28
  134,  //29
  130,  //30
  125,  //31
  121,  //32
  116,  //33
  112,  //34
  109,  //35
  105,  //36
  102,  //37
  99, //38
  96, //39
  93, //40
  90, //41
  87, //42
  83, //43
  80, //44
  77, //45
  74, //46
  71, //47
  68, //48
  65, //49
  61, //50
  58, //51
  56, //52
  54, //53
  51, //54
  49, //55
  46, //56
  44, //57
  42, //58
  39, //59
  37, //60
  34, //61
  31, //62
  28, //63
  25, //64
  22, //65
  19, //66
  16, //67
  13, //68
  12, //69
  12, //70
  11, //71
  11, //72
  10, //73
  10, //74
  10, //75
  9,  //76
  9,  //77
  8,  //78
  8,  //79
  8,  //80
  7,  //81
  7,  //82
  6,  //83
  6,  //84
  6,  //85
  6,  //86
  6,  //87
  5,  //88
  5,  //89
  5,  //90
  5,  //91
  5,  //92
  5,  //93
  5,  //94
  5,  //95
  5,  //96
  4,  //97
  4,  //98
  4,  //99
  4,  //100
  4,  //101
  4,  //102
  3,  //103
  3,  //104
  3,  //105
  2,  //106
  2,  //107
  2,  //108
  1,  //109
  1,  //110
  1,  //111
  1,  //112
  1,  //113
  0,  //114
  0,  //115
  0,  //116
  0,  //117
  0,  //118
  0,  //119
  0,  //120
  0,  //121
  0,  //122
  0,  //123
  0,  //124
  0,  //125
  0,  //126
  0,  //127
  0,  //127
  0,  //127
  0,  //127

};
uint8_t Env1[]=
{
  255,  //0
  242,  //1
  229,  //2
  216,  //3
  204,  //4
  191,  //5
  178,  //6
  165,  //7
  153,  //8
  142,  //9
  134,  //10
  125,  //11
  117,  //12
  108,  //13
  100,  //14
  91, //15
  83, //16
  74, //17
  71, //18
  68, //19
  65, //20
  62, //21
  59, //22
  56, //23
  53, //24
  50, //25
  48, //26
  46, //27
  44, //28
  42, //29
  40, //30
  38, //31
  36, //32
  34, //33
  32, //34
  31, //35
  30, //36
  29, //37
  28, //38
  28, //39
  27, //40
  26, //41
  25, //42
  25, //43
  24, //44
  24, //45
  23, //46
  23, //47
  22, //48
  22, //49
  21, //50
  21, //51
  20, //52
  20, //53
  19, //54
  19, //55
  19, //56
  18, //57
  18, //58
  17, //59
  17, //60
  16, //61
  16, //62
  15, //63
  15, //64
  14, //65
  14, //66
  13, //67
  13, //68
  12, //69
  12, //70
  11, //71
  11, //72
  10, //73
  10, //74
  10, //75
  9,  //76
  9,  //77
  8,  //78
  8,  //79
  8,  //80
  7,  //81
  7,  //82
  6,  //83
  6,  //84
  6,  //85
  6,  //86
  6,  //87
  5,  //88
  5,  //89
  5,  //90
  5,  //91
  5,  //92
  5,  //93
  5,  //94
  5,  //95
  5,  //96
  4,  //97
  4,  //98
  4,  //99
  4,  //100
  4,  //101
  4,  //102
  3,  //103
  3,  //104
  3,  //105
  2,  //106
  2,  //107
  2,  //108
  1,  //109
  1,  //110
  1,  //111
  1,  //112
  1,  //113
  0,  //114
  0,  //115
  0,  //116
  0,  //117
  0,  //118
  0,  //119
  0,  //120
  0,  //121
  0,  //122
  0,  //123
  0,  //124
  0,  //125
  0,  //126
  0,  //127
  0,  //127
  0,  //127
  0,  //127
};
Button knof(2);
bool tonSpuscen = false;
bool tonOdzvenel = false;
bool pritisnen;
uint16_t decayTime = 1;


int k =1;

unsigned char wavetable[256];
uint8_t amplitude = 0;
unsigned int frequency, _phase;
int pot2;
int pot3;
int pot4;
uint8_t pulz;
uint8_t izbira;
unsigned long current;
unsigned long previous;
//unsigned long decayTime;
//unsigned long attackTime;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  knof.begin();
  pinMode(2, INPUT_PULLUP);
  //avdio
  initAudio();
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  Timer1.initialize(30);         // initialize timer1, and set a 1/2 second period
  Timer1.pwm(9, 512);                // setup pwm on pin 9, 50% duty cycle

  Timer1.attachInterrupt(updateAudioEngine);  // attaches callback() as a timer overflow interrupt
}

void loop() {
  // put your main code here, to run repeatedly:
  pritisnen=knof.read();
  frequency = analogRead(A0);
  pot2 = analogRead(A1);
  pot3 = analogRead(A2);
  pot4 = analogRead(A3);
  pulz = map(pot4, 0, 1026, 0, 255);
  //amplitude = map(pot2, 0, 1026, 0, 255);
  decayTime=map(pot2, 0,1026,1,100);

if (knof.wasReleased()) {
    tonSpuscen = true;
    tonOdzvenel = false;
    
    Serial.println("spustil_gumb");
  }
  if (tonSpuscen && !tonOdzvenel) {
    
    decayEnvelope();
  }
  
  selectWave();
}


void updateAudioEngine() {
  _phase += frequency;
  uint16_t output = ((wavetable[_phase >> 8] * amplitude) >> 10);
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


void decayEnvelope() {
  
  current = millis();
  if (current - previous >= decayTime) {
    amplitude=Env1[k];
    //Serial.print("millis : " );
    //Serial.println(current);
    Serial.print("amp: ");
    Serial.println(amplitude);
    Serial.println(k);
    previous = current;
    k++;
    if (amplitude <= 1) {
      tonOdzvenel = true;
      k=1;
    }
  }
}
