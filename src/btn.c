#include "btn.h"
#include "event.h"
#include "timer.h"

#include "brd/brd.h"
#include "brd/gpio.h"

static bool btn_last_dit_state = false;
static uint32_t btn_last_dit_tick = 0;

static bool btn_last_dah_state = false;
static uint32_t btn_last_dah_tick = 0;

static bool btn_last_straight_key_state = false;
static uint32_t btn_last_straight_key_tick = 0;

static timer_t btn_dit_timer;
static timer_t btn_dah_timer;
static timer_t btn_straight_key_timer;

void btn_handle_irq(void) {
	event_push(EVENT_BTN);
}

static void btn_dit_timer_cb(void *arg) {
	btn_update();
}

static void btn_dah_timer_cb(void *arg) {
	btn_update();
}

static void btn_straight_key_timer_cb(void *arg) {
	btn_update();
}

void btn_update(void) {
	uint32_t now = brd_get_tick();

	bool state = gpio_is_dit_active();
	if (state != btn_last_dit_state) {
		if (now - btn_last_dit_tick < BTN_DEBOUNCE_MS) {
			timer_start(&btn_dit_timer, BTN_DEBOUNCE_MS - (now - btn_last_dit_tick), false, btn_dit_timer_cb, NULL);
		} else {
			timer_stop(&btn_dit_timer);
			btn_last_dit_tick = now;
			btn_last_dit_state = state;

			if (state)
				event_push(EVENT_DIT_PRESS);
			else
				event_push(EVENT_DIT_RELEASE);
		}
	}

	state = gpio_is_dah_active();
	if (state != btn_last_dah_state) {
		if (now - btn_last_dah_tick < BTN_DEBOUNCE_MS) {
			timer_start(&btn_dah_timer, BTN_DEBOUNCE_MS - (now - btn_last_dah_tick), false, btn_dah_timer_cb, NULL);
		} else {
			timer_stop(&btn_dah_timer);
			btn_last_dah_tick = now;
			btn_last_dah_state = state;

			if (state)
				event_push(EVENT_DAH_PRESS);
			else
				event_push(EVENT_DAH_RELEASE);
		}
	}

	state = gpio_is_straight_key_active();
	if (state != btn_last_straight_key_state) {
		if (now - btn_last_straight_key_tick < BTN_DEBOUNCE_MS) {
			timer_start(&btn_straight_key_timer, BTN_DEBOUNCE_MS - (now - btn_last_straight_key_tick), false, btn_straight_key_timer_cb, NULL);
		} else {
			timer_stop(&btn_straight_key_timer);
			btn_last_straight_key_tick = now;
			btn_last_straight_key_state = state;

			if (state)
				event_push(EVENT_STRAIGHT_KEY_PRESS);
			else
				event_push(EVENT_STRAIGHT_KEY_RELEASE);
		}
	}
}
