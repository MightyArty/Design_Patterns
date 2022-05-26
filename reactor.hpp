#include <iostream>
#include <pthread.h>

typedef struct Reactor{
    pthread_t thread_id;
}reactor, *p_reactor;