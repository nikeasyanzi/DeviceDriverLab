#include <linux/init.h>
#include <linux/module.h>

#include <linux/fs.h> // chrdev
#include <linux/cdev.h> // cdev_add()/cdev_del()
#include <linux/semaphore.h> // up()/down_interruptible()
#include <linux/slab.h> 
#include <asm/uaccess.h> // copy_*_user()
#include <asm/ioctl.h> 
#include <asm/io.h> 

#include "ioc_brook.h"

MODULE_LICENSE("GPL");

#define DEV_BUFSIZE         1024


static int dev_major;
static int dev_minor;
struct cdev *dev_cdevp = NULL;

static int 
dev_open(struct inode*, struct file*);
static int 
dev_release(struct inode*, struct file*);
static int
dev_ioctl(struct inode*, struct file*, unsigned int, unsigned long);

static void __exit exit_modules(void);

struct file_operations dev_fops = {
	.owner   = THIS_MODULE,
	.open    = dev_open,
	.release = dev_release,
	.unlocked_ioctl=dev_ioctl

};

static int dev_open(struct inode *inode, struct file *filp)
{
	printk("%s():\n", __FUNCTION__);
	return 0;

}

static int dev_release(struct inode *inode, struct file *filp)
{
	printk("%s():\n", __FUNCTION__);
	return 0;

}

static int brook_num = 0;
	static int 
dev_ioctl(struct inode *inode, struct file *filp,
		unsigned int cmd, unsigned long args)
{
	int tmp, err = 0, ret = 0;

	if (_IOC_TYPE(cmd) != BROOK_IOC_MAGIC)
		return -ENOTTY;
	if (_IOC_NR(cmd) > BROOK_IOC_MAXNR)
		return -ENOTTY;

	if (_IOC_DIR(cmd) & _IOC_READ) {
		err = !access_ok(VERIFY_WRITE, (void __user*)args, _IOC_SIZE(cmd));

	} else if (_IOC_DIR(cmd) & _IOC_WRITE) {
		err = !access_ok(VERIFY_READ, (void __user *)args, _IOC_SIZE(cmd));

	}
	if (err)
		return -EFAULT;

	switch (cmd) {
		case BROOK_IOCSETNUM:
			// don't need call access_ok() again. using __get_user().
			ret = __get_user(brook_num, (int __user *)args); 
			printk("%s(): get val = %d\n", __FUNCTION__, brook_num);
			break;
		case BROOK_IOCGETNUM:
			ret = __put_user(brook_num, (int __user *)args);
			printk("%s(): set val to %d\n", __FUNCTION__, brook_num);
			break;
		case BROOK_IOCXNUM:
			tmp = brook_num;
			ret = __get_user(brook_num, (int __user *)args);
			if (!ret) {
				ret = __put_user(tmp, (int __user *)args);
			}
			printk("%s(): change val from %d to %d\n",
					__FUNCTION__, tmp, brook_num);
			break;
		default: /* redundant, as cmd was checked against MAXNR */
			return -ENOTTY;
	}
	return 0;
}

static int __init init_modules(void)
{
	dev_t dev;
	int ret;

	ret = alloc_chrdev_region(&dev, 0, 1, "brook");
	if (ret < 0) {
		printk("can't alloc chrdev\n");
		return ret;
	}
	dev_major = MAJOR(dev);
	dev_minor = MINOR(dev);
	printk("register chrdev(%d,%d)\n", dev_major, dev_minor);

	dev_cdevp = kmalloc(sizeof(struct cdev), GFP_KERNEL);
	if (dev_cdevp == NULL) {
		printk("kmalloc failed\n");
		goto failed;
	}
	cdev_init(dev_cdevp, &dev_fops);
	dev_cdevp->owner = THIS_MODULE;
	ret = cdev_add(dev_cdevp, MKDEV(dev_major, dev_minor), 1);
	if (ret < 0) {
		printk("add chr dev failed\n");
		goto failed;
	}

	return 0;

failed:
	if (dev_cdevp) {
		kfree(dev_cdevp);
		dev_cdevp = NULL;
	}
	return 0;
}

static void __exit exit_modules(void)
{
	dev_t dev;

	dev = MKDEV(dev_major, dev_minor);
	if (dev_cdevp) {
		cdev_del(dev_cdevp);
		kfree(dev_cdevp);
	}
	unregister_chrdev_region(dev, 1);
	printk("unregister chrdev\n");
}

module_init(init_modules);
module_exit(exit_modules);
