#include "queue.hpp"
#include "active_object.hpp"



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

p_queue q;
p_queue q1;
p_queue q2;

/**
 * initializing mutex and condition variable
 */
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

a_obj *newAO(struct queue *q, void(*func1)(), void(*func2)()){
    a_obj *a_o = (a_obj*)malloc(sizeof(a_obj));
    a_o->q = q;
    a_o->func1 = func1;
    a_o->func2 = func2;
    // dealing with every ao object in the queue
    p_node curr = q->root;
    while(curr != NULL){
        func1();
        curr = curr->next;
    }
    printf("Doint something else..\n");
    func2();
    return a_o;
}

void *task1(void *a){
    sleep(2);
    a_obj *a_o = (a_obj *) a;
    newAO(a_o->q, a_o->func1, a_o->func2);
}

void destroyAO(a_obj *ao){
    red();
    printf("Trying to destroy active object\n");
    destroyQ(ao->q);
    pthread_cancel(ao->thread1);
    free(ao);
    green();
    printf("Active object has been destroyed\n");
}

void func1(){
    blue();
    printf("Doing something on every active object\n");
}

void func2(){
    yellow();
    printf("Doing something at the end\n");
}

/**
 * @brief This function will get some string
 * and will activate ceasar cipher on int by key value : 1
 */
void *a_obj1(void *args){
    if(args == NULL)
        return;
    p_node node = (p_node) args;
    char *str = malloc(sizeof(node->object));
    str = node->object;
    ceasar_cipher(str);
    node->object = str;
    return 0;
}

void ceasar_cipher(char *str){
    int len = strlen(str);
    if(len < 1){
        printf("please enter valid string\n");
        return;
    }

    for(int i = 0 ; i < len ; i++){
        if(str[i] < 65 || (str[i] > 90 && str[i] < 97) || str[i] > 122){
            printf("enter valid string between a-z||A-Z");
            return;
        }

        if(str[i] == 'Z'){
            str[i] = 'A';
        }

        if(str[i] == 'z'){
            str[i] = 'a';
        }

        else{
            str[i] += 1;
        }
    }
}

/**
 * @brief This function will get some string
 * trasform capital letters to small
 * and small letters to capital 
 */
void *a_obj2(void *args){
    if(args == NULL)
        return;
    p_node node = (p_node) args;
    char *str = malloc(sizeof(node->object));
    str = node->object;
    transform(str);
    node->object = str;
    return 0;
}

void transform(char *str){
    int len = strlen(str);
    if(len < 1){
        printf("please enter valid string\n");
        return;
    }

    for(int i = 0 ; i < len ; i++){
        // A..Z
        if(str[i] >= 65 && str[i] <= 90){
            str[i] = str[i] + 32;
        }
        // a..z
        else if(str[i] >= 97 && str[i] <= 122){
            str[i] = str[i] - 32;
        }

        else{
            printf("enter valid string between a-z||A-Z");
            return;
        }
    }
}

int main(){
    q = createQ();
    q1 = createQ();
    q2 = createQ();
    enQ("artem", q);
    enQ("barak", q);
    enQ("good", q);
    
    a_obj *first = (a_obj *)malloc(sizeof(a_obj));
    a_obj *second = (a_obj *)malloc(sizeof(a_obj));
    a_obj *third = (a_obj *)malloc(sizeof(a_obj));

    first->q = q;
    second->q = q1;
    third->q = q2;

    first->func1 = a_obj1;
    second->func1 = a_obj2; 


    // pthread_t thread1[2];
    // pthread_create(&thread1[0], NULL, task1, (void *)"thread 1");
    
    // pthread_create(&thread1[1], NULL, task1, (void *)"thread 2");

    // printf("1. %c \n", *((char *)deQ(q)));
    // printf("2. %c \n", *((char *)deQ(q)));
    // printf("3. %c \n", *((char *)deQ(q)));

    // pthread_join(thread1[0], NULL);
    // pthread_join(thread1[1], NULL);
    return 1;

}