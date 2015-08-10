#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/shm.h>
#include "shm_com.h"
#include "../sem/sem.h"

int main()
{
	csem sem(1234);
	
	FILE* pfile;

	int running = 1;

	void *shared_memory = (void*)0;
	struct shared_use_st *shared_stuff;

	char buffer[BUFSIZ];
	int shmid;

	shmid = shmget( (key_t)1234,sizeof(struct shared_use_st), 0666 | IPC_CREAT);
	if ( -1 == shmid)
	{
		fprintf(stderr, "shmget failed \n");
		exit(EXIT_FAILURE);

	}

	shared_memory = shmat(shmid, (void*)0, 0);
	if( (void*)0 == shared_memory )
	{
		fprintf( stderr, "shmat failed \n");
			exit(EXIT_FAILURE);	
	}

	printf("memory attched at %x \n", (int)shared_memory);

	shared_stuff = (struct shared_use_st*) shared_memory;

	pfile = fopen(__FILE__, "r");
	if(pfile == NULL)
	{
		printf("open %s failed \n", __FILE__);
		exit(1);
	}

	while(running)
	{
	/*	while(shared_stuff->written_by_you == 1)
		{
			sleep(1);
			printf("waiting for client... \n");
		}
	*/

	//	printf("enter some text: \n");
	//	fgets(buffer, BUFSIZ, stdin);

		if ( NULL == fgets(buffer, BUFSIZ, pfile) )
		{
			printf("read %s file over", __FILE__);

			strncpy(shared_stuff->some_text, "end", TEXT_SZ);
			running  = 0;
		}
		printf("%s", buffer);
		sem.sem_p();
		if( shared_stuff->written_by_you == 0 )
		{
			strncpy(shared_stuff->some_text, buffer, TEXT_SZ);
			shared_stuff->written_by_you = 1;
		}
		sem.sem_v();
		if(strncmp(buffer, "end", 3) == 0)
		{
			running = 0;
		}
	}

	fclose(pfile);
	if( shmdt(shared_memory) == -1)
	{
		fprintf(stderr, "shmdt failed \n");
		exit(-1);
	}
	exit(EXIT_SUCCESS);
}
