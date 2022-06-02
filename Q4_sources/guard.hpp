#ifndef __GUARD__H__
#define __GUARD__H__

#include <pthread.h>
#include <iostream>
#include <mutex>
#include <string.h>
#include <thread>
#include <stdio.h>
#include <unistd.h>
#include "../color.hpp"
using namespace std;

using namespace std;

void *global_ptr; // global pointer
pthread_mutex_t mtx_lock;

namespace design
{
    class guard
    {
    public:
        /**
         * @brief This function will get some string
         * trasform capital letters to small
         * and small letters to capital
         * @param str: given string to check
         */
        guard();
        /**
         * @brief This function will get some string
         * trasform capital letters to small
         * and small letters to capital
         * @param str: given string to check
         */
        ~guard();
    };
}

#endif //!__GUARD__H__