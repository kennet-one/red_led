//************************************************************
// nodeId = 624315197
//
//************************************************************

#include <FastLED.h>
#include "painlessMesh.h"


#define LED_PIN     5
#define NUM_LEDS    16
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define   MESH_PREFIX     "kennet"
#define   MESH_PASSWORD   "kennet123"
#define   MESH_PORT       5555

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

uint32_t trimi;

bool power = 1;
int mode = 3;

String str1 = "";

CRGBPalette16 currentPalette;
TBlendType    currentBlending;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void receivedCallback( uint32_t from, String &msg ) {
  str1 = msg.c_str();
  String str2 = "power";
  String str3 = "next_eff";

  if (str1.equals(str2)) {
   if (power == 1) {
     power = 0;
    } else {
     power = 1;
   }
  }

  if (str1.equals(str3)) {
    mode++;
    if (mode > 8){
      mode = 0;
    }
  }
}

void FillLEDsFromPaletteColors( uint8_t colorIndex) {
  uint8_t brightness = 255;
    
  for( int i = 0; i < NUM_LEDS; ++i) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}

void sinelon(){
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( 0, 255, 192);
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( 0 + random8(64), 200, 255);
}
// Ця функція налаштовує палітру фіолетових і зелених смуг.
void red(){
    CRGB purple = CRGB(255, 0, 0); //CHSV( HUE_REED, 255, 255);
    
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   purple,  purple,  purple,  purple,
                                   purple, purple, purple,  purple,
                                   purple,  purple,  purple,  purple,
                                   purple, purple, purple,  purple );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);

  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( 20 );
    
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop(){

  mesh.update();

  if (power == 1) {
    FastLED.setBrightness( 255 );
    switch (mode){
      case 0:{        currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND;  break;}

      case 1:{        currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  break;}

      case 2:{        currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND;  break;}

      case 3:{        red();             currentBlending = LINEARBLEND;  break;}

      case 4:{        juggle();              /*currentBlending = LINEARBLEND;*/  break;}

      case 5:{        sinelon();       /*currentBlending = NOBLEND;*/  break;}

      case 6:{        confetti();       /*currentBlending = LINEARBLEND;*/  break;}

      case 7:{        currentPalette = CloudColors_p;           currentBlending = LINEARBLEND;  break;}

      case 8:{        currentPalette = PartyColors_p;           currentBlending = LINEARBLEND;  break;} 
    }
  } else { FastLED.setBrightness( 0 ); }
    
  static uint8_t startIndex = 0;
    if(millis() - trimi >= 10 ){
    trimi = millis();
    startIndex = startIndex + 1; /* motion speed */
  }
    
  FillLEDsFromPaletteColors( startIndex);
    
  FastLED.show();
}
// Додаткові примітки щодо компактних палітр FastLED:

// Зазвичай, у графіці комп'ютера палітра (або "таблиця кольорів")
// має 256 записів, кожен з яких містить певний 24-бітний RGB-колір. Ви можете
// індексувати палітру кольорів за допомогою простого 8-бітного (одного байта) значення.
// Палітра з 256 записами займає 768 байтів RAM, що на Arduino
// може бути "занадто багато" байтів.

// FastLED, однак, пропонує традиційні палітри з 256 елементами для налаштувань,
// які можуть дозволити витрати 768 байтів RAM.

// Однак FastLED також пропонує компактну альтернативу. FastLED надає
// палітри, які містять 16 різних записів, але до них можна отримати ДЛЯ ВСІХ
// 256 записів; це досягається шляхом інтерполяції
// між 16 явними записами для створення п'ятнадцяти проміжних записів палітри
// між кожною парою.

// Наприклад, якщо ви встановите перші два явні записи компактної палітри
// на Зелений (0,255,0) і Синій (0,0,255), а потім отримаєте
// перші шістнадцять записів з віртуальної палітри (з 256), ви отримаєте
// Зелений, за яким слідує плавний градієнт від зеленого до синього, а потім Синій.
