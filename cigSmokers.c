#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include "queue.h"

queue waiting_threads;

pthread_mutex_t queue_mutex;

sem_t tobacco;
sem_t paper;
sem_t matches;
sem_t table_cleared;

worker agent;
worker smoker_tobacco;
worker smoker_paper;
worker smoker_matches;

void* agent_routine(void* arg){
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
	pthread_mutex_lock(&queue_mutex);
	enqueue(&waiting_threads, agent);
	pthread_mutex_unlock(&queue_mutex);
	printf("AGENT: waiting for table to be cleared...\n");
	pthread_exit(NULL);
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
			printf("SMOKER_WITH_TOBACCO: no matches, put back paper. I'll be back when table will be cleared.\n");
			sem_post(&paper);
			pthread_mutex_lock(&queue_mutex);
			enqueue(&waiting_threads, smoker_tobacco);
			pthread_mutex_unlock(&queue_mutex);
			pthread_exit(NULL);
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
			printf("SMOKER_WITH_PAPER: no matches, put back tobacco. I'll be back when table will be cleared.\n");
			sem_post(&tobacco);
			pthread_mutex_lock(&queue_mutex);
			enqueue(&waiting_threads, smoker_paper);
			pthread_mutex_unlock(&queue_mutex);
			pthread_exit(NULL);
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
			printf("SMOKER_WITH_MATCHES: no paper, put back tobacco. I'll be back when table will be cleared.\n");
			sem_post(&tobacco);
			pthread_mutex_lock(&queue_mutex);
			enqueue(&waiting_threads, smoker_matches);
			pthread_mutex_unlock(&queue_mutex);
			pthread_exit(NULL);
		}
	}
}

void new_detached_thread(pthread_t *t, void* (*routine) (void* arg)){
	pthread_create(t, NULL, routine, NULL);
	pthread_detach(*t);
}

void smoking_hall(){
	while(1){
		while(waiting_threads.ind > 0){
			pthread_mutex_lock(&queue_mutex);
			worker extracted = dequeue(&waiting_threads);
			pthread_mutex_unlock(&queue_mutex);
			new_detached_thread(&extracted.thread_id, extracted.routine);
		}
		sem_wait(&table_cleared);
	}
}

void exit_procedure(int signum){
	pthread_mutex_destroy(&queue_mutex);
	printf("\nProcess interrupted by user.\n");
	exit(0);
}

int main(){

	signal(SIGINT, exit_procedure);

	pthread_mutex_init(&queue_mutex, NULL);

	sem_init(&tobacco, 0, 0);
	sem_init(&paper, 0, 0);
	sem_init(&matches, 0, 0);
	sem_init(&table_cleared, 0, 0);
	
	agent.thread_id = 0;
	smoker_tobacco.thread_id = 1;
	smoker_matches.thread_id = 2;
	smoker_paper.thread_id = 3;

	agent.routine = &agent_routine;
	smoker_tobacco.routine = &smoker_tobacco_routine;
	smoker_matches.routine = &smoker_matches_routine;
	smoker_paper.routine = &smoker_paper_routine;
	
	srand(time(NULL));
	
	enqueue(&waiting_threads,agent);
	enqueue(&waiting_threads,smoker_tobacco);
	enqueue(&waiting_threads,smoker_matches);
	enqueue(&waiting_threads,smoker_paper);
	
	smoking_hall();
	
	return 0;
}
