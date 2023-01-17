#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <threadlogger.h>

sem_t tobacco;
sem_t paper;
sem_t matches;
sem_t place_on_table;

void* agent(void* arg){
	srand(time(NULL));
	while(true){
		int random = rand() % 3;
		if(random == 0){
			sem_post(paper);
			sem_post(matches);
		}
		else if(random == 1){
			sem_post(matches);
			sem_post(tobacco);
		}
		else{
			sem_post(tobacco);
			sem_post(paper);
		}
		sem_wait(place_on_table);
	}
}

void* smoker_tobacco(void* arg){
	while(true){
		sem_wait(paper);
		printSemaphoreEvent(SM_TOBACCO, PAPER, true);
		if(sem_try_wait(matches) == 0){
			printSemaphoreEvent(SM_TOBACCO, MATCHES, true);
			sem_post(place_on_table);
			printSmokingEvent(SM_TOBACCO);
			sleep(1);
		}
		else{
			sem_post(paper);
			printSemaphoreEvent(SM_TOBACCO, PAPER, false);
		}
	}
}

void* smoker_paper(void* arg){
	while(true){
		sem_wait(tobacco);
		printSemaphoreEvent(SM_PAPER, TOBACCO, true);
		if(sem_try_wait(matches) == 0){
			printSemaphoreEvent(SM_PAPER, MATCHES, true);
			sem_post(place_on_table);
			printSmokingEvent(SM_PAPER);
			sleep(1);
		}
		else{
			sem_post(tobacco);
			printSemaphoreEvent(SM_PAPER, TOBACCO, false);
		}
	}
}

void* smoker_matches(void* arg){
	while(true){
		sem_wait(tobacco);
		printSemaphoreEvent(SM_MATCHES, TOBACCO, true);
		if(sem_try_wait(paper) == 0){
			printSemaphoreEvent(SM_MATCHES, PAPER, true);
			sem_post(place_on_table);
			printSmokingEvent(SM_MATCHES);
			sleep(1);
		}
		else{
			sem_post(tobacco);
			printSemaphoreEvent(SM_MATCHES, TOBACCO, false);
		}
	}
}

int main(){

}