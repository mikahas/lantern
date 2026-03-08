
#define FASTLED_INTERNAL  // disable pragma message for fastled
#include <FastLED.h>
#include "animations.h"

#define BUTTON_PIN 4
#define POT_PIN A1
#define LED_PIN    3
#define NUM_LEDS    4
#define DEFAULT_BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define UPDATES_PER_SECOND 100
#define DEBOUNCE_DELAY 15

uint8_t currentAnimation = 0;
uint8_t currentState = 1; // 1 - 4
uint8_t lastButtonRawState = LOW;
uint8_t buttonActiveState = LOW;
unsigned long lastDebounceTime = 0;

CRGBPalette16 currentPalette;

CRGB leds[NUM_LEDS];

CRGBPalette16 animations[] = {
    halloween_gp, // *
    // blackhorse_gp,
    autumnrose_gp,  // *
    bambooblossom_gp,
    // healingangel_gp,
    // Wild_Orange_gp,
    // Molten_lava_gp,
    radioactive_slime_gp, // *
    // bumblebee_gp,
    bhw4_011_gp // *
};
#define NUM_ANIMATIONS (sizeof(animations) / sizeof(animations[0]))

void setup() {
    delay(1000); // power-up safety delay
    pinMode(POT_PIN, INPUT);
    pinMode(BUTTON_PIN, INPUT);
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(DEFAULT_BRIGHTNESS);
}


void loop() {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */

    unsigned long currentTime = millis();
    uint8_t buttonState = digitalRead(BUTTON_PIN);

    if (buttonState != lastButtonRawState) lastDebounceTime = currentTime;
    lastButtonRawState = buttonState;

    if ((currentTime - lastDebounceTime) > DEBOUNCE_DELAY && buttonActiveState != buttonState) {
      buttonActiveState = buttonState;
      if (buttonState == HIGH) currentState++;
    }

    if (currentState > 4) {
      currentAnimation++;
      currentState = 1;
    }

    if (currentAnimation >= NUM_ANIMATIONS) currentAnimation = 0;

    currentPalette = animations[currentAnimation];
    
    int brightness = analogRead(POT_PIN);
    int mappedBrightness = map(brightness, 0, 1023, 10, 254);
    FastLED.setBrightness(mappedBrightness);

    switch (currentState) {
      case 1:
        setColor(CRGB::Blue);
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

    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void setColor(CRGB color) {
  for(uint8_t i=0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
}

void FillLEDsFromPaletteColors(uint8_t colorIndex) {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, 255, LINEARBLEND);
    colorIndex += 3;
  }
}
