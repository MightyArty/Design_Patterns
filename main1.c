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

/**
 * Implementation of Q1,Q2,Q3
 */

p_queue q;

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

void *task1(void *a){
    sleep(3);
    enQ((void *)"1", q);
    enQ((void *)"2", q);
    enQ((void *)"3", q);
    enQ((void *)"4", q);
    enQ((void *)"5", q);
    return 0;
}

a_obj newAO(struct queue *q, void(*func1)(), void(*func2)()){
    a_obj ao;
    ao.q = q;
    ao.func1 = func1;
    ao.func2 = func2;
    // dealing with every ao object in the queue
    p_node curr = q->root;
    while(curr != NULL){
        func1();
        curr = curr->next;
    }
    printf("Doint something else..\n");
    func2();
    return ao;
}

void destroyAO(a_obj ao){
    red();
    printf("Trying to destroy active object\n");
    destroyQ(ao.q);
    pthread_cancel(ao.func1);
    pthread_cancel(ao.func2);
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
 * if the original letter was capital it will remain capital
 * check if the string have only english letters
 * if not return
 * @param str: given string to encrypt
 */
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
 * @param str: given string to check
 */
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

    printf("1. %c \n", *((char *)deQ(q)));
    printf("2. %c \n", *((char *)deQ(q)));
    printf("3. %c \n", *((char *)deQ(q)));

    pthread_join(thread1[0], NULL);
    pthread_join(thread1[1], NULL);
    return 1;

}