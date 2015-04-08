#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "test_ioctl.h"

#define DEVFILE "/dev/t1"

int main(void)
{
	struct ioctl_arg cmd;
	int fd;
	long ret;
	int num = 0;

	fd = open(DEVFILE, O_RDWR);
	if (fd == -1)
		perror("open");

	memset(&cmd, 0, sizeof(cmd));
	ret = ioctl(fd, IOCTL_VALGET, &cmd);

	if (ret == -1) {
		printf("errno %d\n", errno);
		perror("ioctl");

	}
	printf("val %x\n", cmd.val);

	cmd.val = 0xCC;
	ret = ioctl(fd, IOCTL_VALSET, &cmd);
	if (ret == -1) {
		printf("errno %d\n", errno);
		perror("ioctl");

	}

	ret = ioctl(fd, IOCTL_VALGET, &cmd);

	if (ret == -1) {
		printf("errno %d\n", errno);
		perror("ioctl");

	}
	printf("val %x\n", cmd.val);


	num = 100;
	ret = ioctl(fd, IOCTL_VALSET_NUM, num);
	if (ret == -1) {
		printf("errno %d\n", errno);
		perror("ioctl");

	}

	ret = ioctl(fd, IOCTL_VALGET_NUM, &num);

	if (ret == -1) {
		printf("errno %d\n", errno);
		perror("ioctl");

	}
	printf("num %d\n", num);

	if (close(fd) != 0)
		perror("close");

	return 0;

}

