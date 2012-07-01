#include <stdio.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char curripfile[] = "/home/administrator/aaamynetwork/aaaconfig/currip";
char currtoipfile[] = "/home/administrator/aaamynetwork/aaaconfig/currtoip";

int main(int argc, char* argv[]) {
	char ip[30];
	FILE * temp;
	strcpy(ip, argv[1]);
	if ((temp = fopen(currtoipfile, "w")) < 0) {
		printf("\nfail to open ip file\n");
	}
	else {
		fprintf(temp, "%s", ip);
	}
	fclose(temp);
	return 0;
}

