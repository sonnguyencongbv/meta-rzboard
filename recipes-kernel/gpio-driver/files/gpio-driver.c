#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <asm/io.h>

#define DRIVER_AUTHOR	"Vu Dang <vu.dang.te@renesas.com> - SonNguyen refer to avnet board"
#define DRIVER_DESC   	"A sample GPIO driver - Using to control gpio P48_3 on avnet rzboard"
#define DRIVER_VERSION 	"0.1"
 
#define DEVICE_NAME     "GPIO_P48_3"
#define PIN_ON          1
#define PIN_OFF         0


#define GPIO_BASE	0x11030000

#define PMC   		0x0240 			// PMC40
#define	PM   		0x0180			// PM40
#define P      		0x0040			// P40

#define PM40_BIT_76     (2<<6)		// bit 76 = 10 -> P48_3 is OPUTPUT disable input.
#define P48_3_HIGH      (1<<3)		// bit 3 = 1 -> P48_3 = HIGH, bit 3 = 0 -> P48_3 = LOW


#define REG_SIZE 	2		// 16-bit register

/* Prototypes of four basic operations: open, close, read, write */
static int gpio_open(struct inode *inode, struct file *file);
static int gpio_release(struct inode *inode, struct file *file);
static ssize_t gpio_read(struct file *file, char *buf, size_t count, loff_t *ppos);
static ssize_t gpio_write(struct file *file, const char *buf, size_t count, loff_t *ppos);

/* file operations structure */
static struct file_operations gpio_fops = {
	.owner      = THIS_MODULE,
	.open       = gpio_open,
	.release    = gpio_release,
	.read       = gpio_read,
        .write      = gpio_write,
};

/* gpio device struct */
static struct gpio_device {
        int status;                     // on/off or 1/0
        struct cdev cdev;               // character device file
} gpio_dev;                             // gpio device
 
/* gpio driver struct */
static struct _my_gpio_driver {
        dev_t gpio_dev_number;
	struct class *dev_class;
	struct device *dev;
} my_gpio_driver;

/**/
/* initialize gpio port p48_3 */
static void initPort(void)
{
	u8 temp;
	/* map physical address into kernel address space */
	void __iomem *pmc3b 	= ioremap_cache(GPIO_BASE + PMC, REG_SIZE);			
    void __iomem *pm3b 		= ioremap_cache(GPIO_BASE + PM, REG_SIZE);
	
	/* read and write to PMC */
	temp = __raw_readb(pmc3b);
	temp &= ~(P48_3_HIGH);
	__raw_writeb(temp, pmc3b);
	
	/* read and write to PM */
	temp = __raw_readb(pm3b);
	temp |= (PM40_BIT_76);
	__raw_writeb(temp, pm3b);
	
	/* unmap the register above*/
	iounmap(pmc3b);
	iounmap(pm3b);
}

/* change status */
static void changeStatus(int status)
{
	u8 temp;
	/* map register and read for gpio p48_3 */
	void __iomem *p3b = ioremap_cache(GPIO_BASE + P, REG_SIZE);
	temp = __raw_readb(p3b);
		
	/* change status */
	switch (status) 
	{
		case PIN_OFF:
			temp &= ~P48_3_HIGH;
			break;
		case PIN_ON:
			temp |= P48_3_HIGH;
			break;
	}
		
	/* write status to P */
	__raw_writeb(temp, p3b);
		
	/* unmap register */
	iounmap(p3b);
}

/* entry point function */
/* open gpio file */
static int gpio_open(struct inode *inode, struct file *file)
{
    	struct gpio_device *gpio_devp;
    	/* get cdev struct */
    	gpio_devp = container_of(inode->i_cdev, struct gpio_device, cdev);
    
    	/* save cdev pointer */
    	file->private_data = gpio_devp;
		
	printk("Handle opened event\n");
    	return 0;
}

/* close gpio file */
static int gpio_release(struct inode *inode, struct file *file)
{
    	printk("Handle closed event\n");
    	return 0;
}

/* read gpio status */
static ssize_t gpio_read(struct file *file, char *buf, size_t count, loff_t *ppos)
{
	struct gpio_device *gpio_devp = file->private_data;
	
	if(*ppos > 0)
		return 0;
		
    	if (gpio_devp->status == PIN_ON) {
		if (copy_to_user(buf, "1", 1))
           		return -EIO;
    	}
    	else {
        	if (copy_to_user(buf, "0", 1))
           		return -EIO;
    	}
	*ppos = 1;
    	return 1;
}

/* write gpio status */
static ssize_t gpio_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
    	struct gpio_device *gpio_devp = file->private_data;
    	char kbuf = 0;  
    	
	if (copy_from_user(&kbuf, buf, 1)) {
		return -EFAULT;
    	}
    
    	if (kbuf == '1') {
       		changeStatus(PIN_ON);
       		gpio_devp->status = PIN_ON;
    	}
    	else if (kbuf == '0') {
       		changeStatus(PIN_OFF);
       		gpio_devp->status = PIN_OFF;
    	}	
    	return count;
}

/* driver initialization */
int __init my_gpio_init(void)
{
	int ret;
 
	/* request a device major number and check */
	ret = alloc_chrdev_region(&my_gpio_driver.gpio_dev_number, 0, 1, DEVICE_NAME);
	if ( ret < 0) {
		printk(KERN_DEBUG "Error registering device!\n");
		return ret;
	}
	
	/* create class for device */
	my_gpio_driver.dev_class = class_create(THIS_MODULE, "my_gpio");
	if (my_gpio_driver.dev_class == NULL) {
		unregister_chrdev_region(my_gpio_driver.gpio_dev_number, 1);
		return -1;
	}
		
	/* init GPIO port */
	initPort();
 
	/* init gpio device */
	gpio_dev.status = PIN_OFF;
		
	/* create device in class and check */
	my_gpio_driver.dev = device_create(my_gpio_driver.dev_class, NULL, my_gpio_driver.gpio_dev_number, NULL, "gpio_p48_3");		
	if (my_gpio_driver.dev == NULL) {	
		class_destroy(my_gpio_driver.dev_class);
		unregister_chrdev_region(my_gpio_driver.gpio_dev_number, 1);
		return -1;
	}
		
	/* connect file operations to this device */
	cdev_init(&gpio_dev.cdev, &gpio_fops);
	gpio_dev.cdev.owner = THIS_MODULE;
 
	/* connect major/minor numbers and check */
	ret = cdev_add(&gpio_dev.cdev, my_gpio_driver.gpio_dev_number, 1);
	if (ret) {
		printk(KERN_DEBUG "Error adding device!\n");
		return ret;
	}
 
	/* init gpio status - low */
	changeStatus(PIN_OFF);
	
	printk("gpio driver initialized.\n");
	return 0;
}
 
/* driver exit */
void __exit my_gpio_exit(void)
{
 
	/* delete device */
	cdev_del(&gpio_dev.cdev);
		
	/*destroy device file*/
	device_destroy(my_gpio_driver.dev_class, my_gpio_driver.gpio_dev_number);
	
	/*destroy device class*/
	class_destroy(my_gpio_driver.dev_class);
	
	/* release major number */
	unregister_chrdev_region(my_gpio_driver.gpio_dev_number, 1);
 
	printk("Exiting gpio driver.\n");
}
 
module_init(my_gpio_init);
module_exit(my_gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERSION);
MODULE_SUPPORTED_DEVICE("GPIO device");
