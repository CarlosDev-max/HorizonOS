#pragma once
#define CYW43_AUTH_WPA2_AES_PSK 0x00400004
static inline int  cyw43_arch_init(void)                              { return 0; }
static inline void cyw43_arch_enable_sta_mode(void)                   {}
static inline int  cyw43_arch_wifi_connect_timeout_ms(
    const char* s, const char* p, uint32_t a, uint32_t t)             { (void)s;(void)p;(void)a;(void)t; return -1; }
