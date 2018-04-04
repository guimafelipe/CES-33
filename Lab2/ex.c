#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define N 10
#define WINAPI
#define debugtxt(FORMAT) printf(" TID %d: " #FORMAT "\n",(int) pthread_self())
#define debug(FORMAT, ARGS...) printf("TID %d: " #FORMAT "\n",(int) pthread_self(),ARGS)
#define NTHREADS 37500000
#define NPOINTS 10000000000

typedef void *LPVOID;

// Funcoes e variaveis do buffer
int buffer[N];
long long redPoints;
long long totPoints;
int nthreads = NTHREADS;
long long npoints = NPOINTS;    
unsigned int seedrand;

// Funcoes e variaveis das threads
pthread_cond_t handleThread[NTHREADS];
pthread_t threadId[NTHREADS];
sem_t mutex; //pthread_cond_t mutex;

// Truque para sabermos qual o semaforo foi chamado e poder imprimi-lo
#define up(SEM) _up(SEM,#SEM)
#define down(SEM) _down(SEM,#SEM)

void _up(sem_t *sem, const char * name) {
	// debug("Up %s ...",name);
	sem_post(sem);
	// debug("Up %s complete!",name);
}
void _down(sem_t *sem, const char * name) {
	// debug("Down %s ...",name);
	sem_wait(sem);
	// debug("Down %s complete!",name);
}

void addPoints(long long redToAdd, long long totToAdd){
    redPoints += redToAdd; //Add to global variables
    totPoints += totToAdd;
}

unsigned long WINAPI myThreadFunc(LPVOID lpParam){
    // debugtxt("Thread criada");
    long long currRedPoints = 0;
    long long currTotPoints = 0;    
    for(int i = 0; i < npoints/nthreads; i++){
        double posX = (double)rand_r(&seedrand)/(double)RAND_MAX; //Generate random number between 0 and 1
        double posY = (double)rand_r(&seedrand)/(double)RAND_MAX;
        posX = -1 + 2*posX; //Interpolate it in square range
        posY = -1 + 2*posY;
        // printf("TID %d: Ponto gerado: %lf, %lf\n", (int)pthread_self(), posX, posY);
        if(posX*posX + posY*posY < 1){
            currRedPoints++; //Update local variables for counting
        }
        currTotPoints++; //Update local variable
    }
    down(&mutex); // Ask mutex for premission
    addPoints(currRedPoints, currTotPoints);
    up(&mutex); //Free mutex
}

int main(int argc, char **argv) {
	int i;

    if(argc >= 2) nthreads = atoi(argv[1]); //Get nthreads from argv if exists
    if(argc >= 3) npoints = atoi(argv[2]); //Get npoints from argv if exists

    redPoints = 0; totPoints = 0; //Reset global counting

	sem_init (&mutex, 0, 1); //Initialize mutex

    clock_t begin = clock(); //Start execution time counting

    for(i=0; i < nthreads; i++) { // Creates all threads
        pthread_create (&threadId[i],
                NULL,
                &myThreadFunc,
                NULL);
    }

    for(i=0; i < nthreads; i++) { //Waits for threads
        pthread_join (threadId[i], NULL);		
    }

    clock_t end = clock(); //End time counting

    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC; //Get time spent

    printf("Temos %lld pontos e %lld deles são vermelhos.\n", totPoints, redPoints);
    printf("Resultado final: pi = %lf\n", 4*(double)redPoints/((double)totPoints));
    printf("O tempo de execução foi %lf\n", time_spent); //Results printed
}
