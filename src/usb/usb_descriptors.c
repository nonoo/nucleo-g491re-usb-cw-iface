#include "usb_descriptors.h"

//--------------------------------------------------------------------+
// Device Descriptors
//--------------------------------------------------------------------+
tusb_desc_device_t const desc_device = {
	.bLength = sizeof(tusb_desc_device_t),
	.bDescriptorType = TUSB_DESC_DEVICE,
	.bcdUSB = 0x0200,
	.bDeviceClass = 0x00,
	.bDeviceSubClass = 0x00,
	.bDeviceProtocol = 0x00,
	.bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,

	.idVendor = 0xCafe,
	.idProduct = 0x4000,
	.bcdDevice = 0x0100,

	.iManufacturer = STRID_MANUFACTURER,
	.iProduct = STRID_PRODUCT,
	.iSerialNumber = STRID_SERIAL,

	.bNumConfigurations = 0x01
};

uint8_t const *tud_descriptor_device_cb(void) {
	return (uint8_t const *)&desc_device;
}

//--------------------------------------------------------------------+
// HID Report Descriptor
//--------------------------------------------------------------------+
uint8_t const desc_hid_report[] = {
	TUD_HID_REPORT_DESC_KEYBOARD()
};

uint8_t const *tud_hid_descriptor_report_cb(uint8_t itf) {
	(void)itf;
	return desc_hid_report;
}

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+
#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN + TUD_MIDI_DESC_LEN)

uint8_t const desc_fs_configuration[] = {
	// Config number, interface count, string index, total length, attribute, power in mA
	TUD_CONFIG_DESCRIPTOR(1, ITF_TOTAL, 0, CONFIG_TOTAL_LEN, 0, 100),

	// Interface number, string index, protocol, report descriptor len, EP In address, size & polling interval
	TUD_HID_DESCRIPTOR(ITF_NUM_KEYBOARD, STRID_KEYBOARD, HID_ITF_PROTOCOL_KEYBOARD, sizeof(desc_hid_report), EPNUM_KEYBOARD, CFG_TUD_HID_EP_BUFSIZE, 10),

	// Interface number, string index, EP Out & EP In address, EP size
	TUD_MIDI_DESCRIPTOR(ITF_NUM_MIDI, STRID_MIDI, EPNUM_MIDI_OUT, EPNUM_MIDI_IN, CFG_TUD_MIDI_EP_BUFSIZE)
};

uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
	(void)index;
	return desc_fs_configuration;
}

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+
static char serial_str[25] = "000000000000000000000000";

static void generate_serial_number(void) {
	uint32_t *uid = (uint32_t *)UID_BASE;
	snprintf(serial_str, sizeof(serial_str), "%08lX%08lX%08lX", uid[0], uid[1], uid[2]);
}

static char const *string_desc_arr[] = {
	(const char[]) {
 0x09, 0x04
}, // 0: English (0x0409)
"HA2NON",                      // 1: Manufacturer
"Nucleo-G491RE CW Interface",  // 2: Product
serial_str,                    // 3: Serial
"HID Keyboard",                // 4: Interface 0
"MIDI Interface"               // 5: Interface 1 & 2
};

static uint16_t _desc_str[64];

uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
	(void)langid;
	uint8_t chr_count;

	if (index == STRID_LANGID) {
		memcpy(&_desc_str[1], string_desc_arr[0], 2);
		chr_count = 1;
	} else {
		if (index == STRID_SERIAL) {
			generate_serial_number();
		}

		if (index >= (sizeof(string_desc_arr) / sizeof(string_desc_arr[0]))) {
			return NULL;
		}

		const char *str = string_desc_arr[index];
		chr_count = (uint8_t)strlen(str);
		if (chr_count > 63) {
			chr_count = 63;
		}

		for (uint8_t i = 0; i < chr_count; i++) {
			_desc_str[1 + i] = str[i];
		}
	}

	// first byte is length (including header), second byte is string type
	_desc_str[0] = (uint16_t)((TUSB_DESC_STRING << 8) | (2 * chr_count + 2));

	return _desc_str;
}
