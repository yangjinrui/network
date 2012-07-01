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
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char quitop[] = "quitthishost";
char rsignal[] = "aaaa";
char control[] = "control";
char follow[] = "follow";

char curripfile[] = "/home/administrator/aaamynetwork/aaaconfig/currip";
char currtoipfile[] = "/home/administrator/aaamynetwork/aaaconfig/currtoip";
char controlip[] = "/home/administrator/aaamynetwork/aaaconfig/controlip";
char followip[] = "/home/administrator/aaamynetwork/aaaconfig/followip";
char otherip[] = "/home/administrator/aaamynetwork/aaaconfig/otherip";
char ipc[] = "/home/administrator/aaamynetwork/aaaconfig/ipc";
char ipscope[] = "/home/administrator/aaamynetwork/aaaconfig/ipscope";
char connectpid[] = "/home/administrator/aaamynetwork/aaaconfig/connectpid";
char ipcputable[] = "/home/administrator/aaamynetwork/aaaconfig/ipcputable";
char thisis[] = "/home/administrator/aaamynetwork/aaaconfig/thisis";

char setip[] = "/home/administrator/aaamynetwork/setip";
char settoip[] = "/home/administrator/aaamynetwork/settoip";
char sendconnect[] = "/home/administrator/aaamynetwork/sendconnect";
char sendopration[] = "/home/administrator/aaamynetwork/sendopration";
char infotoctr[] = "/home/administrator/aaamynetwork/infotoctr";
char recvip[] = "/home/administrator/aaamynetwork/recvip";
char sendip[] = "/home/administrator/aaamynetwork/sendip";
char setctrip[] = "/home/administrator/aaamynetwork/setctrip";

void dtoa(int num, char sn[30]) {
	int d = num;
	bzero(sn, 30);
	char sna[30];
	int i, j;
	i = 0;
	while (d / 10 != 0) {
		sna[i] = '0' + d % 10;
		d = d / 10;
		i++;
	}
	sna[i] = '0' + d;
	for (j = 0; j <= i; j++) {
		sn[j] = sna[i - j];
	}
	printf("%d %s %s\n", num, sn, sna);
}

void getcurrip(char currip[30]) {
	bzero(currip, 30);
	FILE * temp;
	if ((temp = fopen(curripfile, "r")) < 0)
		printf("fail to open current ip file");
	else {
		fscanf(temp, "%s", currip);
	}
	fclose(temp);
}

int main(int argc, char* argv[]) {
	int i, ipnum, num;
	char sbeginip[30];
	char sendip[30];
	char cip[30];//controler ip
	char toip[30];
	char currip[30];
	char spid[10];
	char input[10];
	unsigned long currentip, ncurrentip;
	char scurrentip[30];
	unsigned long nbeginip, nendip, beginip, endip;
	in_addr ina;
	pid_t process;
	int childpid;
	char op[100];
	FILE * temp;
	FILE * cpid;
	FILE * ctripf;
	FILE * tempb;
	//set ipc file value as 0, ipc: save count of computers which has same system
	if ((temp = fopen(ipc, "w")) < 0){
		printf("fail to open ipc file");
	}
	else {
		num = 0;
		fprintf(temp, "%d", num);
	}
	fclose(temp);
	if ((temp = fopen(ipscope, "r")) < 0) {
		printf("\nfail to open ip scope file\n");
		exit(1);
	}
	fscanf(temp, "%s %s", sbeginip, sendip);
	printf("beginip: %s  endip: %s\n", sbeginip, sendip);
	fclose(temp);
	nbeginip = inet_addr(sbeginip);
	nendip = inet_addr(sendip);
	beginip = ntohl(nbeginip);
	endip = ntohl(nendip);
	printf("beginip: %x  endip: %x\n", beginip, endip);
	ipnum = endip - beginip + 1;
	printf("ipnum: %d\n", ipnum);
	i = 0;
			if ((cpid = fopen(connectpid, "w")) < 0) {
				printf("\nfail to open connect pid file\n");
				exit(1);
			}
			fclose(cpid);
	for (currentip = beginip; currentip <= endip; currentip++) {
		bzero(scurrentip, 30);
		bzero(op, 100);
		ncurrentip = htonl(currentip);
		ina.s_addr = ncurrentip;
		strcpy(scurrentip, inet_ntoa(ina));
		process = fork();
		if (process == 0) {
			if ((cpid = fopen(connectpid, "a+")) < 0) {
				printf("\nfail to open connect pid file\n");
				exit(1);
			}
			childpid = getpid();
			fprintf(cpid, "%d\n", childpid);
			fclose(cpid);
			//printf("%d\n", childpid);
			strcat(op, sendconnect);
			strcat(op, " ");
			strcat(op, scurrentip);
			//system(op);
			execl(sendconnect, sendconnect, scurrentip, NULL);
			exit(0);
		}
		else if (process < 0) {
			printf("\nfail to fork\n");
		}
		i++;
	}
	sleep(10);//wait child process (connect) to do their job
	if ((cpid = fopen(connectpid, "r")) < 0) {
		printf("\nfail to open connect pid file\n");
		exit(1);
	}
	//fprintf(cpid, "%d\n", childpid);
	while (!feof(cpid)) {
		bzero(op, 100);
		strcat(op, "kill ");
		bzero(spid, 10);
		fscanf(cpid, "%s\n", spid);
		strcat(op, spid);
		system(op);
		printf("%s\n", op);
	}
	fclose(cpid);
	//clear connectpid
	if ((cpid = fopen(connectpid, "w+")) < 0) {
		printf("\nfail to open connect pid file\n");
		exit(1);
	}
	fclose(cpid);
	if ((ctripf = fopen(controlip, "a+")) < 0) {
		printf("\nfail to open controler ip file\n");
		exit(1);
	}
	fscanf(ctripf, "%s", cip);
	fclose(ctripf);
	if (cip[0] == 0) {//there is no controler
		printf("\nThere is no controller computer now.\n");
		printf("\nSet this computer as controller?<y/n>\n");
		bzero(input, 10);
		scanf("%s", input);
		while (input[0] != 'y' && input[0] != 'Y' && input[0] != 'n'&& input[0] != 'N') {
				printf("\nWrong answer!\n");
				printf("\nThere is no controller computer now.\n");
				printf("\nSet this computer as controller?<y/n>\n");
				bzero(input, 10);
				scanf("%s", input);
		}
		if (input[0] == 'y' || input[0] == 'Y') {
			//write to controlip file
			getcurrip(currip);
			bzero(op, 100);
			strcat(op, setctrip);
			strcat(op, " ");
			strcat(op, currip);
			system(op);
			//write to thisis file
			if ((temp = fopen(thisis, "w")) < 0) {
				printf("\nfail to open thisis file\n");
				exit(1);
			}
			fprintf(temp, "%s", control);
			fclose(temp);
			//settable: write to ipcputable
			if ((temp = fopen(ipcputable, "w")) < 0) {
				printf("\nfail to open thisis file\n");
				exit(1);
			}
			fclose(temp);
			if ((temp = fopen(ipcputable, "a+")) < 0) {
				printf("\nfail to open thisis file\n");
				exit(1);
			}
			fprintf(temp, "%s cpu0 file0\n", currip);
			//tell other computer this is controler
			if ((tempb = fopen(followip, "r")) < 0) {
				printf("\nfail to open follow ip file\n");
				exit(1);
			}
			i = 1;
			while (!feof(tempb)) {
				bzero(toip, 30);
				fscanf(tempb, "%s\n", toip);
				fprintf(temp, "%s cpu%d file%d\n", toip, i, i);
				i++;
				bzero(op, 100);
				strcat(op, sendopration);
				strcat(op, " ");
				strcat(op, toip);
				strcat(op, " ");
				strcat(op, setctrip);
				strcat(op, " ");
				strcat(op, currip);
				system(op);
			}
			fclose(tempb);
			fclose(temp);
			bzero(op, 100);
			strcat(op, recvip);
			strcat(op, " &");
			system(op);//recv ips other follow computer send
		}
		else {
			//write to thisis file
			if ((temp = fopen(thisis, "w")) < 0) {
				printf("\nfail to open thisis file\n");
				exit(1);
			}
			fprintf(temp, "%s", follow);
			fclose(temp);
		}
	}
	else {//there is a controler
		//write to thisis file
		if ((temp = fopen(thisis, "w")) < 0) {
			printf("\nfail to open thisis file\n");
			exit(1);
		}
		fprintf(temp, "%s", control);
		fclose(temp);
		//send own ip
		bzero(op, 100);
		system(sendip);//send own ip to controler
	}
	return 0;
}
