
#define FASTLED_INTERNAL  // disable pragma message for fastled
#include <FastLED.h>
#include "animations.h"

// Pins for Attiny85
#define BUTTON_PIN  PINB4
#define LED_PIN     PINB3
#define ENC_PIN_A   PINB0
#define ENC_PIN_B   PINB1

// LED setup
#define NUM_LEDS            10
#define BRIGHTNESS_DEFAULT  255
#define BRIGHTNESS_MIN      10
#define BRIGHTNESS_STEPS    25  // Rotary Encoder steps for adjusting brightness
#define UPDATES_PER_SECOND  100

// Rotary encoder rotation directions
#define INCREMENT true
#define DECREMENT false

enum uiMenu {
  MENU_BRIGHTNESS = 1,
  MENU_ANIMATION = 2,
};

uint8_t pulsesBrightness = 0;
uint8_t pulsesAnimation = 0;

uint8_t currentAnimation = 0;
uint8_t oldAnimation = 0;
uint8_t currentMenu = MENU_BRIGHTNESS; // 1=brightness, 2=color/animation
uint8_t oldButtonState = LOW;
uint8_t currentBrightness = BRIGHTNESS_DEFAULT;
uint8_t oldBrightness = BRIGHTNESS_DEFAULT;
CRGBPalette16 currentPalette;
CRGB leds[NUM_LEDS];

CRGB colors[] = {
  CRGB::Red,
  CRGB::White,
  CRGB::Blue,
  CRGB::Green,
  CRGB::Yellow,
  CRGB::Purple,
  CRGB::Aqua,
  //CRGB::Black,  // LEDs are off
};
uint8_t colorCount = sizeof(colors) / sizeof(colors[0]);

// All animations can't fit in Attiny85 so choose wisely
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
    bhw4_011_gp, // * 
};
uint8_t animationCount = sizeof(animations) / sizeof(animations[0]);

// One notch/step with rotary encoder equals 4 ticks
uint8_t animationPulsesMax = 4 * (animationCount + colorCount) - 1;

void setup(){
  delay(1000); // power-up safety delay
  pinMode(BUTTON_PIN, INPUT);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS_DEFAULT);
  setColor(colors[0]);  // start with the first color in colors array

  pinMode(ENC_PIN_A, INPUT);
  pinMode(ENC_PIN_B, INPUT);

  GIMSK = 0b00100000; // turns on pin change interrupts
  PCMSK = 0b00000011; // turn on attiny interrupt pins: PB0, PB1
  sei();
}

void loop(){
  // Motion speed counter
  static uint8_t startIndex = 0;
  startIndex++;

  // Handle menu states
  uint8_t buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == HIGH && oldButtonState == LOW) currentMenu++;
  if (currentMenu > MENU_ANIMATION) currentMenu = MENU_BRIGHTNESS;
  oldButtonState = buttonState;

  switch (currentMenu) {
    case MENU_BRIGHTNESS:
      currentBrightness = map(
        pulsesBrightness,
        0, BRIGHTNESS_STEPS * 4,
        BRIGHTNESS_MIN, 255
      );
      if (currentBrightness != oldBrightness) {
        FastLED.setBrightness(currentBrightness);
        oldBrightness = currentBrightness;
      }
      break;
    case MENU_ANIMATION:
      currentAnimation = pulsesAnimation/4;
      break;
    default:
      break;
  }

  if (currentAnimation >= colorCount) {
    currentPalette = animations[currentAnimation - colorCount];
    FillLEDsFromPaletteColors(startIndex);
  } else if (currentAnimation != oldAnimation) {
    setColor(colors[currentAnimation]);
    oldAnimation = currentAnimation;
  }

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void setColor(CRGB color) {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
  }
}

void FillLEDsFromPaletteColors(uint8_t colorIndex) {
  for (uint8_t i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(currentPalette, colorIndex, 255, LINEARBLEND);
    colorIndex += 3;
  }
}

// Handle rotary encoder interrupts
volatile int lastEncoded = 0;
ISR(PCINT0_vect) {
  int MSB = digitalRead(ENC_PIN_A); //MSB = most significant bit, HIGH/LOW
  int LSB = digitalRead(ENC_PIN_B); //LSB = least significant bit, HIGH/LOW
 
  int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number, 
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
 
  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011)
    handleStateCount(INCREMENT);
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000)
    handleStateCount(DECREMENT);
 
  lastEncoded = encoded; //store this value for next time

}

void handleStateCount(bool direction) {
  switch(currentMenu) {
    case MENU_BRIGHTNESS:
      if (!direction && pulsesBrightness > 0) {
        pulsesBrightness--;
      } else if (direction && pulsesBrightness < BRIGHTNESS_STEPS * 4) {
        pulsesBrightness++;
      }
      break;
    case MENU_ANIMATION:
      if (!direction) {
        pulsesAnimation--;
        if (pulsesAnimation > animationPulsesMax) pulsesAnimation = animationPulsesMax;
      } else {
        pulsesAnimation++;
      }
      if (pulsesAnimation > animationPulsesMax) pulsesAnimation = 0;
      break;
    default:
      break;
  }

}
