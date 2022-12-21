#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>

#include "network.h"

void itoa(int n, char s[]);
void reverse(char s[]);

int main()
{
    int main_pid = getpid();
    int net_w[2];
    int net_main_w[2];
    //fd[0] - read
    //fd[1] - write
    if(pipe(net_w) == -1)
    {
        printf("An error ocurre with opening the pipe\n");
        return 1;
    }
    if(pipe(net_main_w) == -1)
    {
        printf("An error ocurre with opening the pipe\n");
        return 1;
    }
    int net_pid = fork();
    if(net_pid == 0)
    {
        char* buffer = (char*) malloc(1024);
        char arg[100];
        while(1)
        {
            
            read(net_main_w[0], arg, 100);
            net(buffer, atoi(arg));
            
            write(net_w[1], buffer, 1024);
            
        }
        
        free(buffer);
        
    }else{
        char* buffer = (char*) malloc(1024);
        //if(render_pid == 0)
        //{
        //    printf("pid before exec = %d\n", getpid());
        //      execl("./test", NULL);
        if(getpid() == main_pid){
            char command[100];
            int arg;
            int tabs[100];
            int last_free_tab = 0;
            while(1)
            {
                printf("enter com: ");
                scanf("%s", command);
                if(command[0] == 'i') // info
                {
                    //
                }else if(command[0] == 'e') // exit
                {
                    for(int i = 0; i < last_free_tab; i++)
                    {
                        kill(tabs[i], SIGKILL);
                    }
                    free(buffer);
                    return 0;
                }else{
                    printf("enter arg: ");
                    scanf("%d", &arg);
                    if(command[1] == 'l') // close tab
                    {
                        kill(tabs[arg], SIGKILL);
                    }else{ // connect
                        printf("1\n");
                        char arg_str[100];
                        itoa(arg, arg_str);
                        printf("2\n");
                        write(net_main_w[1], arg_str, 100);
                        printf("3\n");
                        
                        printf("4\n");
                        read(net_w[0], buffer, 1024);
                        printf("5\n");
                        printf("msg=%s\n", buffer);
                        
                        int new_tab = fork();
                        if(new_tab == 0)
                        {
                            execl("./test", "buffer", NULL);
                        }else{
                            tabs[last_free_tab] = new_tab;
                            last_free_tab++;
                        }
                    }
                }
            }
        }
    }
    
}

void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  
        n = -n;          
    i = 0;
    do {       
        s[i++] = n % 10 + '0';   
    } while ((n /= 10) > 0);
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) 
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}