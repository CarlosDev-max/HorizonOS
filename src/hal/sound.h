#pragma once
#include <stdint.h>

#define BUZZER_PIN 26

void sound_init(void);
void sound_play_tone(uint32_t freq_hz, uint32_t duration_ms);
void sound_play_boot(void);
void sound_play_click(void);
void sound_stop(void);

