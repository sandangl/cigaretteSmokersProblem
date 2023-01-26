#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t tobacco;
sem_t paper;
sem_t matches;
sem_t table_cleared;
pthread_t agent;
pthread_t smoker_tobacco;
pthread_t smoker_paper;
pthread_t smoker_matches;

void* agent_routine(void*arg){
	while(1){
		int random = rand() % 3;
		if(random == 0){
			printf("AGENT: put paper on table.\nAGENT: put matches on table.\n");
			sem_post(&paper);
			sem_post(&matches);
		}
		else if(random == 1){
			printf("AGENT: put matches on table.\nAGENT: put tobacco on table.\n");
			sem_post(&matches);
			sem_post(&tobacco);
		}
		else{
			printf("AGENT: put tobacco on table.\nAGENT: put paper on table.\n");
			sem_post(&tobacco);
			sem_post(&paper);
		}
		printf("AGENT: waiting for table to be cleared...\n");
		sem_wait(&table_cleared);
	}
}

void* smoker_tobacco_routine(void* arg){
	while(1){
		printf("SMOKER_WITH_TOBACCO: waiting for paper...\n");
		sem_wait(&paper);
		printf("SMOKER_WITH_TOBACCO: took paper.\n");
		printf("SMOKER_WITH_TOBACCO: waiting for matches...\n");
		if(sem_trywait(&matches) == 0){
			printf("SMOKER_WITH_TOBACCO: took matches.\n");
			printf("SMOKER_WITH_TOBACCO: table cleared.\n");
			sem_post(&table_cleared);
			printf("SMOKER_WITH_TOBACCO: smoking...\n");
			sleep(1);
		}
		else{
			printf("SMOKER_WITH_TOBACCO: no matches, put back paper. Waiting for table to be refreshed...\n");
			sem_post(&paper);
			usleep(500000);
		}
	}
}

void* smoker_paper_routine(void* arg){
	while(1){
		printf("SMOKER_WITH_PAPER: waiting for tobacco...\n");
		sem_wait(&tobacco);
		printf("SMOKER_WITH_PAPER: took tobacco.\n");
		printf("SMOKER_WITH_PAPER: waiting for matches...\n");
		if(sem_trywait(&matches) == 0){
			printf("SMOKER_WITH_PAPER: took matches.\n");
			printf("SMOKER_WITH_PAPER: table cleared.\n");
			sem_post(&table_cleared);
			printf("SMOKER_WITH_PAPER: smoking...\n");
			sleep(1);
		}
		else{
			printf("SMOKER_WITH_PAPER: no matches, put back tobacco.\n");
			sem_post(&tobacco);
			usleep(500000);
		}
	}
}

void* smoker_matches_routine(void* arg){
	while(1){
		printf("SMOKER_WITH_MATCHES: waiting for tobacco...\n");
		sem_wait(&tobacco);
		printf("SMOKER_WITH_MATCHES: took tobacco.\n");
		printf("SMOKER_WITH_MATCHES: waiting for paper...\n");
		if(sem_trywait(&paper) == 0){
			printf("SMOKER_WITH_MATCHES: took paper.\n");
			printf("SMOKER_WITH_MATCHES: table cleared.\n");
			sem_post(&table_cleared);
			printf("SMOKER_WITH_MATCHES: smoking...\n");
			sleep(1);
		}
		else{
			printf("SMOKER_WITH_MATCHES: no paper, put back tobacco.\n");
			sem_post(&tobacco);
			usleep(500000);
		}
	}
}

void new_detached_thread(pthread_t *t, void* (*routine) (void* arg)){
	pthread_create(t, NULL, routine, NULL);
	pthread_detach(*t);
}

int main(){
	sem_init(&tobacco, 0, 0);
	sem_init(&paper, 0, 0);
	sem_init(&matches, 0, 0);
	sem_init(&table_cleared, 0, 0);
	
	srand(time(NULL));
	
	new_detached_thread(&agent, &agent_routine);
	new_detached_thread(&smoker_tobacco, &smoker_tobacco_routine);
	new_detached_thread(&smoker_paper, &smoker_paper_routine);
	new_detached_thread(&smoker_matches, &smoker_matches_routine);
	
	/*pthread_create(&agent, NULL, &agent_routine, NULL);
	pthread_detach(agent);
	pthread_create(&smoker_tobacco, NULL, &smoker_tobacco_routine, NULL);
	pthread_detach(smoker_tobacco);
	pthread_create(&smoker_paper, NULL, &smoker_paper_routine, NULL);
	pthread_detach(smoker_paper);
	pthread_create(&smoker_matches, NULL, &smoker_matches_routine, NULL);
	pthread_detach(smoker_matches);*/
	
	while(1) ;
	
	return 0;
}
