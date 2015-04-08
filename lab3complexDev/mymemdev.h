#ifndef _MYMEMDEV_H_
#define _MYMEMDEV_H_

#ifndef MEMDEV_MAJOR
#define MEMDEV_MAJOR 0
#endif

#ifndef MEMDEV_NR_DEVS
#define MEMDEV_NR_DEVS 2
#endif

#ifndef DATA_SIZE
#define DATA_SIZE 512 
#endif

#ifndef DRIVER_NAME
#define DRIVER_NAME "mymemdev"
#endif 

#ifdef MYDEBUGLOG
#define MYDEBUG(fmt, args...) printk("%s: " fmt, __FUNCTION__ , ## args)
#else
#define MYDEBUG(args...)
#endif

struct MYMEMDEV{ 
char *data;
struct cdev mycdev;
struct semaphore sem;  
};
#endif

