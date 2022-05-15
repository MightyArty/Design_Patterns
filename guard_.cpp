#include <pthread.h>
#include <iostream>
#include <mutex>
#include <string.h>
#include <thread>

void *ptr;
int global = 0;
std::mutex mtx;

void *task(void *id){
    std::lock_guard<std::mutex> lock(mtx);
    for(int i = 0 ; i < 1000 ; i++){
        global++;
    }

    int temp = *(int *)id;
    temp = temp + 1;
    void *curr = (void *)malloc(sizeof(int));
    curr = ptr;
    return curr;
}

int main(){
    pthread_t thread_arr[10];
    std::cout << "ptr before :" << ptr << std::endl;
    std::cout << "global before :" << global << std::endl;

    int runner = 0;

    for(int i = 0 ; i < 10 ; i++){
        pthread_create(&thread_arr[i], NULL, task, &runner);
    }
    for(int i = 0 ; i < 10 ; i++){
        pthread_join(thread_arr[i], NULL);
    }
    std::cout << "ptr after :" << ptr << std::endl;
    std::cout << "global after :" << global << std::endl;

    return 0;
}