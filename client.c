/*
Задание:
Генерация последовательности Фибоначчи:
на входе начальные параметры последовательности F0 и F1 и 
длина ряда. Формула ряда следующая: Fn = Fn-1 + Fn-2

Пример ряда: 1 1 2 3 5 8 13 21 34...
Каждое новое число ряда определяется как сумма двух предыдущих элементов ряда

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024 //размер буффера, в который будут записываться сообщения перед отправкой
#define SERVER_IP "127.0.0.1"//ip адрес по которому будет общение в нашем случае loopback
#define DEFAULT_PORT 1601//порт по которому создадим соединение

char* buffer;//буффер, в него будет записываться сообщение перед отправкой, и при получении
size_t buffer_size = 1024;//размер буффера в переменной а не через define, я хз зачем я сделал два разных определения длины буффера

void init(int n, int k);//не нужно, забыл удалить
void print(int n, int k);//не нужно, забыл удалить
void itoa(int n, char s[]);//перевод числа в строку
void reverse(char s[]);//реверс строки, нужен для работы itoa
int is_num(char *arr);//проверка является ли строка числом

int main(int argc, char* argv[])
{
    int n, f0, f1;//параметры по заданию
    while(1)
    {

        int client = socket(AF_INET, SOCK_STREAM, 0);//создадим сокет клиента, возвращает дескриптор сокета !!
        struct sockaddr_in server_address;//структура адреса!!
        buffer = (char *)malloc(buffer_size * sizeof(char));//БУФФЕР!
        if(client < 0)//если не удалось создать сокет, выведем ошибку
        {
            printf("ERROR: Establishing socket error.\n");
            exit(0);
        }
        //заполним поля адреса
        server_address.sin_port = htons(DEFAULT_PORT);//установим порт!!
        server_address.sin_family = AF_INET;//семейство адресов!!
        inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);//преобразует адреса ipv4 ipv6 из текстового вида в двоичный 
        printf("LOG: Client socket created\n");

        int ret = connect(client, (struct sockaddr *)&server_address, sizeof(server_address));//устанавливаем соединение с сервером 0 - успешно, -1 нет !!
        int q = 0;
        while(ret == -1)//если неуспешно, будем пробовать до тех пор пока не подключимся
        {
            ret = connect(client, (struct sockaddr *)&server_address, sizeof(server_address));
            q++;
            if(q > 100000)
            {
                printf("ERROR: Server is not responding\n");//c каким то перерывом будем писать что серв не отвечает
                return 0;

            }   
                
        }

        recv(client, buffer, BUFFER_SIZE, 0);//получаем сообщение!! 
        printf("LOG: Connection established.\n");

        strcpy(buffer, "Connection established");
        send(client, buffer, BUFFER_SIZE, 0);//отправляем сообщение!!

        while(1)
        {
            //строки для параметров, именно строки, т.к. их легче проверять на адекватность введенных данных 
            strcpy(buffer, "hello");
            send(client, buffer, BUFFER_SIZE, 0);//отправляем сообщение!!
            recv(client, buffer, BUFFER_SIZE, 0);
            printf("%s", buffer);
        }
    }


}

void itoa(int n, char s[])//хехе
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

void reverse(char s[])//хаха
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

int is_num(char *arr)//проверка на то, является ли строка числом
{
    int err = 0;
    for(int i = 0; i < strlen(arr); i++)
    {
        if(arr[i] < '0' || arr[i] > '9')
            return 0;
    }
    return 1;
}