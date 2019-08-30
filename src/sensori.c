#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "SocketConnection.h"
#include "sensori.h"
void frontWindshield()
{
    int ecu;
    char d[255], data[255];
    strcpy(d, "PERICOLO\n");
    FILE *wscIN = fopen("input/frontCamera.data", "r");
    FILE *wscOUT = fopen("log/camera.log", "w");
    if (wscIN == NULL || wscOUT == NULL)
    {
        perror("error not open fileINPUT");
        exit(EXIT_FAILURE);
    }
    while (fgets(data, sizeof(data), wscIN) != NULL)
    {
        if (strcmp(data, d) == 0)
        {
            kill(getppid(), SIGUSR1);
            fputs(data, wscOUT);
            fflush(wscOUT);
            sleep(2);
        }
        else
        {
            ecu = connectToServer(".ecu");
            send(ecu, data, sizeof(data), 0);
            close(ecu);
            fputs(data, wscOUT);
            fflush(wscOUT);
            sleep(10);
        }
    }
    fclose(wscOUT);
    fclose(wscIN);
    pause();
}
void parkAssist(int mode)
{
    FILE *p;
    unsigned char data[4];
    int secs = 0;
    if (mode == 0)
    {
        p = fopen("/dev/urandom", "r");
    }
    else
    {
        p = fopen("input/urandomARTIFICIALE.binary", "r");
    }

    FILE *log = fopen("log/assist.log", "w");
    int ecuServer;
    if (p == NULL || log == NULL)
    {
        perror("errore in apertura file");
        exit(EXIT_FAILURE);
    }
    for (secs = 0; secs < 30; secs++)
    {

        if (fread(data, 1, 4, p) < 4)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < 4; i++)
        {
            fprintf(log, "%d ", (int)data[i]);
        }
        fputc('\n', log);
        fflush(log);
        sleep(1);
        ecuServer = connectToServer(".ecu");
        if (send(ecuServer, data, 4, 0) < 0)
        {
            perror("send");
            exit(EXIT_FAILURE);
        }
        close(ecuServer);
    }
    ecuServer = connectToServer(".ecu");
    if (send(ecuServer, "FINE\n", 6, 0) < 0)
    {
        perror("send");
        exit(EXIT_FAILURE);
    }
    close(ecuServer);
    fclose(log);
    fclose(p);
    pause();
}
void forwardFacing(int mode)
{
    unsigned char data[24];
    FILE *p;
    FILE *log = fopen("log/radar.log", "a");
    int ecuServer;
    if (mode == 0)
    {
        p = fopen("/dev/urandom", "r");
    }
    else
    {
        p = fopen("input/urandomARTIFICIALE.binary", "r");
    }
    if (p == NULL || log == NULL)
    {
        perror("errore in apertura file");
        exit(EXIT_FAILURE);
    }
    for (;;)
    {
        if (fread(data, 1, 24, p) == 24)
        {
            ecuServer = connectToServer(".ecu");
            if (send(ecuServer, data, 24, 0) < 0)
            {
                perror("send");
                exit(EXIT_FAILURE);
            }
            close(ecuServer);
            for (int i = 0; i < 24; i++)
            {
                fprintf(log, "%d ", (int)data[i]);
            }
            fputc('\n', log);

            fflush(log);
        }
        sleep(2);
    }
}
void surroundViews(int mode)
{
    unsigned char data[16];
    FILE *p;
    FILE *log = fopen("log/cameras.log", "w");
    if (mode == 0)
    {
        p = fopen("/dev/urandom", "r");
    }
    else
    {
        p = fopen("input/urandomARTIFICIALE.binary", "r");
    }
    int ecuServer;
    for (int secs = 0; secs < 30; secs++)
    {
        //
        if (fread(data, 1, 16, p) < 16)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
        ecuServer = connectToServer(".ecu");
        if (send(ecuServer, data, strlen(data), 0) < 0)
        {
            perror("send");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < 16; i++)
        {
            fprintf(log, "%d ", (int)data[i]);
        }
        fputc('\n', log);
        fflush(log);
        sleep(1);
    }
    fclose(log);
    fclose(p);
    pause();
}

void blindSpot(int mode)
{
    unsigned char data[8];
    FILE *p = fopen("/dev/urandom", "r");
    FILE *log = fopen("log/spot.log", "a");
    int ecuServer;
    for (;;)
    {

        for (int secs = 0; secs < 4; secs++)
        {

            if (fread(data, 1, 8, p) < 8)
            {
                perror("read");
                exit(EXIT_FAILURE);
            }
            /** ecuServer = connectToServer("ecu");
        if (send(ecuServer, data, strlen(data), 0) < 0)
        {
            perror("send");
            exit(EXIT_FAILURE);
        }*/
            for (int i = 0; i < 8; i++)
            {
                fprintf(log, "%d ", (int)data[i]);
            }
            fflush(log);
            sleep(1);
        }
        kill(getpid(), SIGSTOP); //aspetta segnale da steer
    }
    fclose(log);
    fclose(p);
}