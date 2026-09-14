# Building & Running HorizonOS

## Quick Start (Wokwi simulation)

1. **Install dependencies**
   ```bash
   sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential
   ```

2. **Get pico-sdk**
   ```bash
   git clone https://github.com/raspberrypi/pico-sdk.git --depth=1 --branch 2.0.0
   cd pico-sdk && git submodule update --init --depth=1
   export PICO_SDK_PATH=$(pwd)
   ```

3. **Build**
   ```bash
   cd HorizonOS
   mkdir build && cd build
   cmake .. -DPICO_BOARD=pico_w
   make -j$(nproc)
   ```

4. **Open Wokwi**
   - Go to [wokwi.com](https://wokwi.com) → *New Project* → *Raspberry Pi Pico W*
   - Replace `diagram.json` with the one in `wokwi/`
   - Upload `build/horizonos.uf2`
   - Press **Play**

## Wokwi Navigation Controls

| Button | GPIO | Action |
|--------|------|--------|
| Blue   | GP2  | Navigate ◄ left |
| Green  | GP3  | Navigate ► right |
| Red    | GP4  | Open selected app (A) |

## Deploy to Pico 2W

1. Hold **BOOTSEL** while plugging in USB
2. Copy `build/horizonos.uf2` to the mounted drive
3. Pico reboots automatically

## UART Shell

Connect via any serial terminal at **115200 baud** (USB):

```
HorizonOS v0.1 shell ready. Type "help".
> help
Commands: help, tasks, mem, reboot
```

## Project Structure

```
src/
├── main.c              ← Boot entry point
├── kernel/             ← Scheduler, memory, LittleFS, UART shell
├── hal/                ← Display, touch, sound, WiFi, input drivers
├── gui/                ← Window manager, widgets, Switch-inspired theme
├── services/           ← Event bus, app manager
└── apps/               ← Home screen, Files, Network, Settings
wokwi/
├── diagram.json        ← Wokwi circuit (Pico W + ILI9341 + buzzer + buttons)
└── wokwi.toml
```
