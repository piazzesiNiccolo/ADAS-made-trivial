#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main()
{
    char buff[255], timer[30];
    time_t rawtime;
    struct tm *timeinfo;
    ///FILE *l = fopen("log/ecu.log", "w");
    //fputs("MACCHINA ACCESA\n", f);
    //fclose(l);
    FILE *f = fopen("log/ecu.log", "w+");
    fprintf(f,"MACCHINA ACCESA\n");
    fflush(f);
    if (f == NULL)
    {
        perror("impossibile aprire il file");
    }

    while (strcmp(buff, "PARCHEGGIO\n") != 0)
    {
        if (fgets(buff, sizeof(buff), f) != NULL)
        {
            rawtime = time(0);
            timeinfo = localtime(&rawtime);
            strftime(timer, sizeof(timer), "[%Y-%m-%d %H:%M:%S]", timeinfo);
            printf("%s ", timer);
            fputs(buff, stdout);
        }

        sleep(1);
    }
    printf("\n*************** MACCHINA PARCHEGGIATA,CHIUDERE QUESTA FINESTRA**********\n");
    pause();
}