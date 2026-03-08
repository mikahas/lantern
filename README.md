# Lantern

A handheld LED lantern built around an ATtiny85 and WS2812B LEDs. Controlled by a rotary encoder and a button, powered by a single 18650 battery chargeable over USB.

[Build photos](https://www.instagram.com/p/BiZ8sZbgN5C/)

## Features

- **Brightness control** — rotate the encoder to dim or brighten
- **Color & animation selector** — press the button to switch menus, then rotate through solid colors and animated gradient palettes
- **Animated palettes** — flowing gradient effects using FastLED (defined in `animations.h`)
- **Compact** — fits on an ATtiny85 at 8 MHz with careful memory management

## Hardware

| Component | Detail |
|---|---|
| MCU | ATtiny85 @ 8 MHz |
| LEDs | WS2812B strip (configurable count) |
| Input | Rotary encoder + momentary button |
| Power | 18650 Li-ion, USB charging |

## Firmware

`lantern.ino` — two-menu UI:
- **Menu 1 (Brightness):** rotate to adjust
- **Menu 2 (Animation):** rotate to cycle colors/palettes, press button to return to brightness

`animations.h` defines many gradient palettes, but only a few fit in flash at once due to ATtiny85 memory limits. Swap them in/out by commenting/uncommenting in the `animations[]` array.

## Building

1. Open `lantern.ino` in Arduino IDE
2. Install **FastLED** (Library Manager)
3. Install **ATtiny85 board support** via Boards Manager (Digistump or ATTinyCore)
4. Set board to ATtiny85 @ 8 MHz internal clock
5. Use an **Arduino Uno as ISP** to program the ATtiny85 (Sketch → Upload Using Programmer)
