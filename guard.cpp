#include <pthread.h>
#include <iostream>
#include <mutex>
#include <string.h>
#include <thread>
#include <stdio.h>
#include <unistd.h>

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
    printf("\033[1;34m");
}
void green()
{
    printf("\033[1;32m");
}
void cyan()
{
    printf("\033[1;36m");
}
void reset()
{
    printf("\033[0m");
}


using namespace std;

void *global_ptr;  //global pointer
pthread_mutex_t mtx_lock;

class guard{
    public:
        guard();
        ~guard();
};

guard::guard(){
    pthread_mutex_lock(&mtx_lock);
    green();
    std::cout << "Initializing guard and mutex lock" << std::endl;
}

guard::~guard(){
    pthread_mutex_unlock(&mtx_lock);
    red();
    std::cout << "Destroying guard and unlocking mutex" << std::endl;
}

void *task(void *args){
    guard guard{
    };
    sleep(3);

    blue();
    std::cout << "Global pointer before change: " << global_ptr << std::endl;
    global_ptr = args;
    int *local_ptr = (int *)global_ptr;
    yellow();
    std::cout << "Global pointer after change: " << *local_ptr << std::endl;
    return NULL;
}

int main(){
    pthread_mutex_init(&mtx_lock, NULL);
    pthread_t threads[4];
    int a = 5;
    for(int i = 0 ; i < 4 ; i++){
        pthread_create(&threads[i], NULL, &task, &a);
    }
    for(int i = 0 ; i < 4 ; i++){
        pthread_join(threads[i], NULL);
    }
    cyan();
    std::cout << "Finished :)" << std::endl;
    reset();
    return 0;
}