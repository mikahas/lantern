
#define FASTLED_INTERNAL  // disable pragma message for fastled
#include <FastLED.h>
#include "animations.h"

// Pins for Attiny85
#define BUTTON_PIN  PINB4
#define LED_PIN     PINB3
#define ENC_PIN_A   PINB0
#define ENC_PIN_B   PINB1

// LED setup
#define NUM_LEDS            22
#define BRIGHTNESS_DEFAULT  5
#define BRIGHTNESS_MIN      10
#define BRIGHTNESS_STEPS    25  // Rotary Encoder steps for adjusting brightness
#define UPDATES_PER_SECOND  120

// Rotary encoder & button
#define INCREMENT true
#define DECREMENT false
#define DEBOUNCE_DELAY 15 // Milliseconds

enum uiMenu {
  MENU_BRIGHTNESS = 1,
  MENU_ANIMATION = 2,
  // TODO: Single LED selector?
};

uint8_t pulsesBrightness = BRIGHTNESS_DEFAULT;
uint8_t pulsesAnimation = 0;

uint8_t currentAnimation = 0;
uint8_t oldAnimation = 0;
uint8_t currentMenu = MENU_BRIGHTNESS; // 1=brightness, 2=color/animation

byte lastButtonPressState = LOW;
unsigned long lastDebounceTime = 0;
byte buttonActiveState = HIGH; // HIGH with INPUT_PULLUP means button is not pressed, LOW = pressed

uint8_t currentBrightness = 0;
uint8_t oldBrightness = 0;

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
    //halloween_gp, // *
    //blackhorse_gp,
    autumnrose_gp,  // *
    bambooblossom_gp,
    //healingangel_gp,
    Wild_Orange_gp,
    // Molten_lava_gp,
    radioactive_slime_gp, // *
    // bumblebee_gp,
    //bhw4_011_gp, // * 
};
uint8_t animationCount = sizeof(animations) / sizeof(animations[0]);

uint8_t animationPulsesMax = (animationCount + colorCount) - 1;
void setup(){

  delay(500); // power-up safety delay
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(0);
  setColor(colors[0]);  // start with the first color in colors array

  pinMode(ENC_PIN_A, INPUT);
  pinMode(ENC_PIN_B, INPUT);

  GIMSK = 0b00100000; // Turns on pin change interrupts
  PCMSK = 0b00000011; // Turn on attiny interrupt pins: PB0, PB1
  sei();
}

void loop() {
  unsigned long currentTime = millis();
  
  // Motion speed counter
  static uint8_t startIndex = 0;
  startIndex++;

  // Handle menu states
  byte buttonPressState = digitalRead(BUTTON_PIN);
  if (buttonPressState != lastButtonPressState) lastDebounceTime = currentTime;

  // Quick press handling
  if ((currentTime - lastDebounceTime) > DEBOUNCE_DELAY &&
    buttonActiveState != buttonPressState
  ) {
    buttonActiveState = buttonPressState;
    if (buttonPressState == LOW) {    // button pressed
      currentMenu++;
      if (currentMenu > MENU_ANIMATION) currentMenu = MENU_BRIGHTNESS;
    }
  }
  lastButtonPressState = buttonPressState;

  // Handle menu
  switch (currentMenu) {
    case MENU_BRIGHTNESS:
      currentBrightness = map(
        pulsesBrightness,
        0, BRIGHTNESS_STEPS,
        BRIGHTNESS_MIN, 255
      );
      if (currentBrightness != oldBrightness) {
        FastLED.setBrightness(currentBrightness);
        oldBrightness = currentBrightness;
      }
      break;
    case MENU_ANIMATION:
      currentAnimation = pulsesAnimation;
      break;
    default:
      break;
  }

  if (currentAnimation >= colorCount) {
    currentPalette = animations[currentAnimation - colorCount];
    FillLEDsFromPaletteColors(startIndex);
  } else if (currentAnimation != oldAnimation) {
    setColor(colors[currentAnimation]);
  }
  oldAnimation = currentAnimation; // Needed for making simple colors work

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
  int currentStateCLK = digitalRead(ENC_PIN_B);

  // If last and current state of CLK are different, then pulse occurred
	// React to only 1 state change to avoid double count
	if (currentStateCLK != lastEncoded && currentStateCLK == 1) {

		// If the DT state is different than the CLK state then
		// the encoder is rotating CCW so decrement
		if (digitalRead(ENC_PIN_A) != currentStateCLK) handleStateCount(DECREMENT);
		else handleStateCount(INCREMENT);
	}

	// Remember last CLK state
	lastEncoded = currentStateCLK;
}

void handleStateCount(bool direction) {
  switch(currentMenu) {
    case MENU_BRIGHTNESS:
      if (!direction && pulsesBrightness > 0) {
        pulsesBrightness--;
      } else if (direction && pulsesBrightness < BRIGHTNESS_STEPS) {
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
