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

int getthisport() {
	int port;
	port = 10000;
	return port;
}

int main(){
	socklen_t tosocket, fromsocket;
	pid_t pid;
        struct sockaddr_in   to_addr, from_addr;
        
        bzero((char *)&from_addr,sizeof(from_addr));
        from_addr.sin_family = AF_INET;
        from_addr.sin_port = htons(0);
        from_addr.sin_addr.s_addr = htons(INADDR_ANY);
	fromsocket = socket(AF_INET,SOCK_STREAM,0);
	if (fromsocket < 0)
        	printf("fail to create local socket, socketid: %d\n", fromsocket);
	else
		printf("succeed to create local socket, socketid: %d\n", fromsocket);
        if (bind(fromsocket,(struct sockaddr *)&from_addr,sizeof(from_addr)) < 0)
        	printf("fail to bind local socket\n");
	else
		printf("succeed to bind local socket\n");
	//printf("server:   starting   listen   \n");
        //if   (listen(sockid,5)   <   0) 
		//while(1) {
		/*   ACCEPT   A   CONNECTION   AND   THEN   CREATE   A   CHILD   TO   DO   THE   WORK   */   
		/*   LOOP   BACK   AND   WAIT   FOR   ANOTHER   CONNECTION                                     */   
			//printf("server:   starting   accept\n");   
			//if   ((newsd   =   accept(sockid   ,(struct   sockaddr   *)   &client_addr, &clilen)) < 0)  
				//printf("server:   return   from   accept,   socket   for   this   ftp:   %d\n");
			//if   ((pid=fork()) == 0) {   
			/*   CHILD   PROC   STARTS   HERE.   IT   WILL   DO   ACTUAL   FILE   TRANSFER   */   
				//close(sockid);       /*   child   shouldn't   do   an   accept   */   
				//doftp(newsd);
				//close(newsd);   
				//exit(0);                   /*   child   all   done   with   work   */   
			//}
			/*   PARENT   CONTINUES   BELOW   HERE   */   
			//close(newsd);                 /*   parent   all   done   with   client,   only   child   */   
		//}                             /*   will   communicate   with   that   client   from   now   on   */   
} 
