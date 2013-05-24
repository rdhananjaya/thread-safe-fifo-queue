#include "queue_ts.h"
#include <time.h>



#define number_of_threads 6 
#define number_of_threads_d 5
#define number_of_iters 1000


#define DEBUG
//#undef DEBUG /*  uncomment when you are done! */
  
#ifdef DEBUG
    #define print printf
#else
    #define print(...)
#endif



/*****************************************
struct _node {
    void *data;
    struct _node * next;
};
typedef struct _node node;

struct _end_q {
   node * first;
   node * last;
};
typedef struct _end_q queue;

****************************************/


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * create and return a new queue
 **/
queue * queue_factory()
{
   queue * new_queue = malloc(sizeof(queue));
   if(new_queue == NULL) {
       fprintf(stderr, "Malloc failed creating the que\n");
       return NULL;
   }
   new_queue->first = NULL;
   new_queue->last = NULL;

   print("Generated the que @ %p\n", new_queue);

   return new_queue;
}

void queue_destroy(queue * que)
{
    
    print("Enterd to queue_destroy\n");

    if(que == NULL) {
        return;
    }

    print("que is not null, que = %p\n", que);

    pthread_mutex_lock(&mutex);
    if(que->first == NULL) {
        print("que->first == NULL .... \n");
        free(que);
        pthread_mutex_unlock(&mutex);
        return;
    }

    print("que is there lets try to free it...\n");

    node * _node = que->first;

    while(_node != NULL) {
        // freeing the data coz it's on the heap and no one to free it
        // except for this one
        print("freeing : %s\n", (char *)_node->data);
        free(_node->data);
        node *tmp = _node->next;
        free(_node);
        _node = tmp;
    }

    free(que);

    pthread_mutex_unlock(&mutex);
}

/**
 * que is a queue pointer
 * data is a heap allocated memory pointer
 */
int enque(queue * que, void * data)
{
    node * new_node = malloc(sizeof(node));
    if(new_node == NULL) {
        fprintf(stderr, "Malloc failed creating a node\n");
        return -1;
    }
    // assumming data is in heap
    new_node->data = data;
    new_node->next = NULL;

    pthread_mutex_lock(&mutex);
    if (que->first == NULL) {
        // new que
        que->first = new_node;
        que->last = new_node;
    } else {
        que->last->next = new_node;
        que->last = new_node;
    }
    pthread_mutex_unlock(&mutex);

    return 0;
}

void * deque(queue * que) 
{
    print("Entered to deque\n");
    if (que == NULL) {
        print("que is null exiting...\n");
        return NULL;
    }


    pthread_mutex_lock(&mutex);
    if (que->first == NULL) {
        pthread_mutex_unlock(&mutex);
        print("que->first is null exiting...\n");
        return NULL;
    }

    void * data;
    node * _node = que->first;
    if (que->first == que->last) {
        que->first = NULL;
        que->last = NULL;
    } else {
        que->first = _node->next;
    }

    data = _node->data;

    print("Freeing _node@ %p", _node);
    free(_node);
    pthread_mutex_unlock(&mutex);
    print("Exiting deque\n");
    return data;
}

struct _stru {
    int number;
    int thread_no;
    queue * q;
};

void * func(void * arg)
{
    struct _stru * args = (struct _stru *) arg;
    int number = args->number;
    int th_number = args->thread_no;
    queue * q = args->q;

    int i;
    srand(time(NULL));
    for(i = 0; i < number; i++) {
        char * message = malloc(16);
        snprintf(message, 15, "rand: %d", rand());
        enque(q, (void *)message);
    }
    
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
}

int main() 
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


