#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include<semaphore.h>

int main(){
	int choice;
	printf("\nWitam w czytelni. Prosze wybrac sposob funkcjonowania biblioteki.\n\n");
	printf("\t\t#===============CZYTELNIA===============#\n"
		"\t\t|[1] Bez zagladzania osob w czytelni.   |\n"
		"\t\t|[2] Zagladzanie czytelnikow.           |\n"
		"\t\t|[3] Zagladzanie pisarzy.               |\n"
		"\t\t#=======================================#\n\n");
	
	scanf("%d", &choice);
	
	switch(choice){
		case 1:
			printf("Wybrales opcje bez zagladzania uzytkownikow.\n");
			system("cc bez_zagladzania.c -o bez -lpthread");
			system("./bez_zagladzania");
			break;

		case 2:
			printf("Wybrales opcje zagladzajaca czytelnikow\n");
			system("cc zagladzanie_czytelnikow.c -o czytelnikow -lpthread");		
			system("./zagladzanie_czytelnikow");
			break;

		case 3:
			printf("Wybrales opcje zagladzajaca pisarzy.\n");
			system("cc zaglodzenie_pisarzy.c -o pisarzy -lpthread");
			system("./zaglodzenie_pisarzy");
			break;
	}
	return 0;
}
