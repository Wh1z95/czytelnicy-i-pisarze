//Zag³odzenie pisarzy

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include<semaphore.h>

#define WRITERS 2 //iloœæ pisarzy
#define READERS 2 //iloœæ czytelników

//Deklarujemy globalnie semafory, ¿eby w¹tki mia³y do nich dostêp
sem_t mutex;
sem_t acces;
int readers_count;

//Funkcja dla pisarza
void* Writer_TH(void* arg){
	while(1){
		int *tmp = (int*)arg;
		printf("Writer %d waiting.\n", *tmp);
		//sleep(1);
		sem_wait(&acces); //Blokujemy dostêp do czytelni waitem
		printf("Writer %d working.\n",*tmp);
		sleep(2); //Tak jakby jego praca - sekunda
		sem_post(&acces); //Zwalniamy semafor
	}
}
	
//Funkcja dla czytelnika
void* Reader_TH(void* arg){
	while(1){
		sem_wait(&mutex);
		readers_count++;
		
		if(readers_count == 1){
			//Blokujemy semafor dla pisarzy je¿eli readers_count > 0
			sem_wait(&acces); 
		}
		//Odblokowujemy mutex czyli jakby dostêp dla czytelników
		sem_post(&mutex); 
        printf("Readers in library: %d.\n", readers_count);
		sleep(1); //Czyta, tak jak pisarz pracuje - sekunda
		sem_wait(&mutex); //Blokujemy wejœcie dla czytelników
		readers_count--; //Zmniejszamy a¿ wszyscy wyjd¹
		if(readers_count == 0){
			sem_post(&acces); //je¿eli wszystkich usunêliœmy to mo¿emy odblokowaæ dostêp
						     //dla pisarzy
		}
                  
		sem_post(&mutex); //na sam koniec znów odblokowujemy czytelników
	}
}

int main(){
	int i;
    sem_init(&mutex, 0, 1);		
    sem_init(&acces,0, 1);
    int tab[WRITERS];
	
	pthread_t WritersThreads[WRITERS]; //tablica z w¹tkami pisarzy
    pthread_t ReadersThreads[READERS]; //tablica z w¹tkami czytelników
	
	for(i = 0; i < WRITERS; i++){ //dla ka¿dego pisarza tworzymy w¹tek
		tab[i] = i;
		pthread_create(&WritersThreads[i], NULL, Writer_TH, &tab[i]);
	}
	
	for(i = 0; i < READERS; i++){ //to co wy¿ej, tylko dla czytelników
		pthread_create(&ReadersThreads[i], NULL, Reader_TH, NULL);
	}
	
	for(i = 0; i < WRITERS; i++){ //czekamy na zakoñczenie watkow
		pthread_join(WritersThreads[i], NULL);
	}
	
	for(i = 0; i < READERS; i++){ //to co wy¿ej
		pthread_join(ReadersThreads[i], NULL);
	}
}