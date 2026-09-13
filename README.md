# HorizonOS 🌅

> A Nintendo Switch-inspired embedded OS for the Raspberry Pi Pico 2W

![Platform](https://img.shields.io/badge/platform-Pico%202W-blue)
![Language](https://img.shields.io/badge/language-C-green)
![Status](https://img.shields.io/badge/status-in%20development-orange)

## Overview

HorizonOS is a custom embedded operating system for the **Raspberry Pi Pico 2W**, featuring a touch-based GUI inspired by the Nintendo Switch UI. Built from scratch in C using the official pico-sdk.

## Hardware

| Component | Model |
|-----------|-------|
| MCU | Raspberry Pi Pico 2W (RP2350) |
| Display | ILI9341 2.8" 320×240 SPI |
| Touch | XPT2046 resistive touch |
| Audio | Passive buzzer (PWM) |
| WiFi | CYW43439 (onboard) |

## Architecture

```
┌─────────────────────────────────────┐
│           Apps Layer                │  Home · Files · Settings · Network
├─────────────────────────────────────┤
│           GUI Layer                 │  Window Manager · Widgets · Theme
├─────────────────────────────────────┤
│         Services Layer              │  Event Bus · App Manager · Net Stack
├─────────────────────────────────────┤
│          Kernel Layer               │  Scheduler · Memory · LittleFS · UART
├─────────────────────────────────────┤
│           HAL Layer                 │  Display · Touch · Sound · WiFi
├─────────────────────────────────────┤
│         Hardware Layer              │  ILI9341 · XPT2046 · Buzzer · CYW43
└─────────────────────────────────────┘
```

## Building

```bash
mkdir build && cd build
cmake ..
make -j4
```

## Simulation

Development and testing is done via [Wokwi](https://wokwi.com) using the Pico W as a proxy for the Pico 2W.

## License

MIT

