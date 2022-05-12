#include <string>
#include "queue.hpp"
#include "active_object.hpp"
using namespace std;



active_object::active_object(/* args */)
{
}

active_object::~active_object()
{
}

void active_object::newAO(p_queue q, void *(*func1)(void *), void *(*func2)(void *)){
    
}
void active_object::destroyAO(struct a_object *a_obj){

}
void *newAO(p_queue q, void *(*func1)(void *), void *(*func2)(void *)){
     while(q->root != NULL){
         printf("Multiplying the node by 4\n");
         func1(q->root->object);
     }
     printf("Deviding all nodes by 2\n");
     func2(q);
     return 0;
}

void *func1(void *element){
    // need to do something on every element of the queue
    if(element == NULL){
        printf("node does not exist\n");
    }

    element = (void *)((int)element * 4);

}

void *func2(p_queue q){
    // need to do something after func1() finish to run
    if(q->root == NULL){
        printf("Queue is empty\n");
    }

    while(q->root != NULL){
        q->root->object = (void *)((int)q->root->object / 2); // deviding every node by 2
    }

}