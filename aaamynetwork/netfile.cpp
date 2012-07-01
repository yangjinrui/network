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

void getmap() {
	FILE * temp;
	ipgross = 0;
	if ((temp = fopen(ipcputable, "r")) < 0) {
		printf("fail to open ip-cup-mapping-tabel");
	}
	else {
		while (!feof(temp)) {
			fscanf(temp, "%s %s %s\n", map[ipgross].ip, map[ipgross].cpu, map[ipgross].file);
			ipgross++;
		}
	}
	fclose(temp);
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

int main(int argc, char* argv[]){
	FILE * ipfile;
	char toip[30] = {0};
	char sendtoip[30] = {0};
	char toipopration[100] = {0};
	char opration[120] = {0};
	char setipopration[120] = {0};
	char toipfilea[50] = {0};
	char tofilea[50] = {0};
	char toipfileb[50] = {0};
	char tofileb[50] = {0};
	char settoipop[50] = {0};
	char hfilea[10] = {0};
	char hfileb[10] = {0};
	int i;
	int j;
	getmap();
	if (argc == 1) {
		for (i = 0; i < ipgross; i++) {
			printf("%s    ", map[i].file);
		}
		printf("\n");
	}
	else if (strcmp(argv[1], "cp") == 0) {
		//"file1/home/administrator/aaa.cpp" to "/home/administrator/aaa.cpp"
		strcpy(toipfilea, argv[2]);
		i = 0;
		while (toipfilea[i] != '/') {
			i++;
		}
		strcpy(hfilea, toipfilea);
		hfilea[i] = 0;
		for (j = 0; j < strlen(toipfilea); j++) {
			tofilea[j] = toipfilea[j + i];
		}
		//"file1/home/administrator/aaa.cpp" to "/home/administrator/aaa.cpp"
		strcpy(toipfileb, argv[3]);
		i = 0;
		while (toipfileb[i] != '/') {
			i++;
		}
		strcpy(hfileb, toipfileb);
		hfileb[i] = 0;
		for (j = 0; j < strlen(toipfileb); j++) {
			tofileb[j] = toipfileb[j + i];
		}
		filetoip(hfilea, toip);
		filetoip(hfileb, sendtoip);
		//get toip(when connect, it is fromip)
		//toip;
		//get send toip(when connect, it is toip)
		//sendtoip;
		//set toip(when connect, it is fromip)
		//send sendfile
		strcat(opration, sendopration);//sendopration
		strcat(opration, " ");
		strcat(opration, toip);
		strcat(opration, " ");
		strcat(opration, sendfile);//sendfile
		strcat(opration, " ");
		strcat(opration, tofilea);
		strcat(opration, " ");
		strcat(opration, sendtoip);
		strcat(opration, " ");
		strcat(opration, tofileb);//sendopration
		printf("\n%s\n", opration);
		system(opration);
	}
	else if (strcmp(argv[1], "rm") == 0) {
		//"file1/home/administrator/aaa.cpp" to "/home/administrator/aaa.cpp"
		strcpy(toipfilea, argv[2]);
		i = 0;
		while (toipfilea[i] != '/') {
			i++;
		}
		strcpy(hfilea, toipfilea);
		hfilea[i] = 0;
		for (j = 0; j < strlen(toipfilea); j++) {
			tofilea[j] = toipfilea[j + i];
		}
		filetoip(hfilea, toip);
		//send rm file
		strcat(opration, sendopration);//sendopration
		strcat(opration, " ");
		strcat(opration, toip);
		strcat(opration, " ");
		strcat(opration, argv[1]);//rm
		strcat(opration, " ");
		strcat(opration, tofilea);
		system(opration);
		printf("\n%s\n", opration);
	}
	else if (strcmp(argv[1], "mv") == 0) {
	}
	else {
		printf("\nthere is no command like this\n");
	}
	return 0;
} 
