#include "timer.h"
#include "event.h"
#include "btn.h"
#include "led.h"

#include "brd/brd.h"
#include "brd/gpio.h"
#include "uart/uart-console.h"
#include "usb/usb.h"

static __unused const char *TAG = "main";

static timer_t sleep_timer;
static bool is_sleeping = false;

static void sleep_timer_cb(void *arg) {
	is_sleeping = true;
}

int main(void) {
	HAL_Init();
	brd_clk_init();
	gpio_init();
#if UART_CONSOLE_ENABLED
	uart_console_init();
#endif

	usb_init();

	LOG(TAG, "init done\r\n");

	while (1) {
		timer_process();
		usb_process();

		event_t evt;
		if (event_pop(&evt)) {
			timer_stop(&sleep_timer);
			is_sleeping = false;

			switch (evt) {
				case EVENT_BTN:
					btn_update();
					break;
				case EVENT_LED_ON:
				case EVENT_LED_ON_BLINK:
				case EVENT_LED_OFF:
					led_process_event(evt);
					break;
				case EVENT_DIT_PRESS:
					led_on(false);
#if USB_KEYBOARD_ENABLED
					usb_keyboard_set_modifier(USB_KEYBOARD_MODIFIER_LEFTCTRL, true);
#endif
#if USB_MIDI_ENABLED
					usb_midi_send_note_on(1, 0x7F);
#endif
					LOG(TAG, "dit on\r\n");
					break;
				case EVENT_DIT_RELEASE:
					led_off();
					LOG(TAG, "dit off\r\n");
#if USB_KEYBOARD_ENABLED
					usb_keyboard_set_modifier(USB_KEYBOARD_MODIFIER_LEFTCTRL, false);
#endif
#if USB_MIDI_ENABLED
					usb_midi_send_note_off(1);
#endif
					break;
				case EVENT_DAH_PRESS:
					led_on(true);
					LOG(TAG, "dah on\r\n");
#if USB_KEYBOARD_ENABLED
					usb_keyboard_set_modifier(USB_KEYBOARD_MODIFIER_RIGHTCTRL, true);
#endif
#if USB_MIDI_ENABLED
					usb_midi_send_note_on(2, 0x7F);
#endif
					break;
				case EVENT_DAH_RELEASE:
					led_off();
					LOG(TAG, "dah off\r\n");
#if USB_KEYBOARD_ENABLED
					usb_keyboard_set_modifier(USB_KEYBOARD_MODIFIER_RIGHTCTRL, false);
#endif
#if USB_MIDI_ENABLED
					usb_midi_send_note_off(2);
#endif
					break;
				case EVENT_STRAIGHT_KEY_PRESS:
					led_on(false);
					LOG(TAG, "straight key on\r\n");
#if USB_KEYBOARD_ENABLED
					usb_keyboard_set_key(USB_KEY_SPACE, true);
#endif
#if USB_MIDI_ENABLED
					usb_midi_send_note_on(0, 0x7F);
#endif
					break;
				case EVENT_STRAIGHT_KEY_RELEASE:
					led_off();
					LOG(TAG, "straight key off\r\n");
#if USB_KEYBOARD_ENABLED
					usb_keyboard_set_key(USB_KEY_SPACE, false);
#endif
#if USB_MIDI_ENABLED
					usb_midi_send_note_off(0);
#endif
					break;
			}
		} else {
			if (!sleep_timer.active && !is_sleeping)
				timer_start(&sleep_timer, SLEEP_TIMEOUT_MS, false, sleep_timer_cb, NULL);

			if (is_sleeping)
				brd_enter_sleep();
		}
	}
}
