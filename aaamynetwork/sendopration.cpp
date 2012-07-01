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

#define oprationsendport 9878
#define oprationrecvport 6791

char quitop[] = "quitthishost";

char curripfile[] = "/home/administrator/aaamynetwork/aaaconfig/currip";
char currtoipfile[] = "/home/administrator/aaamynetwork/aaaconfig/currtoip";
char ipcputable[] = "/home/administrator/aaamynetwork/aaaconfig/ipcputable";

char setip[] = "/home/administrator/aaamynetwork/setip";
char settoip[] = "/home/administrator/aaamynetwork/settoip";
char sendopration[] = "/home/administrator/aaamynetwork/sendopration";
char sendfile[] = "/home/administrator/aaamynetwork/sendfile";

int iserror;
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

int getexerport() {
	int port;
	port = oprationrecvport;
	return htons(port);
}

int getsayerport() {
	int port;
	port = oprationsendport;
	return htons(port);
}

unsigned long getsayerip() {
	unsigned long ip;
	char strip[30];
	FILE * temp;
	if ((temp = fopen(curripfile, "r")) < 0)
		printf("fail to open current ip file");
	else {
		fscanf(temp, "%s", strip);
		ip = inet_addr(strip);
	}
	fclose(temp);
	return ip;
}

void initbuf(char buf[1024]) {
	int i;
	for (i = 0; i < 1024; i++) {
		buf[i] = 0;
	}
}

int main(int argc, char * argv[]){
	char *command;
	char toip[30];
	char temp[100] = {0};
	int num;
	int i;
	bzero(toip, 30);
	strcpy(toip, argv[1]);
	num = argc - 2;
	strcat(temp, argv[2]);
	for (i = 1; i < num; i++) {
		strcat(temp, " ");
		strcat(temp, argv[i + 2]);
	}
	command = temp;
	char buf[1024];
	int exersocket, sayersocket;
        struct sockaddr_in   exer_addr, sayer_addr;
        
        bzero((char *)&sayer_addr,sizeof(sayer_addr));
        sayer_addr.sin_family = AF_INET;
        sayer_addr.sin_port = getsayerport();
        sayer_addr.sin_addr.s_addr = getsayerip();
	sayersocket = socket(AF_INET,SOCK_STREAM,0);
	if (sayersocket < 0) {
        	printf("\nsayer: fail to create local socket, socketid: %d\n", sayersocket);
		exit(1);
	}
	printf("\nsayer:succeed to create local socket, socketid: %d , from ip: %x , port: %d h ip: %x port %d\n", sayersocket, sayer_addr.sin_addr.s_addr, sayer_addr.sin_port, getsayerip(), getsayerport());
        if (bind(sayersocket,(struct sockaddr *)&sayer_addr,sizeof(sayer_addr)) < 0) {
        	printf("\nsayer: fail to bind local socket\n");
		exit(1);
	}
	printf("\nsayer: succeed to bind local socket\n");

        bzero((char *)&exer_addr,sizeof(exer_addr));
        exer_addr.sin_family = AF_INET;
        exer_addr.sin_port = getexerport();
        exer_addr.sin_addr.s_addr = inet_addr(toip);
	printf("\nsayer: to ip: %x port: %d\n", exer_addr.sin_addr.s_addr, exer_addr.sin_port);
	if (connect(sayersocket, (sockaddr *)&exer_addr, sizeof(exer_addr)) < 0) {
		printf("\nsayer: fail to connect with %x : %d\n", exer_addr.sin_addr.s_addr, exer_addr.sin_port);
		exit(1);
	}
	if (send(sayersocket, command, strlen(command), 0) < 0) {
		printf("\nsayer: fail to send\n");
		exit(1);
	}
	printf("\nsayer: succeed to send: %s\n", command);
	if (recv(exersocket, buf, 1024, 0) < 0) {
		printf("\nsayer: fail to recv response\n");
		exit(1);
	}
	printf("\nexer: %s\n", buf);
	close(sayersocket);
	return 0;
} 
