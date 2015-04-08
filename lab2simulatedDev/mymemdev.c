#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include "mymemdev.h"
//#include "proc.c"
struct MYMEMDEV *mymem_dev;
static int mymem_major = 0;

static int mymem_open(struct inode *inode,struct file *filp){
	/*struct MYMEMDEV *dev;
    int num = MINOR(inode->i_rdev);
    if (num >= MEMDEV_NR_DEVS) 
            return -ENODEV;
*/  
	filp->private_data=mymem_dev;
	return 0;
}  

static int mymem_release(struct inode *inode, struct file *filp){
	return 0;
} 
static ssize_t mymem_read(struct file *filp,char __user *buf,size_t size,loff_t *ppos){
	unsigned long p=*ppos;
	unsigned int count=size;
	MYDEBUG("unsigned p=%u, size=%u, count=%u =============\n",p,size,count);
	MYDEBUG("signed p=%d, size=%d, count=%d =============\n",p,size,count);
	int ret=0;
	MYDEBUG("=========read initial============\n");
	struct MYMEMDEV *dev=filp->private_data;
 	if(dev==NULL){
		MYDEBUG("==========dev = NULL=============\n");
	}

	if(p>=DATA_SIZE)
		return 0;
	MYDEBUG(" p>data_size  p=%d, size=%d, count=%d =============\n",p,size,count);
	if(count > DATA_SIZE-p)
		count=DATA_SIZE-p;
	MYDEBUG("count>data_size p=%d, size=%d, count=%d =============\n",p,size,count);
	//MYDEBUG("=========read 1============\n");
	if(copy_to_user(buf,(void *)(dev->data+p),count)){
		ret=-EFAULT;
	}else{
		MYDEBUG(KERN_INFO "read %d bytes from %p\n",count, ppos);
		*ppos+=count;
		ret = count;
		MYDEBUG(KERN_INFO "read %d bytes from %p\n",count, ppos);
 	} 
	//MYDEBUG("=========read 2============\n");
	return ret;
}  

static loff_t mymem_llseek(struct file *filp, loff_t offset, int whence){
	loff_t newpos;

	MYDEBUG("in llseek %d\n",offset);
	switch(whence){ 
		case 0://move to begin
			newpos=offset;
			break;
		case 1://move to curr
			newpos=filp->f_pos+offset;
			break;
		case 2://move to the end
			newpos=DATA_SIZE-1+offset;
			break;
		default:
			return -EINVAL;
 	}
	if((newpos<0)||(newpos>DATA_SIZE))//out of range
		return -EINVAL;
	filp->f_pos=newpos;
	return newpos;
} 

static ssize_t mymem_write(struct file *filp,const char __user *buf,size_t size,loff_t *ppos){
	unsigned long p=*ppos;

	unsigned int count=size;
	MYDEBUG("==========size=%d=============\n",size);
	int ret=0;
	struct MYMEMDEV *dev=filp->private_data;
	if(dev==NULL){ 
		MYDEBUG("==========dev NULL=============\n");
	}
	MYDEBUG("KERN_ALERT==========write1=============\n");
	if(p>=DATA_SIZE)
		return 0;
	MYDEBUG("KERN_ALERT==========write2=============\n");
	if(count > DATA_SIZE-p)
		count=DATA_SIZE-p;
	MYDEBUG("KERN_ALERT==========write3=============\n");
	if(copy_from_user(dev->data + p,buf,count)){
		ret=-EFAULT;
	}else{
		MYDEBUG(KERN_INFO "write %d bytes from %p\n",count,ppos );
		*ppos+=count;
		ret = count;
		MYDEBUG(KERN_INFO "write %d bytes from %p\n",count,ppos );
	} 
	MYDEBUG("KERN_ALERT==========write4=============\n");
	MYDEBUG("ret=%d\n",ret);
	return ret;
}

static const struct file_operations mymem_fops={
	.owner=THIS_MODULE,
	.llseek=mymem_llseek,
	.read=mymem_read,
	.write=mymem_write,
	.release=mymem_release,
	.open=mymem_open,
};

static int mymemdev_init (void){
	int result;
	//	myproc_init();
	MYDEBUG("=====mymemdev_init========\n");
	dev_t mydev=MKDEV(MEMDEV_MAJOR,0);
//	if(MEMDEV_MAJOR){
//		result=register_chrdev_region(mydev, MEMDEV_NR_DEVS,"mymemdev");
//		MYDEBUG("register_chrdev_region result=%d\n",result);
//	}else{
		result = alloc_chrdev_region(&mydev,0,MEMDEV_NR_DEVS,"mymemdev");
		MYDEBUG("alloc_chrdev_region result=%d\n",result);
//	}
	if(result<0){
		goto error;//register or alloc error
	}
	mymem_major=MAJOR(mydev);

	MYDEBUG("device create major=%d\n",mymem_major);
	MYDEBUG("=====kmalloc for mymem_dev========\n");
	mymem_dev=kmalloc((sizeof(struct MYMEMDEV)),GFP_KERNEL);
	mymem_dev->data=kmalloc(sizeof(char)*DATA_SIZE,GFP_KERNEL);
	if(!mymem_dev){
		result=-ENOMEM;
		goto error;
	} 
	MYDEBUG("=====cdev_init========\n");
	cdev_init(&(mymem_dev->mycdev),&mymem_fops);
	MYDEBUG("=====cdev_add========\n");
	int err=cdev_add(&(mymem_dev->mycdev),mydev,MEMDEV_NR_DEVS);
	if(err){
		MYDEBUG("cdev adding error\n");
 	}

	MYDEBUG("=====init finish========\n");
error:	//error redo recycle
	return result;
}

static void mymemdev_exit(void){

	MYDEBUG("=====mymemdev exit========\n");
	cdev_del(&(mymem_dev->mycdev));
	MYDEBUG("=====free mymemdev->data========\n");
	kfree(mymem_dev->data);
	MYDEBUG("=====free mymemdev========\n");
	kfree(mymem_dev);

	MYDEBUG("=====unregister mymemdev========\n");
	//MYDEBUG("=====mymemdev=%d ,MEMDEV_MAJOR=%d,========\n",mymem_major,MEMDEV_MAJOR);

	unregister_chrdev_region(MKDEV(MEMDEV_MAJOR,0),MEMDEV_NR_DEVS);
	//myproc_exit();

	MYDEBUG("=====exit finish========\n");
}

MODULE_LICENSE("GPL");
module_init(mymemdev_init);
module_exit(mymemdev_exit);


