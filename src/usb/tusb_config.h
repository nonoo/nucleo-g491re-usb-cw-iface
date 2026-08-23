#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

	//--------------------------------------------------------------------+
	// Board Specific Configuration
	//--------------------------------------------------------------------+
#define CFG_TUSB_MCU OPT_MCU_STM32G4
#define CFG_TUSB_RHPORT0_MODE (OPT_MODE_DEVICE | OPT_MODE_FULL_SPEED)
#define CFG_TUSB_OS OPT_OS_NONE

#ifndef CFG_TUSB_DEBUG
#define CFG_TUSB_DEBUG 0
#endif

//--------------------------------------------------------------------+
// Device Configuration
//--------------------------------------------------------------------+
#define CFG_TUD_ENDPOINT0_SIZE 64

// Class drivers
#define CFG_TUD_CDC 0
#define CFG_TUD_MSC 0
#define CFG_TUD_HID 1
#define CFG_TUD_MIDI 1
#define CFG_TUD_VENDOR 0

// HID configuration
#define CFG_TUD_HID_EP_BUFSIZE 16

// MIDI configuration
#define CFG_TUD_MIDI_EP_BUFSIZE 64
#define CFG_TUD_MIDI_RX_BUFSIZE 64
#define CFG_TUD_MIDI_TX_BUFSIZE 64

#ifdef __cplusplus
}
#endif

#endif /* _TUSB_CONFIG_H_ */
