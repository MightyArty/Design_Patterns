#include "queue.hpp"

struct a_objcet{
    p_queue q;
    void *(*A)(void*);
    void *(*B)(void*);
};

/**
 * @brief this function will get 3 parameters
 * 1. queue
 * 2. function that deals with queue object
 * 3. function that deals with cleanup the queue object
 * @return struct a_object* 
 */
struct a_object *newAO(p_queue *, void* (void*), void* (void*));

/**
 * @brief this function will get 1 parameter
 * the active object and will destroy it
 * by calling to pthread_cancel
 * and will release all data structers
 */
void destroyAO(struct a_object *);