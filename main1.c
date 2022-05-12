#include "active_object.h"

void red()
{
    printf("\033[1;31m");
}
void yellow()
{
    printf("\033[1;33m");
}
void blue()
{
    printf("\033[0;34m");
}
void green()
{
    printf("\033[0;32m");
}
void reset()
{
    printf("\033[0m");
}

/**
 * initializing mutex and condition variable
 */
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

p_queue createQ(){
    p_queue new_q = (p_queue)malloc(sizeof(queue));
    new_q->root = NULL;
    new_q->tail = NULL;
    return new_q;
}

void destroyQ(p_queue q){
    if(q->root == NULL){
        printf("Queue already destroyed\n");
        return;
    }

    while(q->root != NULL){
        p_node curr = q->root;
        q->root = q->root->next;
        free(curr);
    }
    q->size = 0;
}

void enQ(void *object, p_queue q){
    p_node new_n = (p_node)malloc(sizeof(Node));
    if(new_n == NULL){
        printf("malloc failed :(\n");
        return;
    }

    new_n->object = object;
    new_n->next = NULL;

    if(q->root == NULL){
        q->root = new_n;
        q->tail = new_n;
        pthread_cond_signal(&cond);
        return;
    }

    // update the tail to the new node
    q->tail->next = new_n;
    q->tail = new_n;
    q->size++;
}

void *deQ(p_queue q){
    pthread_mutex_lock(&lock);

    if(q->root == NULL){
        printf("Queue is empty\n");
        pthread_cond_wait(&cond, &lock);
    }

    p_node curr = q->root;
    void *result = curr->object;
    q->root = q->root->next;

    if(q->root == NULL){
        q->tail = NULL;
    }
    q->size--;
    free(curr);
    pthread_mutex_unlock(&lock);
    return result;
}

int isEmpty(p_queue q){

    if(q->size == 0)
        return 0;
    else
        return 1;
}

void *newAO(p_queue q, void *(*func1)(void *), void *(*func2)(void *)){
     while(q->root != NULL){
         red();
         printf("Multiplying the node by 4\n");
         func1(q->root->object);
     }
     yellow();
     printf("Deviding all nodes by 2\n");
     reset();
     func2(q);
}

void *func1(void *element){
    // need to do something on every element of the queue
    if(element == NULL){
        printf("node does not exist\n");
    }

    element = (int)element * 4;
}

void *func2(p_queue q){
    // need to do something after func1() finish to run
    if(q->root == NULL){
        printf("Queue is empty\n");
    }

    while(q->root != NULL){
        q->root->object = (int)q->root->object / 2; // deviding every node by 2
    }

}

int main(){
    p_queue q = createQ();

    deQ(q); // should throw
    enQ((int *)1, q);
    enQ((int *)2, q);
    enQ((int *)3, q);
    enQ((int *)4, q);
    deQ(q); //1
    deQ(q); //2
    printf("root is: %d", (int *)(q->root->object));
    printf("tail is: %d", (int *)(q->tail->object));
    destroyQ(q);

}
