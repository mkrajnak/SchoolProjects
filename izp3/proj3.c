/*
*	Author 	Martin Krajnak, xkrajn02@stud.fit.vutbr.cz
*	Date 	18.4.2015
*	Desc	Project 2 for IOS course at fit.vutbr.cz
*			Building H2O
*/

//parametre su cele cisla

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <signal.h>
#include <time.h>
#include <string.h>


#define MAXTIME 5000
#define H 'H'
#define O 'O'
#define STARTED ":started"
#define WAITING ":waiting"
#define READY ":ready"
#define BEGINBONDING ":begin bonding"
#define BONDED ":bonded"
#define FINISHED ":finished"

typedef struct{
sem_t *mutex, *hQ, *oQ, *barrier, *writeout, *bonder, *bonding, *allBonded;
signed int *hydrogen = NULL;	//hydrogen atoms counter
signed int *oxygen = NULL;		//oxygen atoms counter
signed int *counter=NULL;
signed int *sharedCounter = NULL;int *bonded = NULL;
} Share;

int sharedId=0;
Shared *share=NULL;

void clean(void){

	return;
}
void init(void){
	if ((sharedId = shmget(IPC_PRIVATE, sizeof(shared), IPC_CREAT | 0666)) == -1) {
		printf("ERR3\n");
	}
	if ((shared =(shared *)shmat(allBondedId,NULL,0)) ==(shared *)(-1)) 
	{
		printf("ERR4\n");
		clean();
		return;
	}
	return;
}

int int main(int argc, char const *argv[])
{
	init();
	return 0;
}

