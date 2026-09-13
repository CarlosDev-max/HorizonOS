#pragma once
#include <stdbool.h>

void wifi_init(void);
void wifi_poll(void);
bool wifi_connect(const char* ssid, const char* pass);
bool wifi_is_connected(void);
const char* wifi_ip(void);

