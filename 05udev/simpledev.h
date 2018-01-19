
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/fcntl.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>        /* for put_user */
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/semaphore.h>

#define BUF_LEN 80  /* Max length of the message from the device */
#define DEVICE_NAME "simplechardev"   /* Dev name as it appears in /proc/devices   */

struct my_char_dev{
	struct cdev mycdev;
	struct semaphore sem;

	char msg[BUF_LEN];       /* The msg the device will give when asked */
	int dev_open_counter;     /* Is device open*/
	dev_t dev;
};



