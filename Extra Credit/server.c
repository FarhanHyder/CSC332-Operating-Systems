/*[server]
	author: Farhan Hyder
	CSC 332 - HW4 (Extra Credit)
	Date: 12/16/2018
*/

#include <stdlib.h>
#include <stdio.h>			// FILE, fopen
#include <arpa/inet.h>		// socket operations
#include <string.h>
#include <unistd.h>			// usleep, close
#include <pthread.h>		// pthread operations
#define MAXPENDING 5    /* maximum outstanding connection requests */
#define RCVBUFSIZE 32   /* size of receive buffer */
#define PORT 8080

struct Song
{
	char name[25];
	int totalPlayingTime;
	int duration;
	struct Song *nextSong;
	struct Song *prevSong;
};

struct Songs
{
	struct Song *firstSong;
	struct Song *lastSong;
	struct Song *currentSong;
	int numberOfSongs;
};

/* global variables */
int clntSock[2];                 /* socket descriptor for client */
int closedSockets[2];
int clientsConnected = 0;
int disconnectedUsers = 0;
int currentPlayingTime = 0;

//initialize mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int withinCriticalArea = 0;

struct Songs *songs;

/* prototypes */
void addSong(char *line);
void readPlaylist();
void *acceptConnections();
void playMusic();
void nextSong(int id);
void prevSong(int id);
void sendMessageToClients(char id[2]);
void updateMusicTxt();
void updatePlayingTime(char *line);
void writeMusicTxt();
void *listenToClients(void *t);



int main(int argc, char *argv[])
{
	readPlaylist();
	
	int rc, i;
	pthread_t acceptConnectionsThread, listenToClientsThreads[2];
	pthread_attr_t attr;
	void *status;

	/* Initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	// create thread that accepts new connections
	rc = pthread_create(&acceptConnectionsThread, &attr, acceptConnections, NULL);
	if (rc)
	{
		//failed to create the thread
		printf("ERROR; return code from pthread_create() is %d\n", rc);
		exit(-1);
	}

	while (clientsConnected < 2)
		usleep(500);

	/* receive message from clients */
	for (i = 0; i < clientsConnected; i++)
	{

		// create thread that accepts new connections
		rc = pthread_create(&listenToClientsThreads[i], &attr, listenToClients, (void *)(intptr_t)i);
		if (rc)
		{
			// failed to create the thread
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	playMusic();

	/* free attribute and wait for the other threads */
	pthread_attr_destroy(&attr);

	rc = pthread_join(acceptConnectionsThread, &status);
	if (rc)
	{
		// failed  to join the thread
		printf("ERROR; return code from pthread_join() is %d\n", rc);
		exit(-1);
	}

	for (int i = 0; i < clientsConnected; i++){
		rc = pthread_join(listenToClientsThreads[i], &status);
		if (rc)
		{
			// failed  to join the thread
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
	}
	
	updateMusicTxt();
    return 0;
}

void addSong(char *line)
{
	/* split line by tab and extract song name and duration. Then create a song struct
	and add it to song list */
	int length = strlen(line), i = 0;
	int tabPosition = 0;
	char songName[25], duration[20];

	for (i = 0; i < length; i++)
	{
		if (line[i] == '\t')
		{
			tabPosition = i;
			break;
		}
	}

	strncpy(songName, line, tabPosition);
	strncpy(duration, line+tabPosition+1, length-tabPosition);
	struct Song *song = (struct Song*)malloc(sizeof(struct Song));
	strcpy(song->name, songName);

	song->totalPlayingTime = 0;
	song->duration = atoi(duration);
	song->nextSong = NULL;

	if (songs->numberOfSongs == 0)
	{
		songs->firstSong = song;
		songs->lastSong = song;
	}
	else
	{
		songs->lastSong->nextSong = song;
		song->prevSong = songs->lastSong;
		songs->lastSong = song;
	}

	songs->numberOfSongs += 1;
}

/* this function is called when both users are disconnected. It's
used to update the playing times for each song */
void updatePlayingTime(char *line)
{
	int length = strlen(line), i = 0;
	int tabPosition = 0;
	char songName[25], playingTime[20];

	for (i = 0; i < length; i++)
	{
		if (line[i] == '\t')
		{
			tabPosition = i;
			break;
		}
	}

	strncpy(songName, line, tabPosition);
	strncpy(playingTime, line+tabPosition+1, length-tabPosition);

	int intPlayingTime = atoi(playingTime);
	struct Song *currentSong = songs->firstSong;

	for (int i = 0; i < songs->numberOfSongs; i++)
	{
		if (strcmp(currentSong->name, songName) == 0)
			currentSong->totalPlayingTime += intPlayingTime;

		currentSong = currentSong->nextSong;
	}
}

/* read playlist and create the song list */
void readPlaylist()
{
	FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    songs = (struct Songs*)malloc(sizeof(struct Songs));
    songs->firstSong = NULL;
    songs->lastSong = NULL;
    songs->numberOfSongs = 0;

    fp = fopen("playlist.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    read = getline(&line, &len, fp);
    while ((read = getline(&line, &len, fp)) != -1)
    	addSong(line);

    songs->lastSong->nextSong = songs->firstSong;
    songs->firstSong->prevSong = songs->lastSong;

    fclose(fp);
    if (line)
        free(line);
}

/* read music txt and update playing times */
void updateMusicTxt(){
	printf ("Updating music txt file...\n");
	FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
   
    fp = fopen("music.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    read = getline(&line, &len, fp);
    while ((read = getline(&line, &len, fp)) != -1) 
    	updatePlayingTime(line);

    writeMusicTxt();

    fclose(fp);
    if (line)
        free(line);
}

/* write the updated songs to music.txt file */
void writeMusicTxt()
{
	printf("Writing to music txt...\n");
	FILE *fp;
	char lineToWrite[30], playingTime[10];
	fp = fopen("music.txt", "w+");
	fputs("Song Name\tPlaying Time\n", fp);

	struct Song *currentSong = songs->firstSong;

	for (int i = 0; i < songs->numberOfSongs; i++)
	{
		strcpy(lineToWrite, currentSong->name);
		strcat(lineToWrite, "\t");
		sprintf(playingTime, "%d", currentSong->totalPlayingTime);
		strcat(lineToWrite, playingTime);
		strcat(lineToWrite, "\n");
		fputs(lineToWrite, fp);
		currentSong = currentSong->nextSong;
		memset(lineToWrite, 0, sizeof lineToWrite);
		memset(playingTime, 0, sizeof playingTime);
	}
	
	fclose(fp);	
}

/* this function is used to accept connections. It only accepts 2 connections */
void *acceptConnections()
{
	int servSock;                    /* Socket descriptor for server */
	struct sockaddr_in ServAddr;     /* Local address */
	struct sockaddr_in ClntAddr;     /* Client address */
	unsigned short ServPort;         /* Server port */
	unsigned int clntLen;            /* Length of client address data structure */

	/* create socket for incoming connections */
    if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
        printf("socket() failed\n");
		exit(-1);
    }

    /* construct local address structure */
    memset(&ServAddr, 0, sizeof(ServAddr));   /* Zero out structure */
    ServAddr.sin_family = AF_INET;                /* Internet address family */
    ServAddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    ServAddr.sin_port = htons(PORT);      /* Local port */

    int one = 1;
	setsockopt(servSock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

    /* bind to the local address */
    if (bind(servSock, (struct sockaddr *) &ServAddr, sizeof(ServAddr)) < 0)
	{
        printf("bind() failed\n");
		exit(-1);
    }

    /* mark the socket so it will listen for incoming connections */
    if (listen(servSock, MAXPENDING) < 0)
	{
        printf("listen() failed\n");
		exit(-1);
    }

    while (1){
    	clntLen = sizeof(ClntAddr);

        /* Wait for a client to connect */
        if ((clntSock[clientsConnected] = accept(servSock, (struct sockaddr *) &ClntAddr, &clntLen)) < 0)
		{
            printf("accept() failed\n");
	    	exit(-1);
		}
		closedSockets[clientsConnected] = 0;
		clientsConnected++;
		/* clntSock is connected to a client! */
        printf("Connection accepted for client #%d\n", clientsConnected);

        if (clientsConnected == 2)
        	break;
    }
}

/* this function updates playing time of current song and changes song if neccessary */
void playMusic()
{
	printf ("Playing music...\n");
	songs->currentSong = songs->firstSong;
	sendMessageToClients("-1");

	while (disconnectedUsers != 2)
	{
		if (withinCriticalArea) continue;

		if (songs->currentSong->duration < currentPlayingTime)
			nextSong(0);
		else
		{
			//create the critical region
			pthread_mutex_lock(&mutex);
			printf("Playing: %s, Time: %d\n", songs->currentSong->name, currentPlayingTime);
			currentPlayingTime++;
			songs->currentSong->totalPlayingTime += 1;
			pthread_mutex_unlock(&mutex);
			usleep(1000000);
		}	
	}

	printf ("Music Player stopped\n");
}

/* this function is used to receive messages from clients */
void *listenToClients(void *t)
{
	int id = (intptr_t)t;
	char echoBuffer[RCVBUFSIZE];     /* Buffer for echo string */
	int recvMsgSize;                 /* Size of received message */

	while (closedSockets[id] != 1)
	{
		if ((recvMsgSize = recv(clntSock[id], echoBuffer, RCVBUFSIZE, 0)) < 0)
		{
		    	printf("recv() failed");
			    exit(-1);
		}

		if (recvMsgSize > 0)
		{

			if (strcmp(echoBuffer, "next") == 0)
				nextSong(id+1);
	
			else if (strcmp(echoBuffer, "previous") == 0)
				prevSong(id+1);

			else if (strcmp(echoBuffer, "exit") == 0)
			{
				printf ("Server stopped listening to user%d\n", (id+1));
				disconnectedUsers += 1;
				closedSockets[id] = 1;
				close(clntSock[id]);    /* Close client socket */
			}

			memset(echoBuffer, 0, sizeof echoBuffer);
		}
	}
}

void nextSong(int id)
{
	char charID[2];
	sprintf(charID, "%d", id);
	//create the critical region
	pthread_mutex_lock(&mutex);
	withinCriticalArea = 1;
	printf ("Next Song\n");
	songs->currentSong = songs->currentSong->nextSong;
	sendMessageToClients(charID);
	currentPlayingTime = 0;
	withinCriticalArea = 0;
	pthread_mutex_unlock(&mutex);
}

void prevSong(int id)
{
	char charID[2];
	sprintf(charID, "%d", id);
	//create the critical region
	pthread_mutex_lock(&mutex);
	withinCriticalArea = 1;
	printf ("Previous Song\n");
	songs->currentSong = songs->currentSong->prevSong;
	sendMessageToClients(charID);
	currentPlayingTime = 0;
	withinCriticalArea = 0;
	pthread_mutex_unlock(&mutex);
}

/* send message to clients that contains the id of the user that
made a change and the current song's name */
void sendMessageToClients(char id[2])
{
	char message[20];
	memset(message, 0, sizeof message);
	strcat(message, id);
	strcat(message, ";");
	strcat(message, songs->currentSong->name);
	int messageLen = strlen(message);

	for (int i = 0; i < clientsConnected; i++)
	{
		if (closedSockets[i] == 1)
			continue;
		
		if (send(clntSock[i], message, messageLen, 0) != messageLen)
		{
			printf("send() failed");
		  	exit(-1);
	    }
	}
}
