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

char setip[] = "/home/administrator/aaamynetwork/setip";
char settoip[] = "/home/administrator/aaamynetwork/settoip";

unsigned long getsayerip() {
	FILE * temp;
	unsigned long ip;
	char sip[30] = {0};
	if ((temp = fopen(curripfile, "r")) < 0)
		printf("fail to open current ip file");
	else
		fscanf(temp, "%s", sip);
	fclose(temp);
	ip = inet_addr(sip);
	return ip;
}

int getexerport() {
	int port;
	port = connectrecvport;
	return htons(port);
}

int main(int argc, char* argv[]){
	char getbuf[50];
	char toip[30] = {0};
	strcpy(toip, argv[1]);
	FILE * cf;
	FILE * ff;
	FILE * of;
	FILE * temp;
	int num;
	int sayersocket;
        struct sockaddr_in   exer_addr, sayer_addr;
        
        bzero((char *)&sayer_addr,sizeof(sayer_addr));
        sayer_addr.sin_family = AF_INET;
        sayer_addr.sin_port = htons(0);
        sayer_addr.sin_addr.s_addr = getsayerip();
	sayersocket = socket(AF_INET,SOCK_STREAM,0);
	if (sayersocket < 0) {
        	printf("\nsayer: fail to create local socket, socketid: %d\n", sayersocket);
		exit(1);
	}
	printf("\nsayer:succeed to create local socket, socketid: %d , from ip: %x , port: %d\n", sayersocket, sayer_addr.sin_addr.s_addr, sayer_addr.sin_port, getsayerip());
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
	//connect exer
	if (connect(sayersocket, (sockaddr *)&exer_addr, sizeof(exer_addr)) < 0) {
		printf("\nsayer: fail to connect with %x : %d\n", exer_addr.sin_addr.s_addr, exer_addr.sin_port);
		exit(1);
	}
	//recv signal
	bzero(getbuf, 50);
	if (recv(sayersocket, getbuf, 50, 0) < 0) {
		printf("\nsayer: fail to recv signal\n");
		exit(1);
	}
	printf("\nexer: %s       from ip: %x port: %d\n", getbuf, exer_addr.sin_addr.s_addr, exer_addr.sin_port);
	if (strcmp(getbuf, rsignal) != 0) {
		printf("\n%s is not upon our system\n", toip);
		exit(0);
	}
	//recv class
	bzero(getbuf, 50);
	if (recv(sayersocket, getbuf, 50, 0) < 0) {
		printf("\nsayer: fail to recv class signal\n");
		exit(1);
	}
	printf("\nexer: %s       from ip: %x port: %d\n", getbuf, exer_addr.sin_addr.s_addr, exer_addr.sin_port);
	if ((ff = fopen(followip, "a+")) < 0) {
		printf("fail to open ip-class file");
		exit(1);
	}
	if ((of = fopen(otherip, "a+")) < 0) {
		printf("fail to open ip-class file");
		exit(1);
	}
	if ((cf = fopen(controlip, "w")) < 0) {
		printf("fail to open ip-class file");
		exit(1);
	}
	if (strcmp(getbuf, control) == 0) {
		fprintf(cf, "%s\n", toip);
		fprintf(of, "%s\n", toip);
		if ((temp = fopen(ipc, "r")) < 0) {
			printf("fail to open ipc file");
		}
		else {
			fscanf(temp, "%d", &num);
		}
		fclose(temp);
		num = num + 1;
		if ((temp = fopen(ipc, "w")) < 0){
			printf("fail to open ipc file");
		}
		else {
			fprintf(temp, "%d", num);
		}
		fclose(temp);
		printf("\n%s is controler\n", toip);
		exit(0);
	}
	else if (strcmp(getbuf, follow) == 0) {
		fprintf(ff, "%s\n", toip);
		fprintf(of, "%s\n", toip);
		if ((temp = fopen(ipc, "r")) < 0) {
			printf("fail to open ipc file");
		}
		else {
			fscanf(temp, "%d", &num);
		}
		fclose(temp);
		num = num + 1;
		if ((temp = fopen(ipc, "w")) < 0){
			printf("fail to open ipc file");
		}
		else {
			fprintf(temp, "%d", num);
		}
		fclose(temp);
		printf("\n%s is follower\n", toip);
		exit(0);
	}
	else {
		printf("\n%s is not upon our system because it is not either controler or follower\n", toip);
		exit(0);
	}
	fclose(ff);
	fclose(of);
	fclose(cf);
	close(sayersocket);
	return 0;
} 
