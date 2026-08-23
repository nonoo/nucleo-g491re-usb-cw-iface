#include "usb.h"
#include "brd/brd.h"
#include "tusb.h"

static __unused const char *TAG = "usb";

static volatile bool usb_mounted = false;
static uint8_t current_keyboard_modifier = 0;
static uint8_t current_keyboard_keycodes[6] = { 0 };
static bool keyboard_report_pending = false;

static void usb_keyboard_update(void) {
	if (keyboard_report_pending && tud_hid_ready()) {
		if (tud_hid_keyboard_report(0, current_keyboard_modifier, current_keyboard_keycodes)) {
			keyboard_report_pending = false;
		}
	}
}

void usb_init(void) {
	// Enable HSI48 oscillator
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
	RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		LOG(TAG, "HSI48 config failed\r\n");
		brd_error_handler();
	}

	// Select HSI48 as USB clock source
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		LOG(TAG, "USB clock config failed\r\n");
		brd_error_handler();
	}

	// Enable CRS (Clock Recovery System) synchronized to USB SOF
	__HAL_RCC_CRS_CLK_ENABLE();
	RCC_CRSInitTypeDef RCC_CRSInitStruct = { 0 };
	RCC_CRSInitStruct.Prescaler = RCC_CRS_SYNC_DIV1;
	RCC_CRSInitStruct.Source = RCC_CRS_SYNC_SOURCE_USB;
	RCC_CRSInitStruct.Polarity = RCC_CRS_SYNC_POLARITY_RISING;
	RCC_CRSInitStruct.ReloadValue = __HAL_RCC_CRS_RELOADVALUE_CALCULATE(48000000, 1000);
	RCC_CRSInitStruct.ErrorLimitValue = 34;
	RCC_CRSInitStruct.HSI48CalibrationValue = 32;
	HAL_RCCEx_CRSConfig(&RCC_CRSInitStruct);

	// Enable USB peripheral clock
	__HAL_RCC_USB_CLK_ENABLE();

	// Set interrupt priorities and enable IRQs
	HAL_NVIC_SetPriority(USB_HP_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(USB_HP_IRQn);

	HAL_NVIC_SetPriority(USB_LP_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(USB_LP_IRQn);

	HAL_NVIC_SetPriority(USBWakeUp_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(USBWakeUp_IRQn);

	tusb_init();
	LOG(TAG, "usb initialized\r\n");
}

void usb_process(void) {
	tud_task();
	usb_keyboard_update();
}

bool usb_is_mounted(void) {
	return usb_mounted && tud_mounted();
}

void usb_midi_send_note_on(uint8_t note_num, uint8_t velocity) {
	if (!tud_midi_mounted()) {
		return;
	}

	uint8_t note_on[3] = { 0x90, note_num, velocity };
	tud_midi_stream_write(0, note_on, sizeof(note_on));
}

void usb_midi_send_note_off(uint8_t note_num) {
	if (!tud_midi_mounted()) {
		return;
	}

	uint8_t note_off[3] = { 0x80, note_num, 0x00 };
	tud_midi_stream_write(0, note_off, sizeof(note_off));
}

void usb_keyboard_set_modifier(uint8_t modifier_mask, bool pressed) {
	if (pressed) {
		current_keyboard_modifier |= modifier_mask;
	} else {
		current_keyboard_modifier &= ~modifier_mask;
	}
	keyboard_report_pending = true;
	usb_keyboard_update();
}

void usb_keyboard_set_key(uint8_t keycode, bool pressed) {
	if (pressed) {
		for (size_t i = 0; i < 6; i++) {
			if (current_keyboard_keycodes[i] == keycode) {
				return;
			}
			if (current_keyboard_keycodes[i] == 0) {
				current_keyboard_keycodes[i] = keycode;
				break;
			}
		}
	} else {
		for (size_t i = 0; i < 6; i++) {
			if (current_keyboard_keycodes[i] == keycode) {
				current_keyboard_keycodes[i] = 0;
			}
		}
	}
	keyboard_report_pending = true;
	usb_keyboard_update();
}

void usb_keyboard_send_report(uint8_t modifier, uint8_t keycode) {
	current_keyboard_modifier = modifier;
	memset(current_keyboard_keycodes, 0, sizeof(current_keyboard_keycodes));
	current_keyboard_keycodes[0] = keycode;
	keyboard_report_pending = true;
	usb_keyboard_update();
}

void usb_keyboard_release(void) {
	current_keyboard_modifier = 0;
	memset(current_keyboard_keycodes, 0, sizeof(current_keyboard_keycodes));
	keyboard_report_pending = true;
	usb_keyboard_update();
}

//--------------------------------------------------------------------+
// TinyUSB Callbacks
//--------------------------------------------------------------------+

void tud_mount_cb(void) {
	usb_mounted = true;
	LOG(TAG, "mounted\r\n");
}

void tud_umount_cb(void) {
	usb_mounted = false;
	LOG(TAG, "unmounted\r\n");
}

void tud_suspend_cb(bool remote_wakeup_en) {
	(void) remote_wakeup_en;
	LOG(TAG, "suspended\r\n");
}

void tud_resume_cb(void) {
	LOG(TAG, "resumed\r\n");
}

uint16_t tud_hid_get_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen) {
	(void) itf;
	(void) report_id;
	(void) report_type;
	(void) buffer;
	(void) reqlen;
	return 0;
}

void tud_hid_set_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize) {
	(void) itf;
	(void) report_id;
	(void) report_type;
	(void) buffer;
	(void) bufsize;
}

void tud_hid_report_complete_cb(uint8_t itf, uint8_t const *report, uint16_t len) {
	(void) itf;
	(void) report;
	(void) len;
	usb_keyboard_update();
}

void tud_midi_rx_cb(uint8_t itf) {
	(void) itf;
	uint8_t packet[4];
	while (tud_midi_available()) {
		tud_midi_packet_read(packet);
	}
}
