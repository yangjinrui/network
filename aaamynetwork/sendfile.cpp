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

#define filesendport 9877
#define filerecvport 6790

char quitop[] = "quitthishost";
char grandhome[] = "/home/administrator";
char motherhome[] = "/aaamynetwork";

char curripfile[] = "/home/administrator/aaamynetwork/aaaconfig/currip";
char currtoipfile[] = "/home/administrator/aaamynetwork/aaaconfig/currtoip";
char ipcputable[] = "/home/administrator/aaamynetwork/aaaconfig/ipcputable";

char setip[] = "/home/administrator/aaamynetwork/setip";
char settoip[] = "/home/administrator/aaamynetwork/settoip";
char sendopration[] = "/home/administrator/aaamynetwork/sendopration";
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

void filetoip(char file[10], char ip[30]) {
	int i;
	bzero(ip, 30);
	i = 0;
	while (strcmp(map[i].file, file) != 0 && i < ipgross) {
		i++;
	}
	if (i == ipgross) {
		printf("\nthere is no file system%s\n", file);
		exit(1);
	}
	else {
		strcpy(ip, map[i].ip);
	}
}

int getsayerport() {
	int port;
	port = filesendport;
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

int getexerport() {
	int port;
	port = filerecvport;
	return htons(port);
}

void initbuf(char buf[1024]) {
	int i;
	for (i = 0; i < 1024; i++) {
		buf[i] = 0;
	}
}

void ltostr(char strtemp[30], long lnum) {
	char str[30] = {0};
	long num = lnum;
	int i = 0;
	int j;
	while (num / 10 != 0) {
		str[i] = '0' + num % 10;
		num = num / 10;
		printf("%s %d\n", str, num);
		i++;
	}
	str[i] = '0' + num % 10;
	for (j = i; j >= 0; j--) {
		strtemp[i - j] = str[j];
	}
	printf("\ndone\n");
	strtemp[i + 1] = '\0';
}

int main(int argc, char* argv[]){
	char filename[100] = {0};
	char toname[100] = {0};
	char strsize[30] = {0};
	char toip[30];
	bzero(toip, 30);
	strcpy(filename, argv[1]);
	strcpy(toip, argv[2]);
	strcpy(toname, argv[3]);
	long size;
	long sizetemp;
	int lastsize;
	FILE *temp;
	char buf[1024];
	//unsigned long i = 65535 - 255;
	//printf("i is x%x d %d\n", i, i);
	int sayersocket;
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
        if (bind(sayersocket,(struct sockaddr *)&sayer_addr,sizeof(sayer_addr)) < 0) {
        	printf("\nsayer: fail to bind local socket\n");
		exit(1);
	}

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

	//begin to send file
	if ((temp = fopen(filename, "r")) == NULL) {
		printf("\nfail to open file\n");
		exit(1);
	}
	fseek(temp, 0, SEEK_END);
	size = ftell(temp);
	lastsize = size % 1024;
	printf("%s length: %ld\n", filename, size);
	fclose(temp);
	//send toname
	if (send(sayersocket, toname, 100, 0) < 0) {
		printf("\nsayer: fail to send toname\n");
		exit(1);
	}
	printf("\nsayer: succeed to send toname: %s\n", toname);
	//recv response toname
	initbuf(buf);			//must clear buf!!!
	if (recv(sayersocket, buf, 1024, 0) < 0) {
		printf("\nsayer: fail to recv quit response\n");
		exit(1);
	}
	printf("\nexer: %s       from ip: %x port: %d\n", buf, exer_addr.sin_addr.s_addr, exer_addr.sin_port);
	//send file size
	ltostr(strsize, size);
	if (send(sayersocket, strsize, 30, 0) < 0) {
		printf("\nsayer: fail to send size\n");
		exit(1);
	}
	printf("\nsayer: succeed to send size: %ld\n", size);
	//recv response file size
	initbuf(buf);			//must clear buf!!!
	if (recv(sayersocket, buf, 1024, 0) < 0) {
		printf("\nsayer: fail to recv quit response\n");
		exit(1);
	}
	printf("\nexer: %s       from ip: %x port: %d\n", buf, exer_addr.sin_addr.s_addr, exer_addr.sin_port);
	if ((temp = fopen(filename, "a+")) == NULL) {
		printf("fail to open file");
		exit(1);
	}
	initbuf(buf);			//must clear buf!!!
	while (!feof(temp)){
		if ((fread(buf, 1024, 1, temp)) == 1) {
			//send command
			if (send(sayersocket, buf, 1024, 0) < 0) {
				printf("\nsayer: fail to send\n");
				exit(1);
			}
		}
		else {
			fread(buf, lastsize, 1, temp);
			//send command
			if (send(sayersocket, buf, lastsize, 0) < 0) {
				printf("\nsayer: fail to send\n");
				exit(1);
			}
		}
		printf("\nsayer: succeed to send\n");
		initbuf(buf);
	}
	//recv response file finish
	if (recv(sayersocket, buf, 1024, 0) < 0) {
		printf("\nsayer: fail to recv quit response\n");
		exit(1);
	}
	printf("\nexer: %s       from ip: %x port: %d\n", buf, exer_addr.sin_addr.s_addr, exer_addr.sin_port);
	fclose(temp);
	close(sayersocket);
	return 0;
} 
