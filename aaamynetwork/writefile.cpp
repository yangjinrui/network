#include <stdio.h>   
#include <sys/types.h>   
#include <sys/socket.h>   
#include <netinet/in.h>   
#include <arpa/inet.h>   
#include <time.h>   
#include <errno.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char *curripfile = "/home/administrator/aaamynetwork/aaaconfig/currip";
char *setip = "/home/administrator/aaamynetwork/setip";
char *settoip = "/home/administrator/aaamynetwork/settoip";
char *ipc = "/home/administrator/aaamynetwork/aaaconfig/ipc";
char *begin = "/home/administrator/aaamynetwork/begin";
char *iloveu = "i love you";

void initbuf(char buf[1024]) {
	int i;
	for (i = 0; i < 1024; i++) {
		buf[i] = 0;
	}
}

int main(int argc, char* argv[]) {
	char buf[50];
	bzero(buf, 50);
	FILE * temp;
	if ((temp = fopen(curripfile, "r")) < 0) {
		printf("fail to open current ip file");
	}
	fscanf(temp, "%s", buf);
	fclose(temp);
	printf("buf is: %s\n", buf);
	if (buf[0] == 0) {
		printf("current ip file is empty\n");
	}
	return 0;
}
