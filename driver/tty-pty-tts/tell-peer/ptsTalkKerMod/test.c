#include <fcntl.h>
#include <stdio.h>

#define DEV_NAME	"/dev/ptsTalkKerMod"

int main()
{
	int fd;
	int val=1;

	fd = open(DEV_NAME, O_RDWR);
	if(fd < 0)
		printf("test: can't open %s \n", DEV_NAME);

	write(fd, &val, 4);

	read(fd, &val, 4);

	return 0;
}
