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
    new_n->object = object;
    new_n->next = NULL;

    if(q->root == NULL){
        q->tail = new_n;
        q->root = new_n;
        q->size++;
    }

    // q->root != NULL
    else{
        q->tail->next = new_n;
        q->tail = new_n;
        q->size++;
    }
}

void *deQ(p_queue q){
    if(isEmpty(q) == 0){
        printf("Queue is empty\n");
        return;
    }

    p_node curr = q->root;
    q->root = q->root->next;
    // check if root is null
    if(q->root == NULL){
        q->tail = NULL;
    }

    return curr->object;
    free(curr);
}

int isEmpty(p_queue q){
    if(q->size == 0)
        return 0;
    else
        return 1;
}