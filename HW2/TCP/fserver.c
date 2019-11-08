#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h> /* memset */
#include <unistd.h> /* close */
#include <string.h>
#include <time.h>
#include <assert.h>
//TCPserver
#define BUFFER 1024
void error_handling(char *message);
void time_taken();

int main(int argc, char **argv){

    int serv_sock;
    int clnt_sock;
	FILE *fp;

    if(argc != 2){ 
        printf("Usage : %s <port_num>\n", argv[0]);
        exit(1);
            } 
   

	char message[BUFFER]={0};
    int str_len;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int clnt_addr_size;
    serv_sock=socket(PF_INET,SOCK_STREAM, 0); 
    if(serv_sock == -1) 
        error_handling("socket() error");

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
        error_handling("bind() error");
    
    if(listen(serv_sock,5)==-1)
        error_handling("listen() error");

    clnt_addr_size = sizeof(clnt_addr);
	while(1)
	{
	clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
	printf("Start : ");//start time
	time_taken();

    if(clnt_sock==-1)
        error_handling("accept() error");
	while((str_len = recv(clnt_sock,message,BUFFER,0)) != 0){
		fp=fopen(message,"wb"); //receive file name and create file
		break;	
	}
	
	while((str_len = recv(clnt_sock,message,BUFFER,0)) > 0){
		printf("Server : %d\n",str_len);	
		fwrite(message,1,str_len,fp); //write data to file.
	}
	fclose(fp);
	close(clnt_sock);
	
	printf("File Received!\n");
	printf("End time: ");
	time_taken();//end time

	printf("Waiting for another client...\n");
	}
	return 0;

}

void error_handling(char *message)

{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);


}
void time_taken(){
	
	time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    assert(strftime(s, sizeof(s), "%c", tm));
    printf("%s\n", s);
	
}




    



