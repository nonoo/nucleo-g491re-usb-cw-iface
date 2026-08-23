#ifndef EVENT_H_
#define EVENT_H_

typedef enum {
	EVENT_BTN,
	EVENT_LED_ON,
	EVENT_LED_ON_BLINK,
	EVENT_LED_OFF,
	EVENT_DIT_PRESS,
	EVENT_DIT_RELEASE,
	EVENT_DAH_PRESS,
	EVENT_DAH_RELEASE,
	EVENT_STRAIGHT_KEY_PRESS,
	EVENT_STRAIGHT_KEY_RELEASE,
} event_t;

void event_push(event_t evt);
bool event_pop(event_t *evt);

#endif // EVENT_H_
