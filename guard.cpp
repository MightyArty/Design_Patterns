#include <pthread.h>
#include <iostream>
#include <mutex>
#include <string.h>
#include <thread>

using namespace std;

void *ptr;  //global pointer
std::mutex mtx;

/**
 * @brief Simple odd printing function
 */
void print_even(void *element){
    element = malloc(sizeof(int));
    if(element == NULL){
        std::cout << "malloc error" << std::endl;
        return;
    }

    int temp = *((int *) element);
    if(temp % 2 == 0){
        std::cout << "Given number is even" << std::endl;
    }
    else throw (std::logic_error("Given number is not even"));
}

void *task(void *p_id){
    try{
        std::lock_guard<std::mutex> lck (mtx);
        print_even(p_id);
    }
    catch(std::logic_error&){
        std::cout << "[exeption caught]" << std::endl;
    }
    return 0;
}

int main(){
    pthread_t thread_arr[10];
    int temp = 0;
    for(int i = 0 ; i < 10 ; i++){
        pthread_create(&thread_arr[i], NULL, task, &temp);
    }
    for(int i = 0 ; i < 10 ; i++){
        pthread_join(thread_arr[i], NULL);
    }
    return 0;
}