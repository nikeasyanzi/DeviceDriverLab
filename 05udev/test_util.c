#include <stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH 256               ///< The buffer length (crude but fine)

int main( int argc ,char * argv [] ) {
    int ret, fd;
    char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM
    char string_buf[BUFFER_LENGTH];
    char path[BUFFER_LENGTH];
    int  num_of_device;

    int i;
	
	
	if(argc!=2){
		printf("please specify total_devices_number ex. ./a.out 3\n");
	return 0;
	}

    num_of_device= strtol (argv[1], (char **)NULL,10 );
    printf("num=%d\n",num_of_device);

	for (i=0; i<num_of_device; i++) {
		memset(path,'\0',sizeof(path));
		memset(receive,'\0',sizeof(receive));
		memset(string_buf,'\0',sizeof(string_buf));
		sprintf(path,"/dev/simplechardev%d",i);
		printf("Starting device test on %s...\n", path);

		fd = open(path, O_RDWR);             // Open the device with read/write access
		if (fd < 0) {
			fprintf(stderr, "%s\n Failed to open the device %s\n", strerror(errno),path);
			return errno;
	 	}
		printf("Type in a short string to send to the kernel module:\n");
		sprintf(string_buf, "hello simplechardev%d\n",i);

	 	if( (strlen(string_buf) >0)&& string_buf[strlen(string_buf)-1]=='\n') {
			string_buf[strlen(string_buf)-1]='\0';
		}

		printf("Writing message to the device [%s].\n", string_buf);
		ret = write(fd, string_buf, strlen(string_buf)); // Send the string to the LKM
		if (ret < 0) {
			fprintf(stderr, "%s\n Failed to write the message to the device %s\n", strerror(errno),path);
			return errno;
		}

		printf("Reading from the device %s\n",path);
		ret = read(fd, receive, BUFFER_LENGTH);        // Read the response from the LKM
		if (ret < 0) {
			fprintf(stderr, "%s\n Failed to read the message from the device %s\n", strerror(errno),path);
			return errno;
		}
		printf("The received message is: [%s]\n", receive);
		
		close(fd);
	}

	printf("End of the program\n");
	return 0;
}
