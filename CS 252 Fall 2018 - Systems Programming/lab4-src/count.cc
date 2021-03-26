
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int count;
pthread_mutex_t lock;

void increment(int ntimes )
{
	pthread_mutex_lock(&lock);

	for ( int i = 0; i < ntimes; i++ ) {
		int c;

		c = count;
		c = c + 1;

		count = c;
	}

	pthread_mutex_unlock(&lock);
}

int main( int argc, char ** argv )
{
	int n = 10000000;
	pthread_t t1, t2;
        pthread_attr_t attr;

        pthread_attr_init( &attr );
        pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	if(pthread_mutex_init(&lock, NULL) != 0){
		printf("mutex init failed\n");
		return 1;
	}

	printf("Start Test. Final count should be %d\n", 2 * n );

	// Create threads
	pthread_create( &t1, &attr, (void * (*)(void *)) increment, 
			(void *) n);

	pthread_create( &t2, &attr, (void * (*)(void *)) increment, 
			(void *) n);

	// Wait until threads are done
	pthread_join( t1, NULL );
	pthread_join( t2, NULL );
	pthread_mutex_destroy(&lock);

	if ( count != 2 * n ) {
		printf("\n****** Error. Final count is %d\n", count );
		printf("****** It should be %d\n", 2 * n );
	}
	else {
		printf("\n>>>>>> O.K. Final count is %d\n", count );
	}
}


