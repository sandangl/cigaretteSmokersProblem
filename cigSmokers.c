#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t tobacco;
sem_t paper;
sem_t matches;
sem_t place_on_table;

void* agent_routine(void*arg){
	int random = rand() % 3;
	if(random == 0){
		printf("AGENT: put paper on table.\n");
		sem_post(&paper);
		printf("AGENT: put matches on table.\n");
		sem_post(&matches);
	}
	else if(random == 1){
		printf("AGENT: put matches on table.\n");
		sem_post(&matches);
		printf("AGENT: put tobacco on table.\n");
		sem_post(&tobacco);
	}
	else{
		printf("AGENT: put tobacco on table.\n");
		sem_post(&tobacco);
		printf("AGENT: put paper on table.\n");
		sem_post(&paper);
	}
	printf("AGENT: waiting for table to be cleared...\n");
	sem_wait(&place_on_table);
}

void* smoker_tobacco_routine(void* arg){
	printf("SMOKER_WITH_TOBACCO: waiting for paper...\n");
	sem_wait(&paper);
	printf("SMOKER_WITH_TOBACCO: took paper.\n");
	printf("SMOKER_WITH_TOBACCO: waiting for matches...\n");
	if(sem_trywait(&matches) == 0){
		printf("SMOKER_WITH_TOBACCO: took matches.\n");
		printf("SMOKER_WITH_TOBACCO: table cleared.\n");
		sem_post(&place_on_table);
		printf("SMOKER_WITH_TOBACCO: smoking...\n");
		sleep(5);
	}
	else{
		printf("SMOKER_WITH_TOBACCO: no matches, put back paper.\n");
		sem_post(&paper);
	}
}

void* smoker_paper_routine(void* arg){
	printf("SMOKER_WITH_PAPER: waiting for tobacco...\n");
	sem_wait(&tobacco);
	printf("SMOKER_WITH_PAPER: took tobacco.\n");
	printf("SMOKER_WITH_PAPER: waiting for matches...\n");
	if(sem_trywait(&matches) == 0){
		printf("SMOKER_WITH_PAPER: took matches.\n");
		printf("SMOKER_WITH_PAPER: table cleared.\n");
		sem_post(&place_on_table);
		printf("SMOKER_WITH_PAPER: smoking...\n");
		sleep(5);
	}
	else{
		printf("SMOKER_WITH_PAPER: no matches, put back tobacco.\n");
		sem_post(&tobacco);
	}
}

void* smoker_matches_routine(void* arg){
	printf("SMOKER_WITH_MATCHES: waiting for tobacco...\n");
	sem_wait(&tobacco);
	printf("SMOKER_WITH_MATCHES: took tobacco.\n");
	printf("SMOKER_WITH_MATCHES: waiting for paper...\n");
	if(sem_trywait(&paper) == 0){
		printf("SMOKER_WITH_MATCHES: took paper.\n");
		printf("SMOKER_WITH_MATCHES: table cleared.\n");
		sem_post(&place_on_table);
		printf("SMOKER_WITH_MATCHES: smoking...\n");
		sleep(5);
	}
	else{
		printf("SMOKER_WITH_MATCHES: no paper, put back tobacco.\n");
		sem_post(&tobacco);
	}
}

int main(){
	pthread_t agent, smoker_tobacco, smoker_paper, smoker_matches;
	
	//Semaphore initialization
	sem_init(&tobacco, 0, 0);
	sem_init(&paper, 0, 0);
	sem_init(&matches, 0, 0);
	sem_init(&place_on_table, 0, 0);
	
	srand(time(NULL));
	
	//thread creation
	pthread_create(&agent, NULL, &agent_routine, NULL);
	pthread_create(&smoker_tobacco, NULL, &smoker_tobacco_routine, NULL);
	pthread_create(&smoker_paper, NULL, &smoker_paper_routine, NULL);
	pthread_create(&smoker_matches, NULL, &smoker_matches_routine, NULL);
	
	//thread termination
	pthread_join(agent, NULL);
	pthread_join(smoker_tobacco, NULL);
	pthread_join(smoker_paper, NULL);
	pthread_join(smoker_matches, NULL);
	
	return 0;
}
