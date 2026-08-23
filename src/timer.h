#ifndef TIMER_H_
#define TIMER_H_

typedef void (*timer_cb_t)(void *arg);

typedef struct timer {
	bool active;
	bool periodic;
	uint32_t period_ms;
	uint32_t expire_tick;
	timer_cb_t callback;
	void *arg;
	struct timer *next;
} timer_t;

void timer_start(timer_t *timer, uint32_t delay_ms, bool periodic, timer_cb_t cb, void *arg);
void timer_stop(timer_t *timer);

void timer_process(void);

#endif // TIMER_H_
