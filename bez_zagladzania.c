//Bez zag�adzania

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

#define WRITERS_SPOTS 4	 //ilo�� pisarzy
#define READERS_SPOTS 10 //ilo�� czytelnik�w

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
	int *tmp = (int*)arg; //rzutowanie zmiennej arg typu void na zmienn� tmp typu int
	
	while(1){ 
		sem_wait(&acces_sem); //blokujemy dost�p
		waiting_writers++;		
		
		if(waiting_readers == 0){ //je�eli nie ma czekaj�cych czytelnik�w wpuszczamy pisarzy
			working_writers++;
			sem_post(&acces_sem); //zwalniamy dost�p
		}
		
		else{
			sem_post(&acces_sem);	//je�eli s� czytelnicy czekamy na zwolnienie semafora acces
			sem_wait(&writers);	//pisarz czeka na dost�p
		}
	
		sem_wait(&writers_block);	//pisarz wchodzi wi�c blokujemy dost�p dla innych pisarzy
		printf("Writer %d working.\n", *tmp); sleep(1);
		sem_post(&writers_block); 	//koniec pracy, odblokowujemy
		sem_wait(&acces_sem);
		working_writers--; 
		waiting_writers--;
		
		if(working_writers == 0){	//je�eli wszyscy pisarze wyszli wpusczczamy czytelnik�w
			do{
				working_readers++;
				sem_post(&readers);	//wpuszczamy czytelnik�w
			}
			while(waiting_readers > working_readers);
		} 
		
		sem_post(&acces_sem);
	}
}

void* Reader_TH(void* arg){
	int i;
	int *tmp = (int*)arg; //rzutowanie, jak wy�ej
	
	while(1){ 
		sem_wait(&acces_sem);	//blokujemy dost�p
		waiting_readers++;		
		
		if(waiting_writers == 0){ //je�eli nie ma czekaj�cych pisarzy wpuszczamy czytelnik�w
			working_readers++;	
			sem_post(&acces_sem); //zwalniamy dost�p
		}
		
		else{
			sem_post(&acces_sem); //zwalniamy dost�p
			sem_wait(&readers);	//czytelnik czeka na dost�p
		}
	
		printf("Readers in library: %d.\n", *tmp); sleep(1);
		sem_wait(&acces_sem);
		working_readers--; 
		waiting_readers--;
	
		if(working_readers == 0){	//je�eli nie ma ju� czytelnik�w mo�emy wpuszcza� pisarzy
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

    sem_init(&writers, 0, 0);		//semafor zliczaj�cy
	sem_init(&readers,0,0);        		//semafor zliczaj�cy
	sem_init(&acces_sem,0, 1);		//semafor binarny
	sem_init(&writers_block,0,1);		//semafor binarny
	

    pthread_t WritersThreads[WRITERS_SPOTS];	//tablica z w�tkami pisarzy, wielko�� narzucona z g�ry
    pthread_t ReadersThreads[READERS_SPOTS];	//tabilca z w�tkami czytelnikow, wielko�� narzucona z g�ry

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