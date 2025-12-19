#include <linux/module.h>
#include <linux/usb.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define USB_PICO_VID 0x2e8a
#define USB_PICO_PID 0x000a

static struct usb_device *pico_dev;

static int pico_open(struct inode *i, struct file *f) {
    pr_info("pico_usb: open\n");
    return 0;
}

static ssize_t pico_read(struct file *f, char __user *buf, size_t len, loff_t *off) {
    char msg[] = "dados da Pico\n";
    return simple_read_from_buffer(buf, len, off, msg, sizeof(msg));
}

static struct file_operations pico_fops = {
    .owner = THIS_MODULE,
    .open  = pico_open,
    .read  = pico_read,
};

static int pico_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
    pico_dev = interface_to_usbdev(intf);
    pr_info("pico_usb: Pico conectada!\n");

    register_chrdev(240, "pico_usb", &pico_fops);
    pr_info("pico_usb: /dev/pico_usb criado\n");

    return 0;
}

static void pico_disconnect(struct usb_interface *intf)
{
    unregister_chrdev(240, "pico_usb");
    pr_info("pico_usb: Pico desconectada!\n");
}

static const struct usb_device_id pico_table[] = {
    { USB_DEVICE(USB_PICO_VID, USB_PICO_PID) },
    {}
};

MODULE_DEVICE_TABLE(usb, pico_table);

static struct usb_driver pico_driver = {
    .name = "pico_usb",
    .probe = pico_probe,
    .disconnect = pico_disconnect,
    .id_table = pico_table,
};

module_usb_driver(pico_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Monica&Renan");
MODULE_DESCRIPTION("Driver USB Vendor-Specific para Raspberry Pi Pico");
