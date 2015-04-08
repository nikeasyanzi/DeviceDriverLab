#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/time.h>
#include <stdio.h>

int main()
{
	FILE *fp0 = NULL;
	char Buf[512];
	
	char filepath[512];
	int i;
	for (i = 0; i < 2; i++) {
		sprintf(filepath,"/dev/mymemdev%d",i);
		printf("filepath=%s\n",filepath);
		strcpy(Buf,"Initial Mem is char dev! write to BUF");
		printf("BUF: %s\n\n",Buf);
		//fp0 = fopen("/dev/mymemdev0","r+");
		fp0 = fopen(filepath,"r+");
		if (fp0 == NULL)
		{
			perror("fopen error");
			return 0;
		}
		sleep(1);

		/*写入设备*/
		fwrite(Buf, sizeof(Buf),1, fp0);
		/*重新定位文件位置（思考没有该指令，会有何后果)*/
		fseek(fp0,0,SEEK_SET);
		/*清除Buf*/
		strcpy(Buf,"NULL!");
		printf("BUF: %s\n",Buf);
		/*读出设备*/
		fread(Buf,sizeof(Buf),1, fp0);
		/*检测结果*/
		printf("String in BUF: %s\n",Buf);
	}
	return 0;	

}
