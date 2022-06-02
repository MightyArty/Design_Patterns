#ifndef __REACTOR__H__
#define __REACTOR__H__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#define BUFFSIZE 1024

typedef int (*acceptType)(int, sockaddr *__restrict__, socklen_t *__restrict);
typedef int (*recvType)(int, void *__buf, size_t __nbytes);
socklen_t len; // store size of the address
struct sockaddr_in svrAdd, clntAdd;
using namespace std;

namespace re
{
    class Reactor
    {

    private:
        struct pollfd *p_fds;
        void (**handlers)();
        int fd_count;
        int fd_size;
        std::thread mThread;
        int index;
        bool stop;

    private:
        /**
         * @brief This function will get some string
         * trasform capital letters to small
         * and small letters to capital
         * @param str: given string to check
         */
        Reactor();
        /**
         * @brief This function will get some string
         * trasform capital letters to small
         * and small letters to capital
         * @param str: given string to check
         */
        ~Reactor();
        /**
         * @brief This function will get some string
         * trasform capital letters to small
         * and small letters to capital
         * @param str: given string to check
         */
        void run();

    public:
        /**
         * @brief This function will get some string
         * trasform capital letters to small
         * and small letters to capital
         * @param str: given string to check
         */
        void info();
    public:
        /**
         * @brief This function will get some string
         * trasform capital letters to small
         * and small letters to capital
         * @param str: given string to check
         */
        static void *newReactor();
        /**
         * @brief This function will get some string
         * trasform capital letters to small
         * and small letters to capital
         * @param str: given string to check
         */
        static void InstallHandler(Reactor *, int, void (*func)());
        /**
         * @brief This function will get some string
         * trasform capital letters to small
         * and small letters to capital
         * @param str: given string to check
         */
        static void RemoveHandler(Reactor *reactor);
    };

}
/**
 * @brief Initiallize the server side
 * @return int = 1 on success, 0 on failure
 */
int server(int argc, char *argv[]);
#endif //!__REACTOR__H__