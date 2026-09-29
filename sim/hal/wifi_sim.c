#include "wifi.h"
void        wifi_init(void)                              {}
void        wifi_poll(void)                              {}
bool        wifi_connect(const char* s, const char* p)  { (void)s;(void)p; return false; }
bool        wifi_is_connected(void)                      { return false; }
const char* wifi_ip(void)                               { return "0.0.0.0"; }
