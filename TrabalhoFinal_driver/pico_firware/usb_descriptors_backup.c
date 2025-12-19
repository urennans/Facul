#include "tusb.h"

// IDs - DEVE ser igual ao código principal
#define USB_VID 0x1209
#define USB_PID 0x0001

tusb_desc_device_t const desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,
    .bDeviceClass       = TUSB_CLASS_VENDOR_SPECIFIC,
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = 0x00,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor           = USB_VID,
    .idProduct          = USB_PID,
    .bcdDevice          = 0x0100,
    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,
    .bNumConfigurations = 0x01
};

uint8_t const desc_configuration[] = {
    // Configuração
    9, TUSB_DESC_CONFIGURATION, 32, 0, 1, 1, 0, 0x80, 100,
    
    // Interface Vendor Specific
    9, TUSB_DESC_INTERFACE, 0, 0, 2, TUSB_CLASS_VENDOR_SPECIFIC, 0, 0, 0,
    
    // Endpoint IN
    7, TUSB_DESC_ENDPOINT, 0x81, TUSB_XFER_BULK, 64, 0,
    
    // Endpoint OUT
    7, TUSB_DESC_ENDPOINT, 0x01, TUSB_XFER_BULK, 64, 0
};

// Strings em UNICODE (UTF-16)
static const uint16_t string_desc_arr[][32] = {
    // 0: Language ID (English)
    { (3 << 8) | 9, 0x0409 },
    
    // 1: Manufacturer
    {
        0x0012, // Length (18 bytes)
        'R', 'a', 's', 'p', 'b', 'e', 'r', 'r', 'y', ' ', 'P', 'i'
    },
    
    // 2: Product
    {
        0x001E, // Length (30 bytes)
        'P', 'i', 'c', 'o', ' ', 'U', 'S', 'B', ' ', 'D', 'e', 'v', 'i', 'c', 'e'
    },
    
    // 3: Serial Number
    {
        0x000E, // Length (14 bytes)
        '1', '2', '3', '4', '5', '6'
    }
};

// Callback para strings - CORRIGIDO: retorna uint16_t const*
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    (void) langid;
    
    if (index == 0) {
        // Language ID
        static uint16_t lang_id[] = { 0x0309, 0x0409 }; // US English
        return lang_id;
    }
    
    if (index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0])) {
        return string_desc_arr[index];
    }
    
    return NULL;
}

// Callback para descriptor do dispositivo
uint8_t const* tud_descriptor_device_cb(void) {
    return (uint8_t const*)&desc_device;
}

// Callback para descriptor de configuração
uint8_t const* tud_descriptor_configuration_cb(uint8_t index) {
    (void) index;
    return desc_configuration;
}