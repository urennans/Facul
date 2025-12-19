#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

// USB device mode
#define CFG_TUSB_MCU OPT_MCU_RP2040
#define CFG_TUSB_RHPORT0_MODE (OPT_MODE_DEVICE | OPT_MODE_FULL_SPEED)

// Device configuration
#define CFG_TUD_ENDPOINT0_SIZE 64

// Enable Vendor class
#define CFG_TUD_VENDOR 1
#define CFG_TUD_CDC    0
#define CFG_TUD_MSC    0
#define CFG_TUD_HID    0

// Buffer sizes
#define CFG_TUD_VENDOR_RX_BUFSIZE 64
#define CFG_TUD_VENDOR_TX_BUFSIZE 64

#endif // _TUSB_CONFIG_H_
