#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int main (int argc, char *argv[])
{
    int simpleSocket = 0, simplePort = 0, returnStatus = 0;
    char buffer[256] = "";
    struct sockaddr_in simpleServer;
    
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <server> <port>\n", argv[0]);
        exit(1);
    }

    simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if (simpleSocket == -1)
    {
        fprintf(stderr, "Unable to create socket\n");
        exit(1);
    }
    else
        fprintf(stderr, "Socket created\n");

    simplePort = atoi(argv[2]);
    memset(&simpleServer, 0, sizeof(simpleServer));
    simpleServer.sin_family = AF_INET;
    simpleServer.sin_port = htons(simplePort);
    // simpleServer.sin_addr.s_addr = inet_addr(argv[1]);
    // inet_pton(AF_INET, argv[1], &simpleServer.sin_addr
    returnStatus = inet_aton(argv[1], &simpleServer.sin_addr);
    if (returnStatus == 0)
    {
        fprintf(stderr, "Unable to use the address\n");
        close(simpleSocket);
        exit(1);
    }

    returnStatus = connect(simpleSocket, (struct sockaddr*) &simpleServer, sizeof(simpleServer));

    if (returnStatus == 0)
        fprintf(stderr, "Connection successful\n");
    else if(returnStatus == -1)
    {
        fprintf(stderr, "Unable to connect to address\n");
        close(simpleSocket);
        exit(1);
    }
    
    returnStatus = read(simpleSocket, buffer, sizeof(buffer));
    if(returnStatus > 0)
        printf("%d: %s", returnStatus, buffer);
    else
        fprintf(stderr, "Return status: %d\n", returnStatus);

    close(simpleSocket);

    return 0;
}