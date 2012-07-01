#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char interface[] = "/etc/network/interfaces";
char networking[] = "/etc/init.d/networking";

char curripfile[] = "/home/administrator/aaamynetwork/aaaconfig/currip";
char ipscope[] = "/home/administrator/aaamynetwork/aaaconfig/ipscope";

int main(int argc, char* argv[]) {
	char buf[50];
	char op[50];
	char address[30];
	char gateway[30];
	char netmask[30];
	char sbeginip[30];
	char sendip[30];
	char input[10];
	FILE * face;
	FILE * temp;
	printf("\n\n\n");
	//get ip from interfaces
	chmod(interface, 7777);
	if ((face = fopen(interface, "r")) < 0) {
		printf("\nfail to open interfaces file\n");
		exit(1);
	}
	while (!feof(face)) {
		bzero(buf, 50);
		fgets(buf, 50, face);
		if (buf[0] != '#' && strcmp(buf, "\n") != 0 && strcmp(buf, "") != 0) {
			printf("%s", buf);
		}
	}
	printf("\n");
	fclose(face);
	chmod(interface, 0777);
	//write to /etc/network/interfaces
	printf("This is network interfaces now.\nInput c to change them, other for no change and system will continu\n");
	bzero(input, 10);
	scanf("%s", input);
	if (input[0] == 'c') {
		bzero(address, 30);
		printf("address: ");
		scanf("%s", address);
		bzero(netmask, 30);
		printf("netmask: ");
		scanf("%s", netmask);
		bzero(gateway, 30);
		printf("gateway: ");
		scanf("%s", gateway);
		printf("Please make sure your input is right.\nIf you want to change, input c. Input other for no.\n");
		printf("Or you can rewrite in /etc/network/interfaces when you get into this system after net-init.\n");
		bzero(input, 10);
		scanf("%s", input);
		while (input[0] == 'c') {
			bzero(address, 30);
			printf("address: ");
			scanf("%s", address);
			bzero(netmask, 30);
			printf("netmask: ");
			scanf("%s", netmask);
			bzero(gateway, 30);
			printf("gateway: ");
			scanf("%s", gateway);
			printf("Please make sure your input is right.\nIf you want to change, input c. Input other for no.\n");
			printf("Or you can rewrite in /etc/network/interfaces when you get into this system after net-init.\n");
			bzero(input, 10);
			scanf("%s", input);
		}
		if ((face = fopen(interface, "w")) < 0) {
			printf("\nfail to open interfaces file\n");
			exit(1);
		}
		fclose(face);
		if ((face = fopen(interface, "a+")) < 0) {
			printf("\nfail to open interfaces file\n");
			exit(1);
		}
		bzero(op, 50);
		strcat(op, "auto eth0");
		fprintf(face, "%s\n", op);
		bzero(op, 50);
		strcat(op, "iface eth0 inet static");
		fprintf(face, "%s\n", op);
		bzero(op, 50);
		strcat(op, "address ");
		strcat(op, address);
		fprintf(face, "%s\n", op);
		bzero(op, 50);
		strcat(op, "netmask ");
		strcat(op, netmask);
		fprintf(face, "%s\n", op);
		bzero(op, 50);
		strcat(op, "gateway ");
		strcat(op, gateway);
		fprintf(face, "%s\n", op);
		fclose(face);
	//write to current ip file
	if ((temp = fopen(curripfile, "w")) < 0) {
		printf("\nfail to open current ip file\n");
	}
	fprintf(temp, "%s", address);
	fclose(temp);
	}
	//  /etc/init.d/networking restart
	bzero(op, 50);
	strcat(op, networking);
	strcat(op, " restart");
	system(op);//restart network and changes are used
	//write to IP scope file
	if ((temp = fopen(ipscope, "r")) < 0) {
		printf("\nfail to open ip scope file\n");
		exit(1);
	}
	bzero(sbeginip, 30);
	bzero(sendip, 30);
	fscanf(temp, "%s %s", sbeginip, sendip);
	fclose(temp);
	if (sbeginip[0] == 0) {
		printf("\nIP scope has not been set util now!\n");
		printf("IP scope should be large enough to contain all IPs of computers which you want to use to build a cluser, but the smaller is better because this computer will connect with everyone in the scope when system initing.\n");
		printf("Please sen IP scope(example 192.168.1.45-192.168.1.64):\n");
		bzero(sbeginip, 30);
		bzero(sendip, 30);
		scanf("%s-%s", sbeginip, sendip);
		printf("Please make sure your input is right.\nIf you want to change, input c then enter. Input other key for no.\n");
		printf("Or you can rewrite in /home/administrator/aaamynetwork/aaaconfig/ipscope when you get into this system after net-init.\n");
		bzero(input, 10);
		scanf("%s", input);
		while (input[0] == 'c') {
			printf("IP scope should be large enough to contain all IPs of computers which you want to use to build a cluser, but the smaller is better because this computer will connect with everyone in the scope when system initing.\n");
			printf("Please sen IP scope(example 192.168.1.45-192.168.1.64):\n");
			bzero(sbeginip, 30);
			bzero(sendip, 30);
			scanf("%s-%s", sbeginip, sendip);
			printf("Please make sure your input is right.\nIf you want to change, input c then enter. Input other key for no.\n");
			printf("Or you can rewrite in /home/administrator/aaamynetwork/aaaconfig/ipscope when you get into this system after net-init.\n");
			bzero(input, 10);
			scanf("%s", input);
		}
		if ((temp = fopen(ipscope, "w")) < 0) {
			printf("\nfail to open ip scope file\n");
			exit(1);
		}
		fprintf(temp, "%s %s", sbeginip, sendip);
		fclose(temp);
	}
	else {
		printf("\nIP scope is frome %s to %s now.\n", sbeginip, sendip);
		printf("IP scope should be large enough to contain all IPs of computers which you want to use to build a cluser, but the smaller is better because this computer will connect with everyone in the scope when system initing.\n");
		printf("If you want to change IP scope, input c. Input other key for no.\n");
		bzero(input, 10);
		scanf("%s", input);
		if (input[0] == 'c') {
			printf("Please sen IP scope(example 192.168.1.45-192.168.1.64):\n");
			bzero(sbeginip, 30);
			bzero(sendip, 30);
			scanf("%s-%s", sbeginip, sendip);
			printf("Please make sure your input is right.\nIf you want to change, input c then enter. Input other key for no.\n");
			printf("Or you can rewrite in /home/administrator/aaamynetwork/aaaconfig/ipscope when you get into this system after net-init.\n");
			bzero(input, 10);
			scanf("%s", input);
			while (input[0] == 'c') {
				printf("Please sen IP scope(example 192.168.1.45-192.168.1.64):\n");
				bzero(sbeginip, 30);
				bzero(sendip, 30);
				scanf("%s-%s", sbeginip, sendip);
				printf("Please make sure your input is right.\nIf you want to change, input c then enter. Input other for no.\n");
				printf("Or you can rewrite in /home/administrator/aaamynetwork/aaaconfig/ipscope when you get into this system after net-init.\n");
				bzero(input, 10);
				scanf("%s", input);
			}
			if ((temp = fopen(ipscope, "w")) < 0) {
				printf("\nfail to open ip scope file\n");
				exit(1);
			}
			fprintf(temp, "%s %s", sbeginip, sendip);
			fclose(temp);
		}
	}
	return 0;
}
