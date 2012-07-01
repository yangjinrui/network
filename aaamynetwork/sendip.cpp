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

int getsayerport() {
	int port;
	port = ipsendport;
	return htons(port);
}

int main(int argc, char * argv[]){
	char ctrip[30];
	char currip[30];
	int exersocket, sayersocket;
        struct sockaddr_in   exer_addr, sayer_addr;
	FILE * temp;
	bzero(ctrip, 30);
	bzero(currip, 30);
	if ((temp = fopen(curripfile, "r")) < 0)
		printf("fail to open current ip file");
	else {
		fscanf(temp, "%s", currip);
	}
	fclose(temp);
	if ((temp = fopen(controlip, "r")) < 0)
		printf("fail to open current ip file");
	else {
		fscanf(temp, "%s", ctrip);
	}
	fclose(temp);
        bzero((char *)&sayer_addr,sizeof(sayer_addr));
        sayer_addr.sin_family = AF_INET;
        sayer_addr.sin_port = getsayerport();
        sayer_addr.sin_addr.s_addr = inet_addr(currip);
	sayersocket = socket(AF_INET,SOCK_STREAM,0);
	if (sayersocket < 0) {
        	printf("\nsayer: fail to create local socket, socketid: %d\n", sayersocket);
		exit(1);
	}
	printf("\nsayer:succeed to create local socket, socketid: %d , from ip: %x , port: %d\n", sayersocket, sayer_addr.sin_addr.s_addr, sayer_addr.sin_port);
        if (bind(sayersocket,(struct sockaddr *)&sayer_addr,sizeof(sayer_addr)) < 0) {
        	printf("\nsayer: fail to bind local socket\n");
		exit(1);
	}
	printf("\nsayer: succeed to bind local socket\n");

        bzero((char *)&exer_addr,sizeof(exer_addr));
        exer_addr.sin_family = AF_INET;
        exer_addr.sin_port = getexerport();
        exer_addr.sin_addr.s_addr = inet_addr(ctrip);
	printf("\nsayer: to ip: %x port: %d\n", exer_addr.sin_addr.s_addr, exer_addr.sin_port);
	if (connect(sayersocket, (sockaddr *)&exer_addr, sizeof(exer_addr)) < 0) {
		printf("\nsayer: fail to connect with %x : %d\n", exer_addr.sin_addr.s_addr, exer_addr.sin_port);
		exit(1);
	}
	if (send(sayersocket, ctrip, strlen(ctrip), 0) < 0) {
		printf("\nsayer: fail to send\n");
		exit(1);
	}
	printf("\nsayer: succeed to send: %s\n", ctrip);
	close(sayersocket);
	return 0;
} 
