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

#define ipsendport 9874
#define iprecvport 6787

char quitop[] = "quitthishost";

char curripfile[] = "/home/administrator/aaamynetwork/aaaconfig/currip";
char currtoipfile[] = "/home/administrator/aaamynetwork/aaaconfig/currtoip";
char controlip[] = "/home/administrator/aaamynetwork/aaaconfig/controlip";
char ipcputable[] = "/home/administrator/aaamynetwork/aaaconfig/ipcputable";
char ipc[] = "/home/administrator/aaamynetwork/aaaconfig/ipc";
char followip[] = "/home/administrator/aaamynetwork/aaaconfig/followip";
char otherip[] = "/home/administrator/aaamynetwork/aaaconfig/otherip";

char setip[] = "/home/administrator/aaamynetwork/setip";
char settoip[] = "/home/administrator/aaamynetwork/settoip";
char sendopration[] = "/home/administrator/aaamynetwork/sendopration";
char sendfile[] = "/home/administrator/aaamynetwork/sendfile";

int iserror;
int ipgross;

int getexerport() {
	int port;
	port = iprecvport;
	return htons(port);
}

unsigned long getexerip() {
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

int getsayerport() {
	int port;
	port = ipsendport;
	return htons(port);
}

int main(int argc, char * argv[]){
	char foip[30];
	int num;
	int exersocket, getsocket;
	socklen_t getsocketlen;
        struct sockaddr_in   exer_addr, get_addr;
	FILE * temp;

        bzero((char *)&exer_addr,sizeof(exer_addr));
        exer_addr.sin_family = AF_INET;
        exer_addr.sin_port = getexerport();
        exer_addr.sin_addr.s_addr = getexerip();
	printf("\nexer: to ip: %x port: %d h ip: %x port: %d\n", exer_addr.sin_addr.s_addr, exer_addr.sin_port, getexerip(), getexerport());
	exersocket = socket(AF_INET,SOCK_STREAM,0);
	if (exersocket < 0) {
        	printf("\nexer: fail to create server socket, socketid: %d\n", exersocket);
		exit(1);
	}
	printf("\nexer: succeed to create server socket, socketid: %d\n", exersocket);
        if (bind(exersocket,(struct sockaddr *)&exer_addr,sizeof(exer_addr)) < 0) {
        	printf("\nexer: fail to bind server socket\n");
		exit(1);
	}
	printf("\nexer: succeed to bind server socket\n");

	if (listen(exersocket, 5) < 0) {
		printf("\nexer: fail to listen %x : %d\n", exer_addr.sin_addr.s_addr, exer_addr.sin_port);
		exit(1);
	}
	bzero(&get_addr, sizeof(get_addr));
	while (1) {
		if ((getsocket = accept(exersocket, (sockaddr *)&get_addr, &getsocketlen)) < 0) {
			printf("\nexer: fail to accept\n");
			exit(1);
		}
		//recv follow ip
		bzero(foip, 30);
		if (recv(getsocket, foip, 30, 0) < 0) {
			printf("\nexer: fail to recv response\n");
			exit(1);
		}
		printf("\nsayer: %s\n", foip);
		if ((temp = fopen(ipc, "r")) < 0)
			printf("fail to open current ip file");
		else {
			fprintf(temp, "%d\n", &num);
		}
		fclose(temp);
		if ((temp = fopen(followip, "a+")) < 0)
			printf("fail to open current ip file");
		else {
			fprintf(temp, "%s\n", foip);
		}
		fclose(temp);
		if ((temp = fopen(otherip, "a+")) < 0)
			printf("fail to open current ip file");
		else {
			fprintf(temp, "%s\n", foip);
		}
		fclose(temp);
		num = num + 1;
		if ((temp = fopen(ipcputable, "a+")) < 0)
			printf("fail to open current ip file");
		else {
			fprintf(temp, "%s %d %d\n", foip, num, num);
		}
		fclose(temp);
		if ((temp = fopen(ipc, "w")) < 0)
			printf("fail to open current ip file");
		else {
			fprintf(temp, "%d\n", num);
		}
		fclose(temp);
		close(getsocket);
	}
	close(exersocket);
	return 0;
} 
