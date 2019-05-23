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

#define CSEMAD_DOT_MATRIX_MAJOR 262
#define CSEMAD_DOT_MATRIX_NAME "csemad_dot_matrix"
#define CSEMAD_DOT_MATRIX_ADDRESS 0x08000210

// Global variable
static int DotMatrixPortUsage = 0;
static unsigned char * DotMatrixAddress;

// Define function
ssize_t csemad_dot_matrix_write(struct file * inode, const char * gdata, size_t length, loff_t * off_what);
int csemad_dot_matrix_open(struct inode * minode, struct file * mfile);
int csemad_dot_matrix_release(struct inode * minode, struct file * mfile);

// Define file_operations structure
struct file_operations csemad_dot_matrix_fops =
{
    .owner = THIS_MODULE,
    .open = csemad_dot_matrix_open,
    .write = csemad_dot_matrix_write,
    .release = csemad_dot_matrix_release,
};

// Initialize DotMatrix port
int __init csemad_dot_matrix_init(void)
{
    int result = register_chrdev(CSEMAD_DOT_MATRIX_MAJOR, CSEMAD_DOT_MATRIX_NAME, &csemad_dot_matrix_fops);

    if (result != 0)
    {
        printk(KERN_ALERT "Failed to init module, result: '%d'\n", result);
        return result;
    }

    DotMatrixAddress = ioremap(CSEMAD_DOT_MATRIX_ADDRESS, 1);
    printk(KERN_ALERT "Initialize DotMatrix port\n");
    return 0;
}

// Release DotMatrix port
void __exit csemad_dot_matrix_exit(void)
{
    iounmap(DotMatrixAddress);
    unregister_chrdev(CSEMAD_DOT_MATRIX_MAJOR, CSEMAD_DOT_MATRIX_NAME);
    printk(KERN_ALERT "Release DotMatrix port\n");
}

// Open DotMatrix port
int csemad_dot_matrix_open(struct inode * minode, struct file * mfile)
{
    if (DotMatrixPortUsage != 0)
    {
        printk(KERN_ALERT "DotMatrix port is busy\n");
        return -EBUSY;
    }

    DotMatrixPortUsage = 1;
    //printk(KERN_ALERT "Open DotMatrix port\n");
    return 0;
}

// Close DotMatrix port
int csemad_dot_matrix_release(struct inode * minode, struct file * mfile)
{
    DotMatrixPortUsage = 0;
    //printk(KERN_ALERT "Close DotMatrix port/n");
    return 0;
}

// Write to DotMatrix
ssize_t csemad_dot_matrix_write(struct file * inode, const char * gdata, size_t length, loff_t * off_what)
{
    const int MATRIX_SIZE = 10;

    int index;
    char buffer[MATRIX_SIZE];

    if (copy_from_user(buffer, gdata, length) != 0)
    {
        printk(KERN_ALERT "Failed to write DotMatrix\n");
        return 0;
    }

    for (index = 0; index < length; index++)
    {
        outb(buffer[index], (unsigned int)DotMatrixAddress + index * 2);
    }

    //printk(KERN_ALERT "Write to DotMatrix\n");
    return length;
}

module_init(csemad_dot_matrix_init);
module_exit(csemad_dot_matrix_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CSEMAD");
