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
    fout = fopen("./hh", "wb");

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

    while(1){
        P(semid, 1);
        // fputs(info[point->out]->buf, fout);
        // printf("%d\n", point->temp);
        fwrite(info[point->out]->buf, 1, point->len[point->out], fout);
        point->out++;
        point->out %= 10;
        if(point->in == point->out && point->end == 1)
            break;
        V(semid, 0);
    }
    fclose(fout);
    return 0;
}