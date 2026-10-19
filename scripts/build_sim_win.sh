#!/bin/bash
set -euo pipefail

CC=x86_64-w64-mingw32-gcc

SRCS="
  sim/sim_gdi.c
  sim/sim_stubs_win.c
  sim/hal/input_sim.c
  sim/hal/sound_sim.c
  sim/hal/touch_sim.c
  sim/hal/wifi_sim.c
  src/kernel/scheduler.c
  src/kernel/memory.c
  src/kernel/shell.c
  src/kernel/fs.c
  src/gui/window_manager.c
  src/gui/widgets.c
  src/services/event_bus.c
  src/services/app_manager.c
  src/apps/home.c
  src/apps/files.c
  src/apps/settings.c
  src/apps/network.c
"

INCLUDES="-I src -I src/hal -I src/gui -I src/kernel -I src/services -I src/apps -I sim/stubs -I sim/hal"
DEFINES="-D_WIN32_WINNT=0x0601"
FLAGS="-O2 -std=c11 -Wall -Wno-unused-function"
LDFLAGS="-mwindows -lgdi32 -luser32"

echo "=== HorizonOS Windows Simulator Build ==="
echo "Compiler: $($CC --version | head -1)"
echo "Sources:  $(echo $SRCS | wc -w) files"

$CC $FLAGS $INCLUDES $DEFINES $SRCS -o horizonos_sim.exe $LDFLAGS

echo ""
echo "=== Build successful ==="
ls -lh horizonos_sim.exe
file horizonos_sim.exe
