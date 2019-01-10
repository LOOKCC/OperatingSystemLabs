#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

int pipefd[2];
pid_t p1;
pid_t p2;

void sub1_print(){
	printf("Child Process l is Killed by Parent!\n");
	close(pipefd[0]);
	close(pipefd[1]);
	exit(0);
}
void sub2_print(){
	printf("Child Process 2 is Killed by Parent!\n");
	close(pipefd[0]);
	close(pipefd[1]);
	exit(0);
}
void sub1(){
	signal(SIGINT, SIG_IGN);
	signal(SIGUSR1, sub1_print);
	int count = 0;
	while(1){
		write(pipefd[1], &count, sizeof(int));
		count++;
		sleep(1);
	}
}
void sub2(){
	signal(SIGINT, SIG_IGN);
	signal(SIGUSR1, sub2_print);
	while(1){
		int count;
		read(pipefd[0], &count, sizeof(int));
		printf("I send you %d times.\n", count);
	}
}
void func(){
	kill(p1, SIGUSR1);
	kill(p2, SIGUSR1);
	wait(NULL);
	wait(NULL);
	printf("Parent Process is Killed!\n");
	close(pipefd[0]);
	close(pipefd[1]);
	exit(0);
}
int main(){
	pipe(pipefd);
	signal(SIGINT, func);
	p1 = fork();
	if(p1 == 0){
		//sub1
		sub1();
	}else{
		p2 = fork();
		if(p2 == 0){
			//sub2
			sub2();
		}else{
			//main
			while(1)
				sleep(1);
		}	
	}
	return 0;
}
