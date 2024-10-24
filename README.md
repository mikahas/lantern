# Lantern v.0.1

This is an LED lantern

[See implementation here](https://www.instagram.com/p/BiZ8sZbgN5C/)

## Versions

- Use lantern.ino with a potentiometer setup.
- Use rotary-encoder.ino for more sophisticated user interface.
- Both use the same animations.h file.

## Specs
- Uses an ATtiny85 microcontroller with a ws2812b RGB LED
- Potentiometer for controlling the brightness
- Momentary push button to switch between modes
- animations.h contains more sequences that can fit in the ATtiny so only a handful is used
- Powered by a single 18650 battery which can be charged via USB
- Has four modes:
-- White, green, red and animation
