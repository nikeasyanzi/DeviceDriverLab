#include <linux/kernel.h> /* We're doing kernel work */
#include <linux/module.h> /* Specifically, a module */
#include <linux/proc_fs.h> /* Necessary because we use proc fs */
#include <linux/seq_file.h> /* for seq_file */
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

#define PROC_NAME "mymemdev"
#define PROCFS_MAX_SIZE 2048

char *proc_msg;
int proc_buf_length=0;

static ssize_t my_proc_write(struct file *fp, const char *buf, size_t len,loff_t *off) {
    if(len>PROCFS_MAX_SIZE) {
        return -EFAULT;
    }

    if(copy_from_user(proc_msg,buf,len)) {
        return -EFAULT;
    }

    proc_buf_length=proc_buf_length+len;

    return len;
}

static ssize_t my_proc_read(struct file *filp,char *buf,size_t count,loff_t *offp ) {

    if(count>proc_buf_length)
    {
        count=proc_buf_length;
    }
    proc_buf_length=proc_buf_length-count;
    copy_to_user(buf,proc_msg, count);

    return count;
}
static struct file_operations my_file_ops = {
    .owner = THIS_MODULE,
    .read=my_proc_read,
    .write=my_proc_write
};

/**
 * This function is called when the module is loaded
 *
  */
int init_module(void)
{
    struct proc_dir_entry *entry;
    entry = proc_create(PROC_NAME, 0, NULL, &my_file_ops);

    if (!entry) {
        return -ENOMEM;
    }

    proc_msg=(char * )vmalloc(PROCFS_MAX_SIZE);

    return 0;
}
/**
 * This function is called when the module is unloaded.
 *
 */
void cleanup_module(void)
{
    remove_proc_entry(PROC_NAME, NULL);
}

MODULE_AUTHOR("Craig Yang");
MODULE_LICENSE("GPL");
