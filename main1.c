#include "queue.h"

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
