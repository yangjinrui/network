#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define connectsendport     9875
#define connectrecvport     6788

char rsignal[] = "aaaa";
char control[] = "control";
char follow[] = "follow";

char controlip[] = "/home/administrator/aaamynetwork/aaaconfig/controlip";
char followip[] = "/home/administrator/aaamynetwork/aaaconfig/followip";
char otherip[] = "/home/administrator/aaamynetwork/aaaconfig/otherip";
char ipc[] = "/home/administrator/aaamynetwork/aaaconfig/ipc";
char curripfile[] = "/home/administrator/aaamynetwork/aaaconfig/currip";
char currtoipfile[] = "/home/administrator/aaamynetwork/aaaconfig/currtoip";
char thisis[] = "/home/administrator/aaamynetwork/aaaconfig/thisis";

char setip[] = "/home/administrator/aaamynetwork/setip";
char settoip[] = "/home/administrator/aaamynetwork/settoip";

int iserror;

int getexerport() {
	int port;
	port = connectrecvport;
	return htons(port);
}

unsigned long getexerip() {
	unsigned long ip;
	char strip[30] = {0};
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
	port = connectsendport;
	return htons(port);
}

unsigned long getsayerip() {
	unsigned long ip;
	char strip[30];
	FILE * temp;
	if ((temp = fopen(currtoipfile, "r")) < 0)
		printf("\nfail to open current ip file\n");
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

int main(){
	FILE * temp;
	int exersocket, getsocket;
	char thisclass[20];
	socklen_t getsocketlen;
        struct sockaddr_in   exer_addr, get_addr;

	iserror = 0;
        
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
	while (1) {
		if ((getsocket = accept(exersocket, (sockaddr *)&get_addr, &getsocketlen)) < 0) {
			printf("\nexer: fail to accept\n");
			exit(1);
		}
		//send signal
		if (send(getsocket, rsignal, strlen(rsignal), 0) < 0) {
			printf("\nexer: fail to recv response\n");
			exit(1);
		}
		printf("\nsend real signal to a new computer\n");
		//send class
		if ((temp = fopen(thisis, "r")) < 0) {
			printf("\nfail to open thisis(class) file\n");
			exit(1);
		}
		else
			fscanf(temp, "%s", thisclass);
		if (send(getsocket, thisclass, strlen(thisclass), 0) < 0) {
			printf("\nexer: fail to send back\n");
			exit(1);
		}
		printf("\nsucceed to send back class: %s\n", thisclass);
		close(getsocket);
	}
	close(exersocket);
	return 0;
} 
