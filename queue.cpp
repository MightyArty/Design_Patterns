#include "queue.hpp"

void destroyQ(p_queue q){
    if(q->root == NULL){
        printf("Queue already destroyed\n");
        return;
    }

    pthread_cond_destroy(&q->q_con);
    pthread_mutex_destroy(&q->q_mutex);

    while(q->root != NULL){
        p_node curr = q->root;
        q->root = q->root->next;
        free(curr);
    }
    q->size = 0;
}

p_queue createQ(){
    p_queue new_q = (p_queue)malloc(sizeof(queue));
    new_q->root = NULL;
    new_q->tail = NULL;
    pthread_cond_init(&new_q->q_con, NULL);
    pthread_mutex_init(&new_q->q_mutex, NULL);
    return new_q;
}

void enQ(void *object, p_queue q){
    p_node new_n = (p_node)malloc(sizeof(Node));
    // if malloc failed
    if(new_n == NULL){
        printf("malloc failed :(\n");
        return;
    }
    new_n->object = object;
    new_n->next = NULL;

    // if there is a tail, connect that tail to the new node
    if(q->tail != NULL){
        q->tail->next = new_n;
    }
    q->tail = new_n;

    if(q->root == NULL){
        q->root = new_n;
    }
    q->size++;
}

void *deQ(p_queue q){
    if(q->root == NULL){
        printf("Queue is empty\n");
        return;
    }

    p_node curr = q->root;
    void *result = curr->object;
    q->root = q->root->next;

    if(q->root == NULL){
        q->tail = NULL;
    }
    q->size--;
    free(curr);
    return result;
}

int isEmpty(p_queue q){
    if(q->size == 0)
        return 0;
    else
        return 1;
}