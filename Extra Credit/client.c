/*[client]
	author: Farhan Hyder
	CSC 332 - HW4 (Extra Credit)
	Date: 12/16/2018
*/


#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#define RCVBUFSIZE 32   /* Size of receive buffer */
#define PORT 8080

int sock;                        /* Socket descriptor */
int toExit = 0;

//initialize mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* prototypes */
void printMessage(char *echoBuffer);
void *listenToServer();

int main(int argc, char *argv[])
{
    struct sockaddr_in ServAddr;     /* server address */
    unsigned short ServPort;         /* server port */
    char *servIP;                    /* Server IP address (dotted quad) */
    char *echoString;                /* String to send to server */
    char echoBuffer[RCVBUFSIZE];     /* Buffer for echo string */
    unsigned int echoStringLen;      /* Length of string to echo */
	int rc, i;
	pthread_t listenToServerThread;
	pthread_attr_t attr;
	void *status;

    servIP = "127.0.0.1";            /* First arg: server IP address (dotted quad) */

    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
        printf("socket() failed\n");
		exit(-1);
    }

    /* Construct the server address structure */
    memset(&ServAddr, 0, sizeof(ServAddr));     /* Zero out structure */
    ServAddr.sin_family      = AF_INET;             /* Internet address family */
    ServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    ServAddr.sin_port        = htons(PORT); /* Server port */

    /* establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) < 0)
	{
        printf("connect() failed\n");
		exit(-1);
    }
   
    /* initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	//create thread that accepts new connections
	rc = pthread_create(&listenToServerThread, &attr, listenToServer, NULL);
	if (rc)
	{
		//failed to create the thread
		printf("[ERROR] return code from pthread_create() is %d\n", rc);
		exit(-1);
	}

    while (toExit != 1)
	{
    	scanf("%s", echoBuffer);

    	echoStringLen = strlen(echoBuffer);

    	if (strcmp(echoBuffer, "exit") == 0)
    		toExit = 1;

    	if (send(sock, echoBuffer, echoStringLen, 0) != echoStringLen)
        	printf("send() sent a different number of bytes than expected");
    }

   	close(sock);
    pthread_attr_destroy(&attr);

	rc = pthread_join(listenToServerThread, &status);
	if (rc) 
	{
		//failed  to join the thread
		printf("ERROR; return code from pthread_join() is %d\n", rc);
		exit(-1);
	}

    exit(0);
}

/* print received message from server */
void printMessage(char *echoBuffer)
{
	int length = strlen(echoBuffer), i = 0;
	int semicolonPosition = 0;
	char id[2], songName[RCVBUFSIZE];

	for (i = 0; i < length; i++)
	{
		if (echoBuffer[i] == ';')
		{
			semicolonPosition = i;
			break;
		}
	}

	strncpy(id, echoBuffer, semicolonPosition);
	strncpy(songName, echoBuffer+semicolonPosition+1, length-semicolonPosition);

	if (strcmp(id, "-1") == 0)
		printf("\nMusic Player started\n\n");
	else if(strcmp(id, "0") == 0)
		printf("Song changed by Music Player\n");
	else
		printf("Song changed by user%s\n", id);

	printf("Playing: %s\n", songName);
	memset(id, 0, sizeof id);
	memset(songName, 0, sizeof songName);
}

/* this function is used to receive messages from server */
void *listenToServer()
{
	char echoBuffer[RCVBUFSIZE];     /* Buffer for echo string */
    unsigned int echoStringLen;      /* Length of string to echo */
	int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv() 
                                        and total bytes read */
	while (toExit != 1){
		if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
		{
			printf("Communication with server has been stopped\n");
			exit(-1);
		}

		if (bytesRcvd > 0)
		{
			echoBuffer[bytesRcvd] = '\0';  /* terminate the string! */
        	printMessage(echoBuffer);
		}

		memset(echoBuffer, 0, sizeof echoBuffer);
	}
}
