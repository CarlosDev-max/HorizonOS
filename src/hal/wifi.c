#include "wifi.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

static bool connected = false;

void wifi_init(void) {
    cyw43_arch_init();
    cyw43_arch_enable_sta_mode();
}

void wifi_poll(void) {
    // lwIP polling handled by background arch
}

bool wifi_connect(const char* ssid, const char* pass) {
    int err = cyw43_arch_wifi_connect_timeout_ms(ssid, pass, CYW43_AUTH_WPA2_AES_PSK, 10000);
    connected = (err == 0);
    return connected;
}

bool wifi_is_connected(void) {
    return connected;
}

const char* wifi_ip(void) {
    // TODO: return ip4addr_ntoa
    return "0.0.0.0";
}

