#ifndef LED_H_
#define LED_H_

#include "event.h"

void led_on(bool blink);
void led_off(void);
void led_process_event(event_t evt);

#endif // LED_H_
