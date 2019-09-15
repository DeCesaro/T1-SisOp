//Guilherme Dall'Agnol Deconto
//Gustavo Possebon Machado
//Bernardo de Cesaro
//Pontifícia Universidade católica do Rio Grande do Sul
//Sistemas Operacionais

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define portions 10 // ammount of portions the cook makes
#define numCanibals 5 // cannibals ammount
#define cookTime 5 // time for the food to be ready
#define eatTime 1 // time it takes for a cannibal to eat
int initialPortions = 10; // ammount of initial portions

pthread_mutex_t cook = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t row =  PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t serv = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t foodReady = PTHREAD_MUTEX_INITIALIZER;

void *Cooking(){
    int light;
    while (1) {
        pthread_mutex_lock(&row);
        printf("Mutex row locked\n");
        if (initialPortions > 0) {
            printf("Canibal is eating before else\n Portion number : %d\n", initialPortions);
            initialPortions --;
            sleep(eatTime);
        }else{
            printf("Call the cook\n");
            pthread_mutex_lock(&serv);
            pthread_mutex_unlock(&cook);
            pthread_mutex_lock(&foodReady);
            printf("Cnibal eating after else\n");
            initialPortions--;
            pthread_mutex_unlock(&serv);
        }
        printf("Canibal is ready\n");
        fflush(stdout);
        pthread_mutex_unlock(&row);
    }
}

void *Cook(){
    while (1) {
        pthread_mutex_lock(&cook);
        printf("Calling the Cook, he is cooking....\n");
        initialPortions = portions;
        sleep(cookTime);
        printf("Cook is making food\n");
        fflush(stdout);
        pthread_mutex_unlock(&foodReady);
    }
}

int main(){
    int rc;
    pthread_t canibal[numCanibals];
    pthread_t cook;
    pthread_mutex_lock(&cook);
    pthread_mutex_unlock(&serv);
    pthread_mutex_unlock(&row);
    pthread_mutex_lock(&foodReady);
    for (int i = 0; i < numCanibals; i++) {
        rc = pthread_create(&canibal[i], NULL, Cooking, NULL);
        printf("Creating canibal %d\n", i);
        if(rc){
            printf("Error creating Canibal Thread : %d\n", i);
        }
    }
    rc = pthread_create(&cook, NULL, Cook, NULL);
    printf("Hiring cook\n");
    if(rc){
        printf("Error creating Canibal Thread\n");
    }
    pthread_join(cook, NULL);
}
