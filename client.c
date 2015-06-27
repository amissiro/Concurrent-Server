//Azret Missirov
//29512170

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>

int sockfd;
struct sockaddr_in servaddr;
fd_set active_fd_set, read_fd_set;
char *commands[100];
void tokenizer(char * str)
{
    const char sepr[2] = ":";
    char *token = strtok(str, sepr);
    int i=0;

    while( token != NULL ){
        commands[i] = token;
        i++;
        token = strtok(NULL, sepr);
    }
}

void sockaddr_handler(char *host){
    bzero(&servaddr,sizeof servaddr);
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(12170);
    inet_pton(AF_INET,host,&(servaddr.sin_addr));
}

void socket_handler(){
    sockfd=socket(AF_INET,SOCK_STREAM,0);
}

void FD_handler(){
    //FD_ZERO (&active_fd_set);
    //FD_SET (sockfd, &active_fd_set);
   // FD_SET (0, &active_fd_set);
    
}

void client_handler(char *filename){
    int n=3;
    while(n>0){
    int i;
    char sendline[1000];
    write(sockfd,filename, strlen(filename)+1);
    char header[1000];
    int p;
     while(1){
         read(sockfd,header,1000);

          break;
      }
      
      printf("copying.....\n");

      tokenizer(header);
    
      if (strcmp(commands[0],"mkdir")==0){
        mkdir(commands[1],S_IRWXU | S_IRWXG | S_IRWXO);
      }
      else if (strcmp(commands[0],"file")==0)
      {
          
      int size = atoi(commands[2]);

      
      int fd = open(commands[1],O_CREAT|O_APPEND|O_RDWR,0644);
          int j=0;
          char recvline[256];
       while (j < size && n>0){
           bzero(recvline,256);
           n = read(sockfd,recvline,256);
           write(fd,recvline,strlen(recvline)+1);
           j = j+n;
     }
       close(fd);
  }
 }
}

int main(int argc,char *argv[])
{
    sockaddr_handler(argv[1]);
    socket_handler();
    FD_handler();
    
    connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    //while(1){
    client_handler(argv[2]);
    //}
    return 0;
}
