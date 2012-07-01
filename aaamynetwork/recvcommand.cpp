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

#define commandsendport     9876
#define commandrecvport     6789

char quitop[] = "quitthishost";

char curripfile[] = "/home/administrator/aaamynetwork/aaaconfig/currip";
char currtoipfile[] = "/home/administrator/aaamynetwork/aaaconfig/currtoip";

char currcommand[] = "/home/administrator/aaamynetwork/farcommand";

int iserror;

int getexerport() {
	int port;
	port = commandrecvport;
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
	port = commandsendport;
	return htons(port);
}

unsigned long getsayerip() {
	unsigned long ip;
	char strip[30];
	FILE * temp;
	if ((temp = fopen(currtoipfile, "r")) < 0)
		printf("fail to open current ip file");
	else {
		fscanf(temp, "%s", strip);
		ip = inet_addr(strip);
	}
	fclose(temp);
	return ip;
} 

char * getresponse() {
	char *response;
	if (!iserror)
		response = "done";
	else {
		response = "gotten but done wrong";
		iserror = 0;
	}
	return response;
}

void initbuf(char buf[1024]) {
	int i;
	for (i = 0; i < 1024; i++) {
		buf[i] = 0;
	}
}

void doonecommand(char *command) {
	system(command);
}

int main(){
	int i;
	char *command;
	char *response;
	char buf[1024];
	//command = "cd /home/administrator";
	//printf("%s\n%d\n", command, strlen(command));
	int exersocket, getsocket;
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
		chmod(currcommand, 0777);
		//recv command
		if (recv(getsocket, buf, 1024, 0) < 0) {
			printf("\nexer: fail to recv response\n");
			exit(1);
		}
		printf("\nsayer: %s\n", buf);
		while (strcmp(buf, quitop) != 0) {
			//do command
			doonecommand(buf);
			initbuf(buf);
			//say back to sayer
			response = getresponse();
			if (send(getsocket, response, strlen(response), 0) < 0) {
				printf("\nexer: fail to send back\n");
				exit(1);
			}
			printf("\nexer: succeed to send back: %s\n", response);
			if (recv(getsocket, buf, 1024, 0) < 0) {
				printf("\nexer: fail to recv response\n");
				exit(1);
			}
			printf("\nsayer: %s\n", buf);
		}
		//do command quitthishost
		//say quitthishost back to sayer
		response = "have quit it";
		if (send(getsocket, response, strlen(response), 0) < 0) {
			printf("\nexer: fail to send back\n");
			exit(1);
		}
		printf("\nexer: succeed to send back: %s\n", response);
		close(getsocket);
	}
	close(exersocket);
	return 0;
} 
