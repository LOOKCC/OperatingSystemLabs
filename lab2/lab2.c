#include<stdio.h>
#include<sys/sem.h>
#include<pthread.h>
#include<sys/types.h>

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo* __buf;
};

int semid;	
pthread_t p1,p2;
int count = 0;
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
    for(int i=1; i<=100; i++){
        P(semid, 0);
        count += i;
        V(semid, 1);
    }
}
void *subp2(){
    for(int i=1; i<=100; i++){
        P(semid, 1);
        printf("%d ", count);
        V(semid, 0);
    }
}

int main(){
    int key, flag = 2;
    union semun arg; 
    key= ftok("./",'a');
    semid = semget(key, flag, IPC_CREAT|0666);
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);
    arg.val = 0;
    semctl(semid, 1, SETVAL, arg);
    pthread_create(&p1, NULL, subp1, NULL);
    pthread_create(&p2, NULL, subp2, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    semctl(semid, 0, IPC_RMID, arg);
    semctl(semid, 1, IPC_RMID, arg);
    return 0;
}
