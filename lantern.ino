
#define FASTLED_INTERNAL  // disable pragma message for fastled
#include <FastLED.h>
#include "animations.h"

#define BUTTON_PIN 1
#define POT_PIN A1
#define LED_PIN    0
#define NUM_LEDS    4
#define DEFAULT_BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100

uint8_t currentAnimation = 0;
uint8_t currentState = 1; // 1 - 4
uint8_t oldButtonState = LOW;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

CRGB leds[NUM_LEDS];

CRGBPalette16 animations[] = {
    halloween_gp, // *
    // blackhorse_gp,
    autumnrose_gp,  // *
    // bambooblossom_gp,
    // healingangel_gp,
    // Wild_Orange_gp,
    // Molten_lava_gp,
    radioactive_slime_gp, // *
    // bumblebee_gp,
    bhw4_011_gp // * 
};

void setup() {
    delay(1000); // power-up safety delay
    pinMode(POT_PIN, INPUT);
    pinMode(BUTTON_PIN, INPUT);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(DEFAULT_BRIGHTNESS);
    currentBlending = LINEARBLEND;
}


void loop() {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */

    int buttonState = digitalRead(BUTTON_PIN);

    if (buttonState == HIGH && oldButtonState == LOW) {
      currentState++;
      
    }
    
    if (currentState > 4) {
      currentAnimation++;
      currentState = 1;
    }

    if (currentAnimation >= 4) currentAnimation = 0;

    currentPalette = animations[currentAnimation];
    
    int brightness = analogRead(POT_PIN);
    int mappedBrightness = map(brightness, 0, 1023, 10, 254);
    FastLED.setBrightness(mappedBrightness);

    switch (currentState) {
      case 1:
        setColor(CRGB::White);
        break;
      case 2:
        setColor(CRGB::Green);
        break;
      case 3:
        setColor(CRGB::Red);
        break;
      case 4:
        FillLEDsFromPaletteColors(startIndex);
        break;
      default:
        break;
    }

    oldButtonState = buttonState;

    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void setColor(CRGB color) {
  for(uint8_t i=0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
}

void FillLEDsFromPaletteColors( uint8_t colorIndex) {
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}
