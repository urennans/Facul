#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

#define USB_PICO_VENDOR_ID   0x2e8a
#define USB_PICO_PRODUCT_ID  0x000a

static int pico_probe(struct usb_interface *interface,
                      const struct usb_device_id *id)
{
    struct usb_host_interface *iface_desc;

    iface_desc = interface->cur_altsetting;

    /* Aceita SOMENTE interface vendor-specific */
    if (iface_desc->desc.bInterfaceClass != USB_CLASS_VENDOR_SPEC) {
        pr_info("pico_usb: ignorando interface %d (classe %02x)\n",
                iface_desc->desc.bInterfaceNumber,
                iface_desc->desc.bInterfaceClass);
        return -ENODEV;
    }

    pr_info("pico_usb: Pico conectada (Ta funcionando Gente!)\n");
    return 0;
}

static void pico_disconnect(struct usb_interface *interface)
{
    pr_info("pico_usb: Pico desconectada\n");
}

static const struct usb_device_id pico_table[] = {
    { USB_DEVICE(USB_PICO_VENDOR_ID, USB_PICO_PRODUCT_ID) },
    { }
};
MODULE_DEVICE_TABLE(usb, pico_table);

static struct usb_driver pico_usb_driver = {
    .name       = "pico_usb",
    .probe      = pico_probe,
    .disconnect = pico_disconnect,
    .id_table   = pico_table,
};

static int __init pico_init(void)
{
    pr_info("pico_usb: registrando driver USB\n");
    return usb_register(&pico_usb_driver);
}

static void __exit pico_exit(void)
{
    usb_deregister(&pico_usb_driver);
    pr_info("pico_usb: driver removido\n");
}

module_init(pico_init);
module_exit(pico_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Monica&Renan");
MODULE_DESCRIPTION("Driver USB para Raspberry Pi Pico (Vendor Specific)");
