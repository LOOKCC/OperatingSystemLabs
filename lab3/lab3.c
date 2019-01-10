#include<stdio.h>
#include<sys/sem.h>
#include<pthread.h>
#include<sys/types.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include <sys/shm.h>
#include<sys/wait.h>
#include"total.h"

int main(){
    int key, num = 2;
    union semun arg; 
    key = ftok("/tmp", 'a');
    semid = semget(key, num, IPC_CREAT|0666);

    for(int i=0; i<10; i++){
        key= ftok("./",i+1);
        shmid[i] = shmget(key, sizeof(struct mem), IPC_CREAT|0666);
        info[i] = (struct mem*)shmat(shmid[i], NULL, SHM_R|SHM_W);        
    }

    key= ftok("./",'i');
    shmidio = shmget(key, sizeof(struct points), IPC_CREAT|0666);
    point = (struct points*)shmat(shmidio, NULL, SHM_R|SHM_W); 
    point->in = 0;
    point->out = 0;
    point->end = 0;
    for(int i=0; i<10; i++){
        point->len[i] = 0;
    }

    arg.val = 10;
    semctl(semid, 0, SETVAL, arg);
    arg.val = 0;
    semctl(semid, 1, SETVAL, arg);
    p1 = fork();
	if(p1 == 0){
		//sub1
		execv("./read", NULL);
	}else{
		p2 = fork();
		if(p2 == 0){
			//sub2
			execv("./write", NULL);
		}else{
			//main
            p1 = wait(NULL);
            p2 = wait(NULL);
            semctl(semid, 0, IPC_RMID, arg);
            semctl(semid, 1, IPC_RMID, arg);
            for(int i=0; i<10; i++){
                shmctl(shmid[i],IPC_RMID,0);
            }
            shmctl(shmidio,IPC_RMID,0);
		}	
	}
    return 0;
}
