#include "queue.hpp"
#include <iostream>

p_queue q;

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
// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock;
pthread_cond_t cond;

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
    pthread_mutex_lock(&lock);
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
    }

    // update the tail to the new node
    else {
        q->tail->next = new_n;
        q->tail = new_n;
    }
    q->size++;
    pthread_mutex_unlock(&lock);
    pthread_cond_broadcast(&cond);
    printf("end\n");
}

void *deQ(p_queue q){
    pthread_mutex_lock(&lock);

    if(q->root == NULL){
        printf("Queue is empty\n");
        pthread_cond_wait(&cond, &lock);
    }
    printf("hey\n");
    p_node curr = q->root;
    void *result = curr->object;
    q->root = q->root->next;

    if(q->root == NULL){
        q->tail = NULL;
    }
    q->size--;
    pthread_mutex_unlock(&lock);
    free(curr);
    return result;
}

int isEmpty(p_queue q){

    if(q->size == 0)
        return 0;
    else
        return 1;
}

void *task1(void *a){
    sleep(3);
    enQ((void *)"1", q);
    enQ((void *)"2", q);
    enQ((void *)"3", q);
    enQ((void *)"4", q);
    enQ((void *)"5", q);
    return 0;
}

int main(){
    q = createQ();
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init has failed\n");
        return 0;
    }

    if (pthread_cond_init(&cond, NULL) != 0)
    {
        printf("\n mutex init has failed\n");
        return 0;
    }
    pthread_t thread1[2];
    pthread_create(&thread1[0], NULL, task1, (void *)"thread 1");
    
    pthread_create(&thread1[1], NULL, task1, (void *)"thread 2");
    
    std::cout << *((char *)deQ(q)) << std::endl;

    printf("deqq\n");

    pthread_join(thread1[0], NULL);
    pthread_join(thread1[1], NULL);
    return 1;

}
