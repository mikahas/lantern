# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Arduino firmware for a WS2812B LED lantern built on an ATtiny85 microcontroller. Two firmware variants exist in the same directory:

- **lantern.ino** — simpler version using a potentiometer for brightness and a button to cycle modes
- **rotary-encoder.ino** — advanced version using a rotary encoder + button for a two-menu UI

Both share [animations.h](animations.h), which defines 12+ FastLED gradient palettes.

## Building & Flashing

This project uses the Arduino IDE. There is no Makefile or CLI build system.

1. Open the desired `.ino` file in Arduino IDE
2. Install the **FastLED** library (Library Manager)
3. Install **ATtiny85 board support** (e.g., via Boards Manager with the Digistump or ATTinyCore URL)
4. Select the correct board and programmer, then Upload

## Architecture

### Hardware Differences

| Feature | lantern.ino | rotary-encoder.ino |
|---|---|---|
| MCU | ATtiny85 | ATtiny85 (8 MHz) |
| LEDs | 4× WS2811 | 22× WS2812 |
| Input | Potentiometer (A1) + button (pin 4) | Rotary encoder (PINB0/1) + button (PINB4) |
| Animations | 4 palettes | 3 palettes (memory constrained) |

### Control Flow — lantern.ino

Cycles through 5 states via button press: Blue → Green → Red → (4 animations). The potentiometer maps to brightness (10–254).

### Control Flow — rotary-encoder.ino

Two-menu system toggled by button press:
- **MENU_BRIGHTNESS** — 25 steps (10–255)
- **MENU_ANIMATION** — 8 solid colors + 3 palette animations

Rotation is detected via a pin change interrupt (`PCINT0_vect` on PINB0/1). The encoder logic reads the rising edge on CLK and compares DT to determine direction, with wraparound handling for cycling animations.

### Memory Constraints (ATtiny85)

The ATtiny85 has **8 KB flash**. `animations.h` contains more palettes than can fit simultaneously. When editing rotary-encoder.ino, only 3 palettes are active — adding more will exceed flash. The comment in the file reads: *"All animations can't fit in Attiny85 so choose wisely."*

### animations.h

Defines gradient palettes using FastLED's `DEFINE_GRADIENT_PALETTE` macro. Available palettes (not all usable at once on ATtiny85):

`halloween_gp`, `blackhorse_gp`, `rainbow_gp`, `otis_gp`, `autumnrose_gp`, `bambooblossom_gp`, `healingangel_gp`, `teabearrose_gp`, `Wild_Orange_gp`, `Molten_lava_gp`, `radioactive_slime_gp`, `bumblebee_gp`, `bhw4_011_gp`
