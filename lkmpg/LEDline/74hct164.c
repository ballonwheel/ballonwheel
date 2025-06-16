


/*
## 🚀 Build & Use

1. **Build the driver:**

   ```bash
   make
   ```

2. **Load the module:**

   ```bash
   sudo insmod 74hct164.ko
   dmesg | tail
   ```
3. **Use the driver:**

   ```bash
   echo -ne "\x80" > /dev/74hct164  # Light up first LED
   ```

4. **Unload it:**

   ```bash
   sudo rmmod 74hct164
   ```




*/



#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/delay.h>

#define DEVICE_NAME "74hct164"
#define CLASS_NAME "shiftreg"

static int major;
static struct class *shiftreg_class;
static struct cdev shiftreg_cdev;

#define GPIO_CLK  27
#define GPIO_DATA 17

static int shiftreg_open(struct inode *inode, struct file *file) {
    return 0;
}

static int shiftreg_release(struct inode *inode, struct file *file) {
    return 0;
}

static void pulse_clock(void) {
    gpio_set_value(GPIO_CLK, 1);
    ndelay(500);
    gpio_set_value(GPIO_CLK, 0);
    ndelay(500);
}

#if 0
static ssize_t shiftreg_write(struct file *file, const char __user *buf, size_t len, loff_t *off) {
    uint8_t byte;
    int i;

    while (len--) {
        if (copy_from_user(&byte, buf++, 1))
            return -EFAULT;

        for (i = 7; i >= 0; i--) {
            gpio_set_value(GPIO_DATA, (byte >> i) & 1);
            pulse_clock();
        }
    }

    return 0;
}
#else
static ssize_t shiftreg_write(struct file *file, const char __user *buf, size_t len, loff_t *ppos)
{
    uint64_t pattern;
    int i;

    if (len < sizeof(pattern))
        return -EINVAL;

    if (copy_from_user(&pattern, buf, sizeof(pattern)))
        return -EFAULT;

    // Shift out 64 bits, MSB first
    for (i = 63; i >= 0; i--) {
        int bit = (pattern >> i) & 1;
        gpio_set_value(GPIO_DATA, bit);
        pulse_clock(); // Clock the bit into the shift register
    }

    return sizeof(pattern);
}
#endif


static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = shiftreg_open,
    .release = shiftreg_release,
    .write = shiftreg_write,
};

static int __init shiftreg_init(void) {
    int err;

    // Request GPIOs
    gpio_request(GPIO_CLK, "clk");
    gpio_request(GPIO_DATA, "data");
    gpio_direction_output(GPIO_CLK, 0);
    gpio_direction_output(GPIO_DATA, 0);

    // Allocate major number
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) return major;

    shiftreg_class = class_create(THIS_MODULE, CLASS_NAME);
    device_create(shiftreg_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);

    pr_info("74HCT164 driver loaded: /dev/%s\n", DEVICE_NAME);
    return 0;
}

static void __exit shiftreg_exit(void) {
    device_destroy(shiftreg_class, MKDEV(major, 0));
    class_destroy(shiftreg_class);
    unregister_chrdev(major, DEVICE_NAME);
    gpio_free(GPIO_CLK);
    gpio_free(GPIO_DATA);
    pr_info("74HCT164 driver unloaded\n");
}

module_init(shiftreg_init);
module_exit(shiftreg_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DobanyByChatGPT");
MODULE_DESCRIPTION("Character device driver for 74HCT164 shift register LED control");

