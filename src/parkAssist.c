#include <stdio.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    FILE *p = fopen("/dev/urandom", "r");
    FILE *log = fopen("assist.log", "w");
    char data[4];
	 if (p == NULL || log == NULL)
        {
            perror("errore in apertura file");
            exit(-1);
            /* code */
	}
	int n=0;
	while(fread(data,1,4,p)==4 && n<10) {
		int c= atoi(data);
		printf("%d",c);
		//fwrite(&c,sizeof(int),1, log);
		fprintf(log, "%d",c);

	//prova con 10 termini
		n=n+1;
	//sleep(1); 

   }
   fclose(log);
   fclose(p);
    
    //exit(0);
}
