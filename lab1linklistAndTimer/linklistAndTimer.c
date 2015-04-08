/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

* File Name : main.c

* Purpose : An simple excer to practice the list, memory and timer struct in Linux kernel

* Creation Date : 26-12-2014

* Last Modified : Mon 12 Jan 2015 12:14:03 AM CST

* Created By : Craig Yang (nikeasyanzi@yahoo.com.tw)

1. how to manipulate "pos"?   Initial 0



_._._._._._._._._._._._._._._._._._._._._.*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/timer.h>  /*timer*/
#include <asm/uaccess.h>  /*jiffies*/
#include <linux/slab.h>
#include <linux/list.h>

#define LISTSIZE 5
#define TIMERLENG 5

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Craig Yang");
MODULE_DESCRIPTION("Simple Module");
MODULE_ALIAS("Simple module");

struct timer_list timer;
struct student{
	char name[100];
	int num;
	struct list_head list;
};

struct student* student_list;
struct list_head *pos;
struct list_head list_ptr;

void timer_function(int para){
    printk("<0>Timer Expired and para is %d !!\n",para);	
} 
int timer_init(void){
    printk("Timer initializated\n");	
	init_timer(&timer);
	timer.data = TIMERLENG;
	timer.expires = jiffies + (TIMERLENG * HZ);
	timer.function = timer_function;
	add_timer(&timer);
	return 0;
} 
int	mylist_init(void){
	int i;
	student_list = kmalloc(sizeof(struct student)*LISTSIZE,GFP_KERNEL);
	memset(student_list,0,sizeof(struct student)*LISTSIZE);
	INIT_LIST_HEAD(&list_ptr);
	printk("start create list\n");
	
	for(i=0;i<LISTSIZE;i++){ 
		sprintf(student_list[i].name,"Student%d",i+1);
		student_list[i].num = i+1; 	
		list_add( &(student_list[i].list), &list_ptr);
 	 }

	printk("start initial list\n");
	struct student* tmp_student;
	list_for_each(pos, &list_ptr){
		tmp_student = list_entry(pos,struct student,list);
		printk("<0>student %d name: %s\n",tmp_student->num,tmp_student->name);
 	}
 
	printk("finish create & initial list\n");
	timer_init();
	return 0;
}  

int mylist_exit(void){
	int i;
	
	printk("start delete list\n");
 	for(i=0;i<LISTSIZE;i++){
		list_del(&(student_list[i].list));     
	}

	kfree(student_list);
	printk("finish delete list\n");
	del_timer(&timer);
	return 0;
} 




module_init(mylist_init);
module_exit(mylist_exit);

