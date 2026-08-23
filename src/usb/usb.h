#ifndef USB_H_
#define USB_H_

#include "main.h"

#define USB_KEYBOARD_MODIFIER_LEFTCTRL   (1 << 0)
#define USB_KEYBOARD_MODIFIER_RIGHTCTRL  (1 << 4)

#define USB_KEY_SPACE                    0x2C

void usb_init(void);
void usb_process(void);
bool usb_is_mounted(void);

void usb_midi_send_note_on(uint8_t note_num, uint8_t velocity);
void usb_midi_send_note_off(uint8_t note_num);

void usb_keyboard_set_modifier(uint8_t modifier_mask, bool pressed);
void usb_keyboard_set_key(uint8_t keycode, bool pressed);
void usb_keyboard_send_report(uint8_t modifier, uint8_t keycode);
void usb_keyboard_release(void);

#endif /* USB_H_ */
