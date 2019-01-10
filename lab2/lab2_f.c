#include<stdio.h>
#include<sys/sem.h>
#include<pthread.h>
#include<sys/types.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include <sys/shm.h>
#include<sys/wait.h>

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo* __buf;
};

int semid;	
int shmid;
int *count;
pid_t p1;
pid_t p2;
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
void *subp1(){
    *count = 0;
    for(int i=1; i<=100; i++){
        P(semid, 0);
        *count += i;
        V(semid, 1);
    }
    return NULL;
}
void *subp2(){
    for(int i=1; i<=100; i++){
        P(semid, 1);
        printf("%d ", *count);
        V(semid, 0);
    }
    return NULL;
}

int main(){
    int key, flag = 2;
    union semun arg;    
    key= ftok("./",'a');
    semid = semget(key, flag, IPC_CREAT|0666);

    shmid = shmget(key, sizeof(int), IPC_CREAT|0666);
    count = (int*)shmat(shmid, NULL, SHM_R|SHM_W);

    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);
    arg.val = 0;
    semctl(semid, 1, SETVAL, arg);

    p1 = fork();
	if(p1 == 0){
		//sub1
		subp1();
	}else{
		p2 = fork();
		if(p2 == 0){
			//sub2
			subp2();
		}else{
			//main
            p1 = wait(NULL);
            p2 = wait(NULL);
            semctl(semid, 0, IPC_RMID, arg);
            semctl(semid, 1, IPC_RMID, arg);
            shmctl(shmid,IPC_RMID,0);
		}	
	}
    return 0;
}
