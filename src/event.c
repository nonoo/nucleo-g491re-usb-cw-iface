#include "event.h"

#define EVENT_QUEUE_SIZE 64

static volatile event_t event_queue[EVENT_QUEUE_SIZE];
static volatile uint32_t queue_head = 0;
static volatile uint32_t queue_tail = 0;

void event_push(event_t evt) {
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	uint32_t next_head = (queue_head + 1) % EVENT_QUEUE_SIZE;
	if (next_head != queue_tail) {
		event_queue[queue_head] = evt;
		queue_head = next_head;
	}
	__set_PRIMASK(primask);
}

bool event_pop(event_t *evt) {
	bool has_event = false;
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	if (queue_head != queue_tail) {
		*evt = event_queue[queue_tail];
		queue_tail = (queue_tail + 1) % EVENT_QUEUE_SIZE;
		has_event = true;
	}
	__set_PRIMASK(primask);
	return has_event;
}
