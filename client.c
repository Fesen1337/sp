#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include "network.h"

int main()
{
    int fd[2];
    //fd[0] - read
    //fd[1] - write
    if(pipe(fd) == -1)
    {
        printf("An error ocurre with opening the pipe\n");
        return 1;
    }
    int net_pid = fork();
    if(net_pid == 0)
    {
        char* buffer = (char*) malloc(1024);
        net(buffer, 1601);
        
        //printf("%s", buffer);

        net(buffer, 1602);
        write(fd[1], buffer, 1024);
        //printf("%s", buffer);
        
        free(buffer);
        
    }else{
        char* buffer = (char*) malloc(1024);
        read(fd[0], buffer, 1024);
        printf("msg=%s\n", buffer);
    }
    
}
