//Bez zag³adzania

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

#define WRITERS_SPOTS 4	 //iloœæ pisarzy
#define READERS_SPOTS 10 //iloœæ czytelników

int waiting_writers = 0;	
int working_writers = 0;
int waiting_readers = 0;
int working_readers = 0;

sem_t writers;
sem_t readers;
sem_t acces_sem;
sem_t writers_block;

void *Writer_TH(void *arg){
	int i;
	int *tmp = (int*)arg; //rzutowanie zmiennej arg typu void na zmienn¹ tmp typu int
	
	while(1){ 
		sem_wait(&acces_sem); //blokujemy dostêp
		waiting_writers++;		
		
		if(waiting_readers == 0){ //je¿eli nie ma czekaj¹cych czytelników wpuszczamy pisarzy
			working_writers++;
			sem_post(&acces_sem); //zwalniamy dostêp
		}
		
		else{
			sem_post(&acces_sem);	//je¿eli s¹ czytelnicy czekamy na zwolnienie semafora acces
			sem_wait(&writers);	//pisarz czeka na dostêp
		}
	
		sem_wait(&writers_block);	//pisarz wchodzi wiêc blokujemy dostêp dla innych pisarzy
		printf("Writer %d working.\n", *tmp); sleep(1);
		sem_post(&writers_block); 	//koniec pracy, odblokowujemy
		sem_wait(&acces_sem);
		working_writers--; 
		waiting_writers--;
		
		if(working_writers == 0){	//je¿eli wszyscy pisarze wyszli wpusczczamy czytelników
			do{
				working_readers++;
				sem_post(&readers);	//wpuszczamy czytelników
			}
			while(waiting_readers > working_readers);
		} 
		
		sem_post(&acces_sem);
	}
}

void* Reader_TH(void* arg){
	int i;
	int *tmp = (int*)arg; //rzutowanie, jak wy¿ej
	
	while(1){ 
		sem_wait(&acces_sem);	//blokujemy dostêp
		waiting_readers++;		
		
		if(waiting_writers == 0){ //je¿eli nie ma czekaj¹cych pisarzy wpuszczamy czytelników
			working_readers++;	
			sem_post(&acces_sem); //zwalniamy dostêp
		}
		
		else{
			sem_post(&acces_sem); //zwalniamy dostêp
			sem_wait(&readers);	//czytelnik czeka na dostêp
		}
	
		printf("Readers in library: %d.\n", *tmp); sleep(1);
		sem_wait(&acces_sem);
		working_readers--; 
		waiting_readers--;
	
		if(working_readers == 0){	//je¿eli nie ma ju¿ czytelników mo¿emy wpuszczaæ pisarzy
			do{
				working_writers++;
				sem_post(&writers);
			}
			while(waiting_writers > working_writers);
		}
		
		sem_post(&acces_sem);
	}	
}

int main(){
	int i,j;

    sem_init(&writers, 0, 0);		//semafor zliczaj¹cy
	sem_init(&readers,0,0);        		//semafor zliczaj¹cy
	sem_init(&acces_sem,0, 1);		//semafor binarny
	sem_init(&writers_block,0,1);		//semafor binarny
	

    pthread_t WritersThreads[WRITERS_SPOTS];	//tablica z w¹tkami pisarzy, wielkoœæ narzucona z góry
    pthread_t ReadersThreads[READERS_SPOTS];	//tabilca z w¹tkami czytelnikow, wielkoœæ narzucona z góry

    int tab1[WRITERS_SPOTS];
	int tab2[READERS_SPOTS];
	
	for(i = 0; i < WRITERS_SPOTS; i++){
		tab1[i] = i;
        pthread_create(&WritersThreads[i], NULL, Writer_TH, &tab1[i]);
	}
	
	for(j = 0; j < READERS_SPOTS; j++){
		tab2[i] = i;          
	    pthread_create(&ReadersThreads[j], NULL, Reader_TH, &tab2[j]);
	}
	
	for(i = 0; i < WRITERS_SPOTS; i++){
		pthread_join(WritersThreads[i], NULL);
	}
	
	for(j = 0; j < READERS_SPOTS; j++){
		pthread_join(ReadersThreads[j], NULL);
	}
}