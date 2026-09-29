#include "touch.h"
void  touch_init(void)         {}
bool  touch_read(TouchEvent* e){ if(e) e->pressed=false; return false; }
