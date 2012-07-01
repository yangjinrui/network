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

#define commandsendport     9876
#define commandrecvport     6789

char quitop[] = "quitthishost";

char curripfile[] = "/home/administrator/aaamynetwork/aaaconfig/currip";
char currtoipfile[] = "/home/administrator/aaamynetwork/aaaconfig/currtoip";
char ipcputable[] = "/home/administrator/aaamynetwork/aaaconfig/ipcputable";

char setip[] = "/home/administrator/aaamynetwork/setip";
char settoip[] = "/home/administrator/aaamynetwork/settoip";
char sendopration[] = "/home/administrator/aaamynetwork/sendopration";
char sendcpucommand[] = "/home/administrator/aaamynetwork/sendcpucommand";
char sendfile[] = "/home/administrator/aaamynetwork/sendfile";

int ipgross;

struct ipcpumap {
	char cpu[10];
	char file[10];
	char ip[30];
};

ipcpumap map[20];

int getmap() {
	int i;
	FILE * temp;
	i = 0;
	if ((temp = fopen(ipcputable, "r")) < 0) {
		printf("fail to open ip-cup-mapping-tabel");
	}
	else {
		while (!feof(temp)) {
			fscanf(temp, "%s %s %s\n", map[i].ip, map[i].cpu, map[i].file);
			i++;
		}
	}
	fclose(temp);
	return i;
}

void cputoip(char cpu[10], char ip[30]) {
	int i;
	bzero(ip, 30);
	i = 0;
	while (strcmp(map[i].cpu, cpu) != 0 && i < ipgross) {
		i++;
	}
	if (i == ipgross) {
		printf("\nthere is no cpu%s\n", cpu);
		exit(1);
	}
	else {
		strcpy(ip, map[i].ip);
	}
}

int main(int argc, char* argv[]) {
	int i;
	char cpu[10] = {0};
	char ip[30] = {0};
	char op[100] = {0};
	ipgross = getmap();
	if (argc == 1) {
		for(i = 0; i < ipgross; i++) {
			printf("%s    ", map[i].cpu);
		}
		printf("\n");
	}
	else {
		strcpy(cpu, argv[1]);
		cputoip(cpu, ip);
		strcat(op, sendcpucommand);
		strcat(op, " ");
		strcat(op, cpu);
		system(op);
	}
	return 0;
}
