#include "queue.hpp"

typedef struct a_objcet{
    struct queue *q;
    void (*func1)();
    void (*func2)();
}a_obj;

/**
 * @brief this function will get 3 parameters
 * 1. queue
 * 2. function that deals with queue object
 * 3. function that deals with cleanup the queue object
 * @return struct a_object* 
 */
a_obj newAO(struct queue *, void (*func1)(), void (*func2)());

/**
 * @brief this function will get an active object
 * and will destroy it
 */
void destroyAO(a_obj ao);