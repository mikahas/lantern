# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Arduino firmware for a WS2812B LED lantern built on an ATtiny85 microcontroller.

- **lantern.ino** — main firmware using a rotary encoder + button for a two-menu UI

It shares [animations.h](animations.h), which defines 12+ FastLED gradient palettes.

## Building & Flashing

This project uses the Arduino IDE. There is no Makefile or CLI build system.

1. Open `lantern.ino` in Arduino IDE
2. Install the **FastLED** library (Library Manager)
3. Install **ATtiny85 board support** (e.g., via Boards Manager with the Digistump or ATTinyCore URL)
4. Select the correct board and programmer, then Upload

## Architecture

### Hardware

| Feature | Value |
|---|---|
| MCU | ATtiny85 (8 MHz) |
| LEDs | 22× WS2812 |
| Input | Rotary encoder (PINB0/1) + button (PINB4) |
| Animations | 3 palettes (memory constrained) |

### Control Flow

Two-menu system toggled by button press:
- **MENU_BRIGHTNESS** — 25 steps (10–255)
- **MENU_ANIMATION** — 8 solid colors + 3 palette animations

Rotation is detected via a pin change interrupt (`PCINT0_vect` on PINB0/1). The encoder logic reads the rising edge on CLK and compares DT to determine direction, with wraparound handling for cycling animations.

### Memory Constraints (ATtiny85)

The ATtiny85 has **8 KB flash**. `animations.h` contains more palettes than can fit simultaneously. Only 3 palettes are active in `lantern.ino` — adding more will exceed flash. The comment in the file reads: *"All animations can't fit in Attiny85 so choose wisely."*

### animations.h

Defines gradient palettes using FastLED's `DEFINE_GRADIENT_PALETTE` macro. Available palettes (not all usable at once on ATtiny85):

`halloween_gp`, `blackhorse_gp`, `rainbow_gp`, `otis_gp`, `autumnrose_gp`, `bambooblossom_gp`, `healingangel_gp`, `teabearrose_gp`, `Wild_Orange_gp`, `Molten_lava_gp`, `radioactive_slime_gp`, `bumblebee_gp`, `bhw4_011_gp`
