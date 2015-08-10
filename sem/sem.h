#ifndef SEM_H
#define SEM_H

#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <stdio.h>
//信号量封装
class csem
{
public:
	csem(int keyid);

	~csem();

	void sem_p();

	void sem_v();

private:
	bool create_sem(int keyid);
	
	bool init_sem();

	void del_sem();

	int m_semid;

};


#endif
