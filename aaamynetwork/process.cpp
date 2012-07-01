#include<stdio.h>
#include<sys/ioctl.h>
#include<fcntl.h>
#include<unistd.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdlib.h>

void process() {
	int i;
	pid_t pid = fork();
	if (pid == 0) {
		execl("/home/administrator/aaamynetwork/farcommand", NULL);
		exit(0);
	}
	else if (pid > 0) {
		for(i = 0; i < 5; i ++) {
			printf("Source %d has already been prepared OK and you should see it.\n", i);
			sleep(1);
		}
	}
	else {
		printf("fail to get son process");
		exit(1);
	}
	printf("\nfinish\n");
}

int main() {
	//execl("/home/administrator/aaamynetwork/getsource", NULL);
	process();
	printf("\nout of process\n");
	return 0;
}
