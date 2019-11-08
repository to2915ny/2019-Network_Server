#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h> /* memset */
#include <unistd.h> /* close */
#include <string.h>
#include <signal.h>
//UDPserver
#define BUFSIZE 4096
void error_handling(char *message);
void timeout_handler(int signal);
int main(int argc, char **argv){

	int serv_sock;
	int str_len, num=0;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;
	FILE *fp;

	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}  
	while(1){
		char message[BUFSIZE] = {0};
		serv_sock=socket(PF_INET, SOCK_DGRAM, 0); 
		if(serv_sock == -1) 
			error_handling("UDP socket not made");
		memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family=AF_INET;
		serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
		serv_addr.sin_port=htons(atoi(argv[1]));
		if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
			error_handling("bind() error");
		clnt_addr_size=sizeof(clnt_addr);
		signal(SIGALRM,timeout_handler);//signal handler
		while(1){
			recvfrom(serv_sock,message,BUFSIZE,0,(struct sockaddr*)&clnt_addr,&clnt_addr_size);//receive filename
			if(message[0] != '\0')//buffer contains something == filename is received
			{   
				sendto(serv_sock,"received filename...",strlen("received filename..."),0,(struct sockaddr*)&clnt_addr,clnt_addr_size);
				//send ACK to client
				printf("Received Filename : %s from Client!\n",message);
				break;
			}   

		}   
		alarm(6);//start alarm for 6secs if program is not executed for another 6 secs then signal.
		fp=fopen(message,"wb");

		while(1)
		{   
			num = recvfrom(serv_sock,message,BUFSIZE,0,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
			printf("Sever : %d\n ", num);

			if(num < BUFSIZE){

				fwrite(message,1,num,fp);

				break;
			}

			fwrite(message,1,num,fp);

		}

		printf("File received\n");
		printf("Waiting for another client...\n");

		alarm(0);//disable alarm
		fclose(fp);
		close(serv_sock);
	}
	return 0;
}
void error_handling(char *message)

{
	fputs(message,stderr);
	fputc('\n',stderr);
	exit(1);


}
void timeout_handler(int signal){
	printf("Timeout!\n");
	exit(1);

}

