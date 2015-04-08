#include <pthread.h>    
#include <stdio.h>    
#define COUNT 10
void *write_george(void *argu) {  
	
	char buf[512];
	int i;
	
	FILE *fp0=NULL;

	for  (i = 0; i < COUNT; i++) {

		fp0=fopen("dev/mymemdev0","r+");

		sprintf(buf,"This is %dnd write",i);

		fseek(fp0,0,SEEK_SET);

		fwrite(buf, sizeof(buf), 1, fp0);

		memset(buf,'\0',sizeof(buf));

		sleep(2);  

		fclose(fp0);	
	}
	return NULL;    
}    

void *read_mary(void *argu) {    
	char buf[512];
	int i;

	FILE *fp0=NULL;

	for  (i = 0; i < COUNT; i++) {
		memset(buf,'\0',sizeof(buf));	
		fp0=fopen("dev/mymemdev0","r+");

		fseek(fp0,0,SEEK_SET);

		fread(buf, sizeof(buf), 0, fp0);

		if(buf[0]=='\0'){ 
			printf("buf:%s\n",buf);
		}else{
			printf("buf is empty\n");
			sleep(1);    
		}
		sleep(1);    
		fclose(fp0);	
	}	

	return NULL;    
}    

int main() {     
	int err=0;
	pthread_t thread1, thread2;  

	err=pthread_create(&thread1, NULL, &write_george, NULL);   
//	if(err) printf("err write george\n");
	err=pthread_create(&thread2, NULL, &read_mary, NULL);   
//	if(err) printf("err read_mary");

	
	/*
	while (1) {    
		printf("----------------\n"); 
	}    
*/
		sleep(20);    
	pthread_exit(NULL);
	return 0;   
}
