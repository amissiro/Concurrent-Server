//Azret Missirov
//29512170
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/select.h>
#include <stdio.h>
#include <strings.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define PORT 12170

fd_set active_fd_set, read_fd_set;
int main_socket,my_accept;
char *commands[100];
struct sockaddr_in main_sockaddr;


void copy_com(char* filename,int i);
void penetrateDir (char *dir,int n);

void tokenizer(char * str)
{
    const char sepr[2] = " ";
    char *token = strtok(str, sepr);
    int i=0;
    
    while( token != NULL ){
        commands[i] = token; i++;
        token = strtok(NULL, sepr);
    }
    commands[i] = NULL;
}

int create_socket (uint16_t port)
{
    struct sockaddr_in sockaddr;
    int my_socket = socket(PF_INET, SOCK_STREAM, 0);
    
    if (my_socket < 0)
    {
        perror ("Socket Error");
        exit (EXIT_FAILURE);
    }
    
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons (port);
    sockaddr.sin_addr.s_addr = htonl (INADDR_ANY);
    
    if (bind (my_socket, (struct sockaddr *) &sockaddr, sizeof (sockaddr)) < 0)
    {
        perror ("Bind Error");
        exit (EXIT_FAILURE);
    }
    return my_socket;
}

//********************************isFile()*************************************
int isFile (char* name)
{
    struct stat buf;
    stat(name, &buf);
    return S_ISREG(buf.st_mode);
}

//********************************isDir()*************************************
int isDir (char* name)
{
    struct stat buf;
    stat(name, &buf);
    return S_ISDIR(buf.st_mode);
}

char *rem_dots(char * fol)
{
    if (fol[0]=='.' && fol[1]=='.')
    {
        return fol+3;
    }
    return fol;
}
//**************************************************************************
void create_com (char *d_name,int i)
{
 
      char* mkd = "mkdir:";
      char * comm = (char *) malloc(1 + strlen(mkd)+ strlen(rem_dots(d_name)));
      strcpy(comm, mkd);
      strcat(comm, rem_dots(d_name));
      write(i, comm, strlen(comm)+1);
      free(comm);
    sleep(1);
}

//**************************************************************************
char *pathmaker (char *dir, char *d_name)
{
    char * allocpath  = malloc(strlen(dir) + strlen(d_name)+10000);
    strcpy(allocpath, dir);
    strcat(allocpath, "/");
    strcat(allocpath, d_name);
    return allocpath;
}
//**************************************************************************
void loop1(DIR *dirp,struct dirent *dr, char * dir, int i){
    while(NULL != (dr = readdir(dirp))){
        if (strcmp(dr->d_name,".")!=0 && strcmp(dr->d_name,"..")!=0){
            char *path = pathmaker(dir, dr->d_name);
            if (isFile(path)==1) 
               copy_com(path,i);
            if (isDir(path)==1)
               create_com(path,i);               
            free(path);
        }
    }
}

//**************************************************************************

void loop2(DIR *dirp,struct dirent *dr, char * dir, int i)
{
    while(NULL != (dr = readdir(dirp))){
        if (strcmp(dr->d_name,".")!=0 && strcmp(dr->d_name,"..")!=0){
            char *path = path = pathmaker(dir, dr->d_name);
            if (isDir(path)==1) 
              penetrateDir(path,i);
            free(path);
            
        }
    }
}

//**************************************************************************
void penetrateDir (char *dir,int n)
{
    DIR *dirp = NULL;
    struct dirent *dr = NULL;
    if ((dirp = opendir(dir))==NULL) 
       printf("Permission denied\n");
    else
    {  
        create_com(dir,n);
        loop1(dirp,dr,dir,n); 
        dirp = opendir(dir);
        loop2(dirp,dr,dir,n); 
        closedir(dirp);
    }
    
}

//****************************************************************************

//****************************************************************************
void copy_com(char* filename,int i){
       int fd, still;
        fd = open(filename, O_RDONLY);
        struct stat buf;
        fstat(fd, &buf);
  
        int size = buf.st_size;
        //char *content = (char*)malloc(size);
        char content[256];
        char a[1000];
        sprintf(a, "file:%s:%d",rem_dots(filename),size);
        write(i, a, strlen(a)+1);
        //sleep(1);
        while((still=read(fd,content,256-10))>0){
            int v = write(i, content, still);
        }
        sleep(1);
        close(fd);
}


void clients_handler (){
    int i;
    for (i = 0; i < FD_SETSIZE; ++i){
        if (FD_ISSET (i, &read_fd_set)){
            if (i == main_socket){
                socklen_t size = sizeof (main_sockaddr);
                my_accept= accept (main_socket,(struct sockaddr *) &main_sockaddr,&size);
                FD_SET(my_accept,&active_fd_set);
            }
            else{
                char str[100];
                read(i,str,100);
               if (isFile(str)==1){
                copy_com(str,i);
               }
               else if (isDir(str)==1){
                penetrateDir(str,i);
                close(i);
               }
            }
        
        }
    }
}

int main (int args, char *argv[]){ 

    printf("Server is running....\n");
    main_socket = create_socket (PORT);
    listen (main_socket, 5);
    
    FD_ZERO (&active_fd_set);
    FD_SET (main_socket, &active_fd_set);

    signal(SIGPIPE,SIG_IGN);
    int pid = fork();
    while (1)
    {
        
        if (pid==0){
        read_fd_set = active_fd_set;
        select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL);
        
        clients_handler();
            //exit(0);
        }
        else if (pid < 0) exit(-1);
        else while(wait(NULL)>0);
    }
    
    close(main_socket);
    
}
