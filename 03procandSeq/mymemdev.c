#include <linux/kernel.h> /* We're doing kernel work */
#include <linux/module.h> /* Specifically, a module */
#include <linux/proc_fs.h> /* Necessary because we use proc fs */
#include <linux/seq_file.h> /* for seq_file */
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
 
#define PROC_NAME "mymemdev"
#define MAX_LINE 5

/** 
 * * This function is called at the beginning of a sequence.
 * * ie, when:
 * * 1.the /proc file is read (first time)
 * * 2.after the function stop (end of sequence)
 * *
 * */ 
static void *my_seq_start(struct seq_file *s, loff_t *pos) 
{   
	printk("==========================\n");
	printk("in seq_start\n");

	uint32_t *lines;

	if (*pos >= MAX_LINE) {
		return NULL; // no more data to read
	}

	lines = kzalloc(sizeof(uint32_t), GFP_KERNEL);
	if (!lines) {
		return NULL;
	}

	*lines = *pos + 1;

	return lines;

} 
/* * 
 * This function is called after the beginning of a sequence.
 * It's called untill the return is NULL (this ends the sequence).
 *
 */  
static void *my_seq_next(struct seq_file *s, void *v, loff_t *pos) 
{ 
	printk("in seq_next\n");
	int32_t *lines = v;
	
	*pos = ++(*lines);
	if (*pos >= MAX_LINE) {
		return NULL; // no more data to read
	}
	return lines;
}  
/**   
 * This function is called at the end of a sequence
 *
 */  
static void my_seq_stop(struct seq_file *s, void *v) 
{ 
	printk("in seq_stop\n");
	
	kfree(v);
	/* nothing to do, we use a static value in start() */ 
}    
/**   
 * This function is called for each "step" of a sequence
 *
 */   
static int my_seq_show(struct seq_file *s, void *v) 
{  
	printk("in seq_snow\n");

	seq_printf(s, "This is %d nd seq trail\n", *((uint32_t*)v));

	return 0;
}   
/** 
 * This structure gather "function" to manage the sequence
 *
 */ 

static struct seq_operations my_seq_ops = { 
	.start = my_seq_start,
	.next = my_seq_next,
	.stop = my_seq_stop,
	.show = my_seq_show 
};

/**
 * This function is called when the /proc file is open.
 *
 */ 
static int my_open(struct inode *inode, struct file *file) 
{ 
	return seq_open(file, &my_seq_ops);
};
/**
 * This structure gather "function" that manage the /proc file
 *
 */ 

static struct file_operations my_file_ops = { 
	.owner = THIS_MODULE,
	.open = my_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release 
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
