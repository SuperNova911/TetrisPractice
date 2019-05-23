#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/version.h>

#include <asm/io.h>
#include <asm/uaccess.h>

#define CSEMAD_PUSH_SWITCH_MAJOR 265
#define CSEMAD_PUSH_SWITCH_NAME "csemad_push_switch"
#define CSEMAD_PUSH_SWITCH_ADDRESS 0x08000050

// Global variable
static int SwitchPortUsage = 0;
static unsigned char * CsemadPushSwitchAddr;

// Define function
ssize_t csemad_push_switch_read(struct file * inode, char * gdata, size_t length, loff_t * off_what);
int csemad_push_switch_open(struct inode * minode, struct file * mfile);
int csemad_push_switch_release(struct inode * minode, struct file * mfile);

// Define file_operations structure
struct file_operations csemad_push_switch_fops =
{
    .owner = THIS_MODULE,
    .open = csemad_push_switch_open,
    .read = csemad_push_switch_read,
    .release = csemad_push_switch_release,
};

// Initialize PushSwitch port
int __init csemad_push_switch_init(void)
{
    int result = register_chrdev(CSEMAD_PUSH_SWITCH_MAJOR, CSEMAD_PUSH_SWITCH_NAME, &csemad_push_switch_fops);

    if (result != 0)
    {
        printk(KERN_ALERT "Failed to init module, result: '%d'\n", result);
        return result;
    }

    CsemadPushSwitchAddr = ioremap(CSEMAD_PUSH_SWITCH_ADDRESS, 1);
    printk(KERN_ALERT "Initialize PushSwitch port\n");
    return 0;
}

// Release PushSwitch port
void __exit csemad_push_switch_exit(void)
{
    iounmap(CsemadPushSwitchAddr);
    unregister_chrdev(CSEMAD_PUSH_SWITCH_MAJOR, CSEMAD_PUSH_SWITCH_NAME);
    printk(KERN_ALERT "Release PushSwitch port\n");
}

// Open PushSwitch port
int csemad_push_switch_open(struct inode * minode, struct file * mfile)
{
    if (SwitchPortUsage != 0)
    {
        printk(KERN_ALERT "PushSwitch port is busy\n");
        return -EBUSY;
    }

    SwitchPortUsage = 1;
    //printk(KERN_ALERT "Open PushSwitch port\n");
    return 0;
}

// Close PushSwitch port
int csemad_push_switch_release(struct inode * minode, struct file * mfile)
{
    SwitchPortUsage = 0;
    //printk(KERN_ALERT "Close PushSwitch port/n");
    return 0;
}

// Read from PushSwitch port
ssize_t csemad_push_switch_read(struct file * inode, char * gdata, size_t length, loff_t * off_what)
{
    const int buttonNumber = 9;

    int index;
    unsigned char buffer[buttonNumber];

    for (index = 0; index < buttonNumber; index++)
    {
        buffer[index] = inb((unsigned int)CsemadPushSwitchAddr + index * 2);
    }

    if (copy_to_user(gdata, buffer, length) != 0)
    {
        printk(KERN_ALERT "Failed to read PushSwitch");
        return -EFAULT;
    }

    //printk(KERN_ALERT "Read from PushSwitch port\n");
    return length;
}

module_init(csemad_push_switch_init);
module_exit(csemad_push_switch_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CSEMAD");
