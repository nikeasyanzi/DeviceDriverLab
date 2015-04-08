#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "mymemdev_ioctl.h"

int main() {
	int fp0 = 0;
	int cmd;
	int arg;
	int ret;
	fp0 = open("/dev/mymemdev0",O_RDWR);
	if (fp0 < 0){
		printf("fopen error\n");
		return -1;
	}else{
		cmd = MYMEMDEV_IOC_PRINT;
		printf("PRINT cmd:%x\n",cmd);
		if(ioctl(fp0,cmd)<0){ 
			printf("ioctl_print error\n");
		}else{
			printf("PRINT cmd:%x success\n",cmd);
		}

		arg = 9999;
		cmd = MYMEMDEV_IOC_SETDATA;
		printf("SETDATA cmd:%x\n",cmd);
		if(ioctl(fp0,cmd,&arg)<0){
			printf("ioctl_setdata error\n");
		} else{
//			printf("user cmd:%x, arg=%d success\n",cmd,arg);
		} 
		

		arg=0;

		printf("Now, change user space arg to %d\n",arg);
		cmd=MYMEMDEV_IOC_GETDATA;
		printf("GETDATA cmd:%x\n",cmd);
		//arg=999;
		if(ioctl(fp0,cmd,&arg)<0){ 
			printf("ioctl_getdata error\n");//wrong
		}else{
			printf("user cmd:%x, arg=%d success\n",cmd,arg);
			printf("ioctl_get from kernel space %d\n",arg);
	 	}  
	} 
	close(fp0);
	return 0;	
}
