
/*
 *  chardev.c: Creates a read−only char device that says how many times you've read from the dev file
 *    */

#include "simpledev.h"
#define SUCCESS 0

/*
 *   *  Prototypes − this would normally go in a .h file
  *    */
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

static int Major;               /* Major number assigned to our device driver */

dev_t dev;
struct my_char_dev *mychardev;
static struct class *charmodule_class;
static unsigned int num_of_dev=2;

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};
/*
*  * This function is called when the module is loaded
*   */

int  init_module(void) {
    int ret,cdev_ret;
    int counter;
    char tmp[512];
    printk("dev_init\n");

    mychardev=kmalloc( sizeof(struct my_char_dev) * num_of_dev, GFP_KERNEL );

    ret=alloc_chrdev_region(&mychardev[0].dev,0, num_of_dev,DEVICE_NAME);
    if (ret<0)
    {
        printk(KERN_ALERT "Registering char device failed\n");
        return -EFAULT;
    } 

	Major=MAJOR(mychardev[0].dev);

    for(counter=0; counter < num_of_dev; counter++) {
        cdev_init( &mychardev[counter].mycdev, &fops);
        
        dev=MKDEV(Major, MINOR(dev)+counter);
        mychardev[counter].dev_open_counter=0;
        sema_init(&mychardev[counter].sem,1);

    	cdev_ret=cdev_add( &mychardev[counter].mycdev, dev,1);
    }

    if(cdev_ret)
        goto cdev_error;

    printk(KERN_ALERT "cdev is added\n");

    charmodule_class=class_create(THIS_MODULE, DEVICE_NAME);

    if (IS_ERR(charmodule_class))
        goto class_create_error;
    printk(KERN_ALERT "%s class is created\n",DEVICE_NAME);
    for(counter=0; counter < num_of_dev; counter++) {
        memset(tmp,'\0',sizeof(tmp));
        //      sprintf(tmp,"%s",DEVICE_NAME);
        sprintf(tmp,"%s%d",DEVICE_NAME,counter);
        //    sprintf(&tmpchar,"%d",counter);
        //  strcat(tmp,&tmpchar);
        printk("device create %s\n",tmp);
        device_create(charmodule_class, NULL, MKDEV(Major,counter) , NULL,tmp);
    } 

    printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
    printk(KERN_INFO "the driver, create a dev file at /dev/%s \n", DEVICE_NAME);
    printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
    printk(KERN_INFO "the device file.\n");
    printk(KERN_INFO "Remove the device file and module when done.\n");
    return SUCCESS;


class_create_error:
    for(counter=0; counter < num_of_dev; counter++) {
        cdev_del(&mychardev[counter].mycdev);
     } 
 
    kfree(mychardev);

cdev_error:
    unregister_chrdev_region(dev,num_of_dev);

    return -EFAULT;
}

/* 
*  * This function is called when the module is unloaded
*    */
void cleanup_module(void)
{ 
    int counter;

    for(counter=0; counter < num_of_dev; counter++) {
        cdev_del(&(mychardev[counter].mycdev));
      }

    kfree(mychardev);

    for(counter=0; counter < num_of_dev; counter++) {
        device_destroy(charmodule_class,MKDEV(Major, counter));
      }
    class_destroy(charmodule_class);
    unregister_chrdev_region(dev,num_of_dev);

    printk(KERN_ALERT "%s driver removed.\n", DEVICE_NAME);

} 
/*
*  * Called when a process tries to open the device file, like
*   * "cat /dev/mycharfile"
*    */
static int device_open(struct inode *inode, struct file *file)
{ 
    static int counter = 0;
    struct my_char_dev *dev=file->private_data;
    printk("dev_open\n");

    dev=container_of(inode->i_cdev,struct my_char_dev,mycdev );
    file->private_data=dev;
      if (dev ==NULL) {
        printk(KERN_INFO "dev == NULL \n");
        return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
    }

    down_interruptible(&(dev->sem));
    if(dev->dev_open_counter)		return -EBUSY;
    dev->dev_open_counter++;

    printk("increase device open counter \n");
    sprintf(dev->msg, "I already told you %d times Hello world!\n", counter++);
    try_module_get(THIS_MODULE);

    up(&(dev->sem));
    return SUCCESS;
}  
/*
 *  * Called when a process closes the device file.
 *      */
static int device_release(struct inode *inode, struct file *file)
{   
    struct my_char_dev *dev=file->private_data;
    dev=container_of(inode->i_cdev,struct my_char_dev,mycdev );
    printk("dev_release\n");
    down_interruptible(&(dev->sem));
    dev->dev_open_counter--; /* We're now ready for our next caller */
    /*
     * Decrement the usage count, or else once you opened the file, you'll
     * never get get rid of the module.
         *                            */
    module_put(THIS_MODULE);
    up(&(dev->sem));
    printk("decrease device open counter \n");

	return 0;
} 
/*  
 *  * Called when a process, which already opened the dev file, attempts to
 * read from it.
 *  */

static ssize_t device_read(struct file *filp,   /* see include/linux/fs.h   */
                           char *buffer,        /* buffer to fill with data */
                           size_t length,       /* length of the buffer     */
                           loff_t * offset)
{

    unsigned int error_count;
    struct my_char_dev *dev=filp->private_data;
    printk("dev_read\n");
   // down_interruptible(&(dev->sem));
    if (dev ==NULL) {
        printk(KERN_INFO "dev == NULL \n");
        return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
    }
    error_count = copy_to_user(buffer,dev->msg, BUF_LEN);
     if (error_count==0) {           // if true then have success
        printk(KERN_INFO "characters sent to the user\n");
        return 0;  // clear the position to the start and return 0
    }
     else {
        printk(KERN_INFO "Failed to send %d characters to the user\n", error_count);
        return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
    }
    //up(&(dev->sem));

} 
/*
 *   * Called when a process writes to dev file: echo "hi" > /dev/hello
 *    */
static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * off) {
    printk("dev_write\n");

    struct my_char_dev *dev=filp->private_data;
//	 down_interruptible(&(dev->sem));
    
    sprintf(dev->msg, "%s", buff, BUF_LEN);   // appending received string with its length
    printk(KERN_INFO "Received %zu characters from the user\n", len);
   // up(&(dev->sem));
    return len;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Craig Yang");	/* Who wrote this module? */
MODULE_DESCRIPTION("udev");	/* What does this module do */
//module_init(init_module);
//module_exit(cleanup_module);

