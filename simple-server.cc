#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char *argv[]) 
{
    int simpleSocket = 0, simplePort = 0, returnStatus = 0;
    struct sockaddr_in simpleServer;
    

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);       
    }

    simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);   
    if (simpleSocket == -1)
    {
        fprintf(stderr, "Unable to create socket\n");
        exit(1);
    }
    else
    {
        fprintf(stderr, "Socket created\n");
    }

    simplePort = atoi(argv[1]);
    memset(&simpleServer, 0, sizeof(simpleServer));
    simpleServer.sin_family = AF_INET;
    simpleServer.sin_addr.s_addr = htonl(INADDR_ANY);
    simpleServer.sin_port = htons(simplePort);
    
    returnStatus = bind(simpleSocket, (struct sockaddr *) &simpleServer, sizeof(simpleServer));

    if (returnStatus == 0)
    {
        fprintf(stderr, "Bind completed\n");
    }
    else {
        fprintf(stderr, "Unable bind to address\n");
        close(simpleSocket);
        exit(1);
    }

    returnStatus = listen(simpleSocket, 5);
    if (returnStatus == 1)
    {
        fprintf(stderr, "Unable to listen on socket\n");
        close(simpleSocket);
        exit(1);
    }

    while(1)
    {
        const char MESSAGE[] = "Message for the client\n";
        struct sockaddr_in clientName = {0};
        int simpleChildSocket = 0;
        socklen_t clientNameLength = sizeof(clientName);

        simpleChildSocket = accept(simpleSocket, (struct sockaddr *)&clientName, &clientNameLength);
        if(simpleChildSocket == -1)
        {
            fprintf(stderr, "Unable to accept connections\n");
            close(simpleSocket);
            exit(1);
        }
        returnStatus = write(simpleChildSocket, MESSAGE, strlen(MESSAGE));
        if(returnStatus == -1)
            fprintf(stderr, "Unable to send message back to the client\n");

        close(simpleChildSocket);
    }
    
    close(simpleSocket);

    return 0;
}