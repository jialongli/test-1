/* Copyright (C) Rong Tao @Sylincom Beijing, 2019年 06月 14日 星期五 09:05:41 CST. */
#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
/* Return a random integer between MIN and MAX, inclusive. Obtain
 * randomness from /dev/random. */
int random_number (int min, int max)
{
	/* Store a file descriptor opened to /dev/random in a static
	 * variable. That way, we don't need to open the file every time
	 * this function is called. */
	static int dev_random_fd = -1;
	char* next_random_byte;
	int bytes_to_read;
	unsigned random_value;
	/* Make sure MAX is greater than MIN. */
	assert (max > min);
	/* If this is the first time this function is called, open a file
	 * descriptor to /dev/random. */
	if (dev_random_fd == -1) {
		dev_random_fd = open ("/dev/random", O_RDONLY);
		assert (dev_random_fd != -1);
	}
	/* Read enough random bytes to fill an integer variable. */
	next_random_byte = (char*) &random_value;
	bytes_to_read = sizeof (random_value);
	/* Loop until we've read enough bytes. Because /dev/random is filled
	 * from user-generated actions, the read may block and may only
	 * return a single random byte at a time. */
	do {
		int bytes_read;
		bytes_read = read (dev_random_fd, next_random_byte, bytes_to_read);
		bytes_to_read -= bytes_read;
		next_random_byte += bytes_read;
	} while (bytes_to_read > 0);
	/* Compute a random number in the correct range. */
	return min + (random_value % (max - min + 1));
}


int main()
{
	int i;
	for (i=1;i<=100;i++)
	{
		printf("%4x ", random_number(1, 100));
		if(i%10==0)
			printf("\n");
	}
}

