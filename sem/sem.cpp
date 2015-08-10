#include "sem.h"
#include "semun.h"
csem::csem(int keyid)
{
	m_semid = 0;

	if (!create_sem(keyid) )
	{
		printf("create sem failed \n");
		exit(EXIT_FAILURE);
	}

	if( !init_sem() )
	{
		printf("init sem failed \n");
		exit(EXIT_FAILURE);
	}
}

csem::~csem()
{
	del_sem();
}

bool csem::create_sem(int keyid)
{
	m_semid = semget( (key_t)(keyid), 1, IPC_CREAT | 0666);
	if( -1 == m_semid )
	{
		return false;
	}
	return true;
}

bool csem::init_sem()
{
	union semun sem_arg;
	sem_arg.val = 1;
	if( -1 == semctl(m_semid, 0, SETVAL, sem_arg) )
	{
		printf("inti sem failed \n");
		//exit(EXIT_FAILURE);
		return false;
	}
	return true;
}

void csem::del_sem()
{
	if( semctl(m_semid, 0, IPC_RMID) == -1 )
	{
		printf("%s : can not rm the sem \n");
	}
}

void  csem::sem_p()
{
	struct sembuf sem_arg;
	sem_arg.sem_num = 0;
	sem_arg.sem_op = -1;
	sem_arg.sem_flg = SEM_UNDO;

	if( -1 == semop(m_semid, &sem_arg, 1) )
	{
		printf("%s:can not do the sem_p",__func__);
		
	}
}

void csem::sem_v()
{
	struct sembuf sem_arg;
	sem_arg.sem_num = 0;
	sem_arg.sem_op = 1;
	sem_arg.sem_flg = SEM_UNDO;

	if( -1 == semop(m_semid, &sem_arg, 1) )
	{
		printf("%s:can not do the sem_v",__func__);
	}	
}
