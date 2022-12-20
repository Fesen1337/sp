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
    char* buffer = (char*) malloc(1024);
    net(buffer);
    printf("%s", buffer);
    free(buffer);
}
