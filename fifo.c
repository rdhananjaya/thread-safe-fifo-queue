#include "queue_ts.h"
#include <time.h>


#define DEBUG
#undef DEBUG /*  uncomment when you are done! */
  
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


