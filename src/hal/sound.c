#include "sound.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

void sound_init(void) {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
}

void sound_play_tone(uint32_t freq_hz, uint32_t duration_ms) {
    if (freq_hz == 0) { sound_stop(); return; }
    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);
    uint32_t wrap = 125000000 / freq_hz;
    pwm_set_wrap(slice, wrap);
    pwm_set_chan_level(slice, pwm_gpio_to_channel(BUZZER_PIN), wrap / 2);
    pwm_set_enabled(slice, true);
    sleep_ms(duration_ms);
    pwm_set_enabled(slice, false);
}

void sound_play_boot(void) {
    sound_play_tone(523, 80);
    sound_play_tone(659, 80);
    sound_play_tone(784, 120);
}

void sound_play_click(void) {
    sound_play_tone(1200, 20);
}

void sound_stop(void) {
    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_enabled(slice, false);
}

