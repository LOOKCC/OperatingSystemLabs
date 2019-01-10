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
    fin = fopen("./out", "rb");

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
        P(semid, 0);
        int temp = 0;
        temp = fread(info[point->in]->buf, 1, 20, fin);
        if( temp < 20){
            point->len[point->in] = temp;
            point->in++;
            point->in %= 10;
            V(semid, 1);
            point->end = 1;
            // printf("end in read: %d\n", point->end);
            break;
        }
        point->in++;
        point->in %= 10;
        point->len[point->in] = temp;
        // info[point->in]->buf[temp] = 0;
        V(semid, 1);
    }
    fclose(fin);
    return 0;
}