#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/time.h>
#include <stdio.h>

int main()
{
	FILE *fp0 = NULL;
	char Buf[512];
	
	/*初始化Buf*/
	strcpy(Buf,"Initial Mem is char dev! write to BUF");
	printf("BUF: %s\n\n",Buf);
	
	/*打开设备文件*/
	fp0 = fopen("/dev/mymemdev","r+");
	if (fp0 == NULL)
	{
		perror("fopen error");
		return 0;
	}

	/*写入设备*/
	fwrite(Buf, sizeof(Buf), 1, fp0);
	
	sleep(5);
	/*重新定位文件位置（思考没有该指令，会有何后果)*/
	fseek(fp0,0,SEEK_SET);
	
	/*清除Buf*/
	strcpy(Buf,"String in Buf is changed to NULL!");
	printf("BUF: %s\n",Buf);
	
	/*读出设备*/
	fread(Buf, sizeof(Buf), 1, fp0);
	
	/*检测结果*/
	printf("BUF: %s\n",Buf);
	
	return 0;	

}
