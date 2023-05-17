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

bool power = 1;

String str1 = "";

void receivedCallback( uint32_t from, String &msg ) {

  str1 = msg.c_str();
  String str2 = "power";

  if (str1.equals(str2)) {

   if (power == 1) {
     power = 0;
    } else {
     power = 1;
   }
  }
}


// Цей приклад показує кілька способів налаштування та використання 'палітр' кольорів
// з використанням FastLED.
//
// Ці компактні палітри надають простий спосіб перекрасити вашу
// анімацію на льоту, швидко, легко і з низькими витратами пам'яті.
//
// ВИКОРИСТАННЯ палітр в практиці набагато простіше, ніж в теорії, тому спочатку просто
// запустіть цей скетч і спостерігайте за красивими світлами, а потім прочитайте
// код. Хоча у цьому скетчі є вісім (або більше) різних схем кольорів,
// весь скетч компілюється приблизно до 6,5 Кб на AVR.
//
// FastLED надає кілька попередньо налаштованих палітр кольорів і
// дозволяє дуже просто створювати власні кольорові схеми з палітрами.
//
// Деякі примітки щодо більш абстрактної "теорії та практики"
// компактних палітр FastLED знаходяться у кінці цього файлу.



CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


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


void loop(){

  Serial.print(power);

  mesh.update();

  if (power == 1) {
    FastLED.setBrightness( 255 );
    SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND;
  } else {
    FastLED.setBrightness( 0 );
  }
    
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
    
  FillLEDsFromPaletteColors( startIndex);
    
  FastLED.show();
}

void FillLEDsFromPaletteColors( uint8_t colorIndex) {
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

// Тут показано кілька різних палітр кольорів.
//
// FastLED надає кілька "вбудованих" палітр: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Крім того, ви можете вручну визначати свої власні палітри кольорів або писати
// код, який створює палітри кольорів на льоту. Все це показано тут.

// void ChangePalettePeriodically()
// {
//     uint8_t secondHand = (millis() / 1000) % 60;
//     static uint8_t lastSecond = 99;
    
//     if( lastSecond != secondHand) {
//         lastSecond = secondHand;
//         if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
//         if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
//         if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
//         if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
//         if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
//         if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
//         if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
//         if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
//         if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
//         if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
//         if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
//     }
// }

// Ця функція заповнює палітру повністю випадковими кольорами.
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; ++i) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}

// Ця функція налаштовує палітру чорно-білих смуг за допомогою коду.
// Оскільки палітра фактично є масивом з шістнадцятьма кольорами CRGB,
// різні функції fill_* можна використовувати для їх налаштування.
void SetupBlackAndWhiteStripedPalette()
{
    // 'зачистити' всі 16 записів палітри (зробити їх чорними)...
    fill_solid( currentPalette, 16, CRGB::Black);
    // і встановити кожний четвертий запис на білий колір.
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
    
}

// Ця функція налаштовує палітру фіолетових і зелених смуг.
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CRGB(255, 0, 0); //CHSV( HUE_REED, 255, 255);
    
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    
    currentPalette = CRGBPalette16(
                                   purple,  purple,  purple,  purple,
                                   purple, purple, purple,  purple,
                                   purple,  purple,  purple,  purple,
                                   purple, purple, purple,  purple );
}


// Цей приклад показує, як налаштувати статичну палітру кольорів,
// яка зберігається в PROGMEM (флеш-пам'яті), яка майже завжди більш
// розкішна, ніж RAM. Статична палітра PROGMEM, подібно до цієї,
// займає 64 байти флеш-пам'яті.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, // 'білий' занадто яскравий порівняно з червоним і синім
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};



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
