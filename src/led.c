#include "led.h"
#include "timer.h"

#include "brd/gpio.h"

static timer_t led_timer;
static uint8_t led_blink_step = 0;

static void led_timer_cb(void *arg) {
	if (led_blink_step == 1) {
		gpio_led_off();
		led_blink_step = 2;
		timer_start(&led_timer, 50, false, led_timer_cb, NULL);
	} else if (led_blink_step == 2) {
		gpio_led_on();
		led_blink_step = 0;
	}
}

void led_on(bool blink) {
	event_push(blink ? EVENT_LED_ON_BLINK : EVENT_LED_ON);
}

void led_off(void) {
	event_push(EVENT_LED_OFF);
}

void led_process_event(event_t evt) {
	switch (evt) {
		case EVENT_LED_ON:
			timer_stop(&led_timer);
			led_blink_step = 0;
			gpio_led_on();
			break;
		case EVENT_LED_ON_BLINK:
			timer_stop(&led_timer);
			gpio_led_on();
			led_blink_step = 1;
			timer_start(&led_timer, 50, false, led_timer_cb, NULL);
			break;
		case EVENT_LED_OFF:
			timer_stop(&led_timer);
			led_blink_step = 0;
			gpio_led_off();
			break;
		default:
			break;
	}
}
