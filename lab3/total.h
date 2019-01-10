#include<stdio.h>
#include<sys/sem.h>
#include<pthread.h>
#include<sys/types.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include <sys/shm.h>
#include<sys/wait.h>

struct points{
    int in;
    int out;
    int end;
    int len[10];
};

struct mem{
    char buf[20];
};

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo* __buf;
};

void P(int semid, int index)
{	   
    struct sembuf sem;	
    sem.sem_num = index;
    sem.sem_op = -1;	
    sem.sem_flg = 0; //操作标记：0或IPC_NOWAIT等
    semop(semid,&sem,1);	//1:表示执行命令的个数	
    return;
}
void V(int semid, int index)
{	
	struct sembuf sem;	
    sem.sem_num = index;
    sem.sem_op =  1;
    sem.sem_flg = 0;	
    semop(semid,&sem,1);	
    return;
}
FILE* fout;
FILE* fin;
int semid;	
int shmid[10];
int shmidio;
struct mem* info[10];
struct points* point;
pid_t p1;
pid_t p2;