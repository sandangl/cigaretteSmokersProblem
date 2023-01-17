#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t tobacco;
sem_t paper;
sem_t matches;
sem_t place_on_table;

void* agent(void* arg){
	while(true){

	}
}

void* smoker_tobacco(void* arg){
	while(true){
		sem_wait(paper);
		fprint("TOBACCO SMOKER: acquired paper.\n");
		if(sem_try_wait(matches) == 0){
			fprint("TOBACCO SMOKER: acquired matches.\n");
			sem_post(place_on_table);
			fprint("TOBACCO SMOKER: table_cleared.\n");
			fprint("TOBACCO SMOKER: smoking...\n");
			sleep(1);
		}
		else{
			sem_post(paper);
			fprint("TOBACCO SMOKER: dropped paper.\n");
		}
	}
}

void* smoker_paper(void* arg){

}

void* smoker_matches(void* arg){

}

int main(){

}