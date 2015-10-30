#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>



int main(int argc, char **argv)
{

	if(argc<2)
	{
		printf("Format is ./driver  sim_filename");
		exit(0);
	}


	char *sim_file;
	sim_file=(char *)malloc(sizeof(char)*strlen(argv[1]));
	strcpy(sim_file,argv[1]);
	FILE *fp;

	fp = fopen(sim_file, "r+");
	char * line = NULL;
	size_t len = 0;
	ssize_t read;

	if (fp == NULL)
               exit(EXIT_FAILURE);

        while ((read = getline(&line, &len, fp)) != -1) 
	{
		//printf("Retrieved line of length %zu :\n", read);
		printf("%s", line);
		char* token = strtok(line, " ");
		while (token) {
			//printf("token: %s\n", token);
			printf("Token length is %d\n",(unsigned int)strlen(token));		
			if(strncmp(token,"cracker",strlen("cracker"))==0)
				printf("Mode is cracker");
			token = strtok(NULL, " ");
		}
	}

	fclose(fp);
        if (line)
		free(line);



}
