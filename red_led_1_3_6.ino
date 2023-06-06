//************************************************************
// nodeId = 624315197
// 
//************************************************************

#include <FastLED.h>
#include "painlessMesh.h"


#define LED_PIN     5
#define NUM_LEDS    32
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define   MESH_PREFIX     "kennet"
#define   MESH_PASSWORD   "kennet123"
#define   MESH_PORT       5555

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

uint32_t trimi;

bool power = 1;
char mode = 8;
char brightness = 20;

String str1 = "";

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

const unsigned long delayDuration = 50; // Тривалість затримки в мілісекундах
unsigned long previousTime = 0; // Попередній час
int step = 0;

uint8_t gHue = 0;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void receivedCallback( uint32_t from, String &msg ) {
  str1 = msg.c_str();
  String str2 = "power";
  String str3 = "next_eff";
  String str4 = "brightness+10";
  String str5 = "brightness-10";

  if (str1.equals(str4)) {
    if (brightness+10 <= 255){
      brightness +10;
    } else { 
      brightness =255;
    }
  }
  else if (str1.equals(str5)) {
    if (brightness-10 >= 0){
      brightness -10;
    } else { 
      brightness =0;
    }
  }

  else if (str1.equals(str2)) {
   if (power == 1) {
     power = 0;
    } else {
     power = 1;
   }
  }

  else if (str1.equals(str3)) {
    mode++;
    if (mode > 8){
      mode = 0;
    }
  }
}


void red(){
    for(int i = 0; i < NUM_LEDS; i++) {   
    leds[i] = CRGB(255, 0, 0);
  }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);

  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( 20 );
    
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);

  //currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop(){

  EVERY_N_MILLISECONDS( 30 ) { gHue++; }

  mesh.update();

  if (power == 1) {
    FastLED.setBrightness( brightness );
    switch (mode){
      case 0:{        rainbow();     break;}

      case 1:{        rainbowWithGlitter();  break;}

      case 2:{        bpm();  break;}

      case 3:{        red();             /*currentBlending = LINEARBLEND;*/  break;}

      case 4:{        juggle();              break;}

      case 5:{        sinelon();       /*currentBlending = NOBLEND;*/  break;}

      case 6:{        confetti();       /*currentBlending = LINEARBLEND;*/  break;}

      case 7:{        pride();           break;}

      case 8:{        rasta();           /*currentBlending = LINEARBLEND;*/  break;} 
    }
  } else { FastLED.setBrightness( 0 ); }
    
    
  FastLED.show();

}


void rasta() {
  unsigned long currentTime = millis(); // Поточний час
  
  if (currentTime - previousTime >= delayDuration) {
    previousTime = currentTime;

    if (step >= 0 && step <= 16) {
      int index = 32 - step;
      leds[index] = CRGB::Green;
    } 
    else if (step > 16 && step <= 32) {
      int index = -17 + step;
      leds[index] = CRGB::Green;
    }

    else if (step > 32 && step <= 48) {
      int index = 64 - step;
      leds[index] = CRGB::Yellow;
    } 
    else if (step > 48 && step <= 64) {
      int index = -49 + step;
      leds[index] = CRGB::Yellow;
    }

   else if (step >= 64 && step <= 80) {
     int index = 96 - step;
     leds[index] = CRGB::Red;
   } 
   else if (step > 80 && step <= 96) {
     int index = -81 + step;
     leds[index] = CRGB::Red;
   }
  
    FastLED.show();
    step++;
    
    if (step > 96) {
      step = 0; // Повторення циклу
    }
  }
}


void confetti() {
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
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

void sinelon(){
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void rainbow() {
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() {
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void bpm(){
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void pride() {
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    CRGB newcolor = CHSV( hue8, sat8, bri8);
    
    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS-1) - pixelnumber;
    
    nblend( leds[pixelnumber], newcolor, 64);
  }
}