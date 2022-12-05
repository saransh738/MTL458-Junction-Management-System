#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

pthread_mutex_t rng_mutex;

int thread_safe_rng(int min, int max) 
{
    pthread_mutex_lock(&rng_mutex);
    int r = rand();
    pthread_mutex_unlock(&rng_mutex);
    return min + r % max;
}

/* TODO : can add global vars, structs, functions etc */
pthread_mutex_t L1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t L2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t L3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t L4 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t L5 = PTHREAD_MUTEX_INITIALIZER;
int l1=0,l2=0,l3=0,l4=0;
int count=0;

void arriveLane(char c) 
{
	/* TODO: add code here */
	if(c=='N')
	{
		pthread_mutex_lock(&L5);
		pthread_mutex_lock(&L1);
		l1=1;
	}
    else if(c=='E')
	{
		pthread_mutex_lock(&L4);
		l2=1;
	}
	else if(c=='W')
	{
		pthread_mutex_lock(&L2);
		l3=1;
	}
	else if(c=='S')
	{
		pthread_mutex_lock(&L3);
		l4=1;
	}
}

void crossLane(char c) 
{
    /* TODO: add code here */
	if(c=='N')
	{
		pthread_mutex_lock(&L2);
	}
    else if(c=='E')
	{
		pthread_mutex_lock(&L1);
    }
	else if(c=='W')
	{
		pthread_mutex_lock(&L3);
	}
	else if(c=='S')
	{
		pthread_mutex_lock(&L4);
	}
    usleep(1000 * thread_safe_rng(500, 1000)); // take 500-1000 ms to cross the lane
}

void exitLane(char c) 
{
    /* TODO: add code here */
	if(c=='N')
	{
		pthread_mutex_unlock(&L1);
		pthread_mutex_unlock(&L2);
		pthread_mutex_unlock(&L5);
		l1=0;
	}
    else if(c=='E')
	{
		pthread_mutex_unlock(&L1);
		pthread_mutex_unlock(&L4);
		l2=0;
	}
	else if(c=='W')
	{
		pthread_mutex_unlock(&L2);
		pthread_mutex_unlock(&L3);
		l3=0;
	}
	else if(c=='S')
	{
		pthread_mutex_unlock(&L3);
		pthread_mutex_unlock(&L4);
		l4=0;
	}
}



void* trainThreadFunction(void* arg)
{
    /* TODO extract arguments from the `void* arg` */
    usleep(thread_safe_rng(0, 10000)); // start at random time

    char* trainDir = NULL; // TODO set the direction of the train: North/South/East/West.

    char* c1 = arg;
	char c = *c1;
    if(c=='N')
	{
		trainDir="North";
	}
	else if(c=='E')
	{
		trainDir="East";
	}
	else if(c=='W')
	{
		trainDir="West";
	}
	else if(c=='S')
	{
		trainDir="South";
	}
    arriveLane(c);
    printf("Train Arrived at the lane from the %s direction\n", trainDir);

    crossLane(c);

    printf("Train Exited the lane from the %s direction\n", trainDir);
    exitLane(c);
}

void* deadLockResolverThreadFunction(void * arg) 
{
    /* TODO extract arguments from the `void* arg` */
    while (1)
	{
        /* TODO add code to detect deadlock and resolve if any */
        if(count==1)
		{
			break;
		}
        int deadLockDetected = 0; // TODO set to 1 if deadlock is detected
        if(l1==1 && l2==1 && l3==1 && l4==1)
		{
			deadLockDetected=1;
		}
        if (deadLockDetected) 
		{
			/* TODO add code to resolve deadlock */
			pthread_mutex_unlock(&L1);
            printf("Deadlock detected. Resolving deadlock...\n");
        }
        
        usleep(1000 * 500); // sleep for 500 ms
    }
}




int main(int argc, char *argv[]) 
{

   
    srand(time(NULL));

    if (argc != 2) 
	{
        printf("Usage: ./main <train dirs: [NSWE]+>\n");
        return 1;
    }

    pthread_mutex_init(&rng_mutex, NULL);


    /* TODO create a thread for deadLockResolverThreadFunction */

    pthread_t p1;
	pthread_create(&p1, NULL,deadLockResolverThreadFunction,NULL); 
	
    
    char* train = argv[1];
	
    int n = strlen(train);
    int num_trains = 0;
    pthread_t T1[n];
	
    while (train[num_trains] != '\0') 
	{
        char trainDir = train[num_trains];

        if (trainDir != 'N' && trainDir != 'S' && trainDir != 'E' && trainDir != 'W')
		{
            printf("Invalid train direction: %c\n", trainDir);
            printf("Usage: ./main <train dirs: [NSEW]+>\n");
            return 1;
        }

        /* TODO create a thread for the train using trainThreadFunction */
		pthread_create(&T1[num_trains], NULL,trainThreadFunction,&train[num_trains]); 
		num_trains++;
    }

    /* TODO: join with all train threads*/
	for(int i=0;i<n;i++)
	{
		pthread_join(T1[i],NULL); 
	}
	count=1;
	pthread_join(p1, NULL); 

	return 0;
}