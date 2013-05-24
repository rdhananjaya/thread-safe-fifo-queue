#include <stdio.h>
#include "queue_ts.h"

#define number_of_threads	6 
#define number_of_threads_d 5
#define number_of_iters		1000
 
#define DEBUG
//#undef DEBUG /*   uncomment when you are done! */
 
#ifdef DEBUG
	#define print printf
#else
    #define print(...)
#endif



struct _stru {
    int number;
    int thread_no;
    queue * q;
};
 
void * func(void * arg)
{
    struct _stru * args = (struct _stru *) arg;
    int number = args->number;
    //int th_number = args->thread_no;
    queue * q = args->q;

    int i;
    srand(time(NULL));
    for(i = 0; i < number; i++) {
        char * message = malloc(16);
        snprintf(message, 15, "rand: %d", rand());
        enque(q, (void *)message);
    }

    return NULL;
}


void * func_d(void * args)
{
    print("Func_D running....\n");

    queue * q = (queue *) args;

    void * data;
    while((data = deque(q)) != NULL) {
        char * string = (char *)data;
        print("DeQued : %s, @%p\n", string, data);
        free(data);
    }
    print("Func_D exiting....\n");

    return NULL;
}

int main(void) 
{
    queue * q = queue_factory();

    pthread_t threads[number_of_threads];
    pthread_t thread_d[number_of_threads_d];
    
    int i;
    struct _stru arg[number_of_threads];
    for(i = 0; i < number_of_threads; i++) {
        arg[i].number = number_of_iters;
        arg[i].thread_no = i;
        arg[i].q = q;
        pthread_create(threads+i, NULL, func, (void *)&arg[i]); 
    }

    for(i = 0; i < number_of_threads_d; i++) {
        pthread_create(thread_d+i, NULL, func_d, (void *)q);
    }

    for(i = 0; i < number_of_threads; i++) {
        pthread_join(*(threads+i), NULL);
    }
    
    for(i = 0; i < number_of_threads_d; i++) {
        pthread_join(*(thread_d+i), NULL);   
    }

    print("Going to run queue_destroy(...) \n");
    queue_destroy(q);
    print("que freeed... :) \n");

    return 0;
}
