#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "SocketConnection.h"
int parkLog()
{ // di base fa questo,va migliorato e testato
	FILE *p = fopen("/dev/urandom", "r");
	FILE *log = fopen("assist.log", "a");
	char data[4];
	int rng = 0, charCount = 0;
	int ecuServer;
	ecuServer = connectToServer("ecu");
	if (p == NULL || log == NULL)
	{
		perror("errore in apertura file");

		/* code */
	}
	for (int secs = 0; secs < 30; secs++)
	{

		for (int i = 0; i < 4; i++)
		{
			rng = getc(p); /* code */
			data[i] = rng;
			charCount += fprintf(log, "%d ", rng);
		}
		send(ecuServer, data, strlen(data), 0);
		fprintf(log, "\n");
		sleep(1);
	}
	fclose(log);
	fclose(p);
	return charCount;
}
