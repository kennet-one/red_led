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


painlessMesh  mesh;

uint32_t trimi;

bool power = 1;
int mode = 2;
int brightness = 20;

String str1 = "";


int led_speed = 50; // Тривалість затримки в мілісекундах

unsigned long previousTime = 0; // Попередній час
int step = 0;

uint8_t gHue = 0;

#define COOLING  55
#define SPARKING 120

bool gReverseDirection = false;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void receivedCallback( uint32_t from, String &msg ) {
  str1 = msg.c_str();
  String str2 = "power";
  String str3 = "next_eff";
  String str4 = "red_led_echo";
  String str5 = "red_led_speed+";
  String str6 = "red_led_speed-";

  String briKey = "02";                         // "02_bri_5"
  if (str1.substring(0, 2) == briKey) {
    if (str1.endsWith(String("0"))) {      brightness = 0;
    } else if (str1.endsWith(String("1"))) {      brightness = 26;
    } else if (str1.endsWith(String("2"))) {      brightness = 51;
    } else if (str1.endsWith(String("3"))) {      brightness = 77;
    } else if (str1.endsWith(String("4"))) {      brightness = 102;
    } else if (str1.endsWith(String("5"))) {      brightness = 128;
    } else if (str1.endsWith(String("6"))) {      brightness = 153;
    } else if (str1.endsWith(String("7"))) {      brightness = 179;
    } else if (str1.endsWith(String("8"))) {      brightness = 204;
    } else if (str1.endsWith(String("9"))) {      brightness = 230;
    } else if (str1.endsWith(String("M"))) {      brightness = 255;
    }
  }

  String modKey = "01";                         // "01_mode_0"
  if (str1.substring(0, 2) == modKey) {
    if (str1.endsWith(String("0"))) {      mode = 0;
    } else if (str1.endsWith(String("1"))) {      mode = 1;
    } else if (str1.endsWith(String("2"))) {      mode = 2;
    } else if (str1.endsWith(String("3"))) {      mode = 3;
    } else if (str1.endsWith(String("4"))) {      mode = 4;
    } else if (str1.endsWith(String("5"))) {      mode = 5;
    } else if (str1.endsWith(String("6"))) {      mode = 6;
    } else if (str1.endsWith(String("7"))) {      mode = 7;
    } else if (str1.endsWith(String("8"))) {      mode = 8;
    } else if (str1.endsWith(String("9"))) {      mode = 9;
    }
  }

  if (str1.equals(str2)) {
   if (power == 1) {
     power = 0;
     mesh.sendSingle(2661345693,"redled_off");
    } else {
     power = 1;
     mesh.sendSingle(2661345693,"redled_on");
   }
  }

  else if (str1.equals(str3)) {
    mode++;
    if (mode > 9){
      mode = 0;
    }
    String moode = "01_mode_"; 
    moode = moode + mode;
    mesh.sendSingle(2661345693,moode);
  }

  else if (str1.equals(str4)) {
    if (power == 0){
      mesh.sendSingle(2661345693,"redled_off");
    } else {
      mesh.sendSingle(2661345693,"redled_on");
    }

    String moode = "01_mode_"; 
    moode = moode + mode;
    mesh.sendSingle(2661345693,moode);
  }

  else if (str1.equals(str5)) {
    led_speed ++;
    Serial.print(led_speed);
  }

  else if (str1.equals(str6)) {
   if (led_speed != 0) {
      led_speed = led_speed -1;
      Serial.print(led_speed);
   }
  }

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);

  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( 20 );
    
  mesh.init( MESH_PREFIX, MESH_PASSWORD/*, &userScheduler, MESH_PORT*/ );
  mesh.onReceive(&receivedCallback);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop(){

  EVERY_N_MILLISECONDS( led_speed ) { gHue++; }

  mesh.update();

  if (power == 1) {
    FastLED.setBrightness( brightness );
    switch (mode){
      case 0:{        rainbow();     break;}
      case 1:{        rainbowWithGlitter();  break;}
      case 2:{        bpm();  break;}
      case 3:{        red();               break;}
      case 4:{        juggle();              break;}
      case 5:{        Fire2012();         break;}     //ls
      case 6:{        confetti();         break;}
      case 7:{        green();           break;}
      case 8:{        rasta();             break;}    //ls
      case 9:{        white();             break;} 
    }
  } else { FastLED.setBrightness( 0 ); }
  
  
  FastLED.show();

}

void red(){
    for(int i = 0; i < NUM_LEDS; i++) {   
    leds[i] = CRGB(255, 0, 0);
  }
}

void white(){
    for(int i = 0; i < NUM_LEDS; i++) {   
    leds[i] = CRGB(255, 255, 255);
  }
}

void green(){
    for(int i = 0; i < NUM_LEDS; i++) {   
    leds[i] = CRGB(0, 255, 0);
  }
}

void rasta() {
  unsigned long currentTime = millis(); // Поточний час CRGB::White;
  
  if (currentTime - previousTime >= led_speed) {
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


void rainbow() {
  for (int i = 16; i < 32; i++) {
    leds[i] = CHSV(gHue + (i * 256 / 32), 255, 255);
  }

  for (int i = 0; i < 16; i++) {
    leds[i] = CHSV((gHue + 128 ) - (i * 256 / 32), 255, 255);
  }

}

void rainbowWithGlitter() {
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void Fire2012() {
// Масив показників температури в кожній клітинці симуляції.
  static uint8_t heat[NUM_LEDS];
  unsigned long currentTime = millis();

  if (currentTime - previousTime >= led_speed) {
    previousTime = currentTime;
    // Крок 1. Знизити температуру кожної клітинки незначно.
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Крок 2. Тепло з кожної клітинки переміщається 'вгору' та трохи дифузіюється.
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Крок 3. Випадково запалювати нові "іскри" тепла біля нижньої частини.
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Крок 4. Відповідність між клітинками тепла та кольорами світлодіодів.
    for( int j = 0; j < NUM_LEDS; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }
  }
}

void bpm(){
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( led_speed, 64, 255);

  for( int i = 16; i < 32; i++) { 
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }

  for( int i = 0; i < 16; i++) { 
    leds[i] = ColorFromPalette(palette, gHue+((i + 16)*2), beat-gHue-(i*10));
  }
}

