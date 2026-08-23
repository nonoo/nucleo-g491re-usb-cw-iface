#include "timer.h"

#include "brd/brd.h"

static timer_t *timer_head = NULL;

void timer_start(timer_t *timer, uint32_t delay_ms, bool periodic, timer_cb_t cb, void *arg) {
	if (timer == NULL || cb == NULL)
		return;

	uint32_t primask = __get_PRIMASK();
	__disable_irq();

	timer->periodic = periodic;
	timer->period_ms = delay_ms;
	timer->expire_tick = brd_get_tick() + delay_ms;
	timer->callback = cb;
	timer->arg = arg;
	timer->active = true;

	// Check if already in the list.
	timer_t *curr = timer_head;
	bool found = false;
	while (curr != NULL) {
		if (curr == timer) {
			found = true;
			break;
		}
		curr = curr->next;
	}

	if (!found) {
		timer->next = timer_head;
		timer_head = timer;
	}

	__set_PRIMASK(primask);
}

void timer_stop(timer_t *timer) {
	if (timer == NULL)
		return;

	uint32_t primask = __get_PRIMASK();
	__disable_irq();

	timer->active = false;

	__set_PRIMASK(primask);
}

bool timer_is_active(const timer_t *timer) {
	if (timer == NULL)
		return false;

	return timer->active;
}

bool timer_is_any_active(void) {
	uint32_t primask = __get_PRIMASK();
	__disable_irq();

	timer_t *curr = timer_head;
	bool active = false;
	while (curr != NULL) {
		if (curr->active) {
			active = true;
			break;
		}
		curr = curr->next;
	}

	__set_PRIMASK(primask);
	return active;
}

void timer_process(void) {
	uint32_t now = brd_get_tick();
	timer_t *curr = timer_head;

	while (curr != NULL) {
		if (curr->active && ((int32_t)(now - curr->expire_tick) >= 0)) {
			timer_cb_t cb = curr->callback;
			void *arg = curr->arg;

			if (curr->periodic)
				curr->expire_tick = now + curr->period_ms;
			else
				curr->active = false;

			if (cb != NULL)
				cb(arg);
		}
		curr = curr->next;
	}
}
