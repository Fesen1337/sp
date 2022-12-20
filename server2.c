#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h> 
#include <math.h>

#define BUFFER_SIZE 1024 
#define SERVER_IP "127.0.0.1"
#define DEFAULT_PORT 1602
#define CLIENT_CLOSE_CONNECTION_SYMBOL '#'

int client, server;
int servs[100];
char buffer[BUFFER_SIZE];
void* clientHandler(void* server);
int is_num(char *str);
void itoa(int n, char s[]);
void reverse(char s[]); 
void task_func(int f0, int f1, int n, int*ser);
void* server_routine();

int main(int argc, char* argv[])
{
    pthread_t server;
    pthread_create(&server, NULL, &server_routine, NULL);
    pthread_yield(server);
    char c;
    while(1)
    {
        scanf("%c", &c);
        if(c == '#')
        {
            printf("SERVER LOG: server off.\n");
            close(client);
            return 0;
        }
    }

}

void *server_routine()
{
    pthread_t t[100];
    int last_handle = 0;

    struct sockaddr_in server_address;
    client = socket(AF_INET, SOCK_STREAM, 0);
    if(client < 0)
    {
        printf("SERVER ERROR: establishing socket error.");
        exit(0);
    }

    printf("SERVER LOG: socket for server was successfully created\n");

    
    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);

    int ret = bind(client, (struct sockaddr *)&server_address, sizeof(server_address));
    if(ret < 0)
    {
        printf("SERVER ERROR: binding connection. Socket was already been establishing");
        return 0;

    }   

    socklen_t size = sizeof(server_address);
    printf("SERVER LOG: Listening clients... \n");
    listen(client, SOMAXCONN); 

    while(1)
    {
        server = accept(client, (struct sockaddr *)&server_address, &size); 
        
        if(server < 0)
        {
            printf("SERVER ERROR: Cant accepting client.\n");
        }
        servs[last_handle] = server;
        pthread_create(&t[last_handle], NULL, &clientHandler, &servs[last_handle]);
        pthread_yield(t[last_handle]);
        last_handle++;
    }
}

void* clientHandler(void* server)
{
    int *ser = (int*)server;
    char buf[BUFFER_SIZE];
    int n, f0, f1;//параметры необходимые для вычисления ряда
    printf("Hello, client №%d!\n", *ser);   
    recv(*ser, buf, BUFFER_SIZE, 0);
    printf("%s", buf);
    strcpy(buf, "eeeeee");
    send(*ser, buf, BUFFER_SIZE, 0);
    
}

void task_func(int f0, int f1, int n, int *ser)
{
    char buf[1024];//ну уже не смешно, буффер это
    itoa(n, buf);//переводим n в строку
    send(*ser, buf, BUFFER_SIZE, 0);//отправляем клиенту количество пакетов, которое он будет должен получить
    recv(*ser, buf, BUFFER_SIZE, 0);//получаем подтверждения от клиента о получении сообщения

    int res[1000];//сюда запишем ряд фибоначчи

    res[0] = f0;//первое число ряда 
    res[1] = f1;//второе число ряда 
    for(int i = 2; i < n; i++)
    {
        res[i] = res[i - 1] + res[i - 2];//вычисление iго числа ряда
    }

    for(int i = 0; i < n; i++)
    {
        itoa(res[i], buf);//переводим i-ое число ряда в строку
        send(*ser, buf, BUFFER_SIZE, 0);//отправляем его клиенту
        recv(*ser, buf, BUFFER_SIZE, 0);//получаем подтвержение получения
    }
    
}

void itoa(int n, char s[])//перевод числа в строку (украдено) позаимствовано с stackoverflow
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

void reverse(char s[])//реверс строки (украдено) позаимствовано с stackoverflow
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

