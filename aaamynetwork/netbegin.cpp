#include<stdio.h>
#include<sys/ioctl.h>
#include<fcntl.h>
#include<unistd.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
	int i;
	for(i = 0; i < 10; i ++) {
		printf("I love you!\n");
	}
	return 0;
}
