//Zag³adzanie czytelników

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include<semaphore.h>

#define WRITERS 1 //iloœæ pisarzy
#define READERS 10 //iloœæ czytelników

int readers_waiting_count;
int readers_count;

sem_t mutex;
sem_t acces_sem;

//Nie wpuszczamy czytelników tak d³ugo, jak czeka jakiœ pisarz
void* Writer_TH(void *arg){
	while(1){
		int *tmp = (int*)arg;
		readers_waiting_count++;
		sem_wait(&mutex);

		printf("Writer %d waiting.\n", *tmp);
		sem_post(&mutex);
		sem_wait(&acces_sem);

		printf("Writer %d working.\n", *tmp);
		sleep(2); 
		readers_waiting_count--;
		sem_post(&acces_sem);
	}
}

void *Reader_TH(void *arg){
	while(1){
		if(readers_waiting_count == 0){
			sem_wait(&mutex);
			readers_count++;
			sem_post(&mutex);

			if(readers_count == 1)
				sem_wait(&acces_sem);
			
			printf("Readers in library: %d.\n", readers_count);
			sleep(1);
			sem_wait(&mutex);
			readers_count--;

			if(readers_count == 0)
				sem_post(&acces_sem);
			sem_post(&mutex);
		}
	}
}

int main(){
	int i;
	sem_init(&mutex, 0, 1);
    sem_init(&acces_sem,0, 1);
    int tab[WRITERS];
	
	pthread_t WritersThreads[WRITERS];
    pthread_t ReadersThreads[READERS];
	
	for(i = 0; i < WRITERS; i++){
		tab[i] = i;
		pthread_create(&WritersThreads[i], NULL, Writer_TH, &tab[i]);
	}
	
	for(i = 0; i < READERS; i++){
		pthread_create(&ReadersThreads[i], NULL, Reader_TH, NULL);
	}
	
	for(i = 0; i < WRITERS; i++){
		pthread_join(WritersThreads[i], NULL);
	}
	
	for(i = 0; i < READERS; i++){
		pthread_join(ReadersThreads[i], NULL);
	}
}